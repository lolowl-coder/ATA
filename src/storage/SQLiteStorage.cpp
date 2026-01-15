#include "storage/SQLiteStorage.hpp"

#include <sqlite3.h>
#include <stdexcept>

namespace {

int64_t toEpoch(const TimePoint& tp)
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        tp.time_since_epoch()
    ).count();
}

TimePoint fromEpoch(int64_t v)
{
    return TimePoint{ std::chrono::seconds(v) };
}

}

SQLiteStorage::SQLiteStorage(const std::string& dbPath)
{
    open(dbPath);
    initSchema();
}

SQLiteStorage::~SQLiteStorage()
{
    if(mDb) {
        sqlite3_close(static_cast<sqlite3*>(mDb));
    }
}

void SQLiteStorage::open(const std::string& path)
{
    if(sqlite3_open(path.c_str(), reinterpret_cast<sqlite3**>(&mDb)) != SQLITE_OK) {
        throw std::runtime_error("Failed to open SQLite DB");
    }
}

void SQLiteStorage::initSchema()
{
    exec(
        "CREATE TABLE IF NOT EXISTS bars ("
        "symbol TEXT NOT NULL,"
        "timeframe INTEGER NOT NULL,"
        "ts INTEGER NOT NULL,"
        "open REAL,"
        "high REAL,"
        "low REAL,"
        "close REAL,"
        "volume REAL,"
        "PRIMARY KEY(symbol, timeframe, ts)"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_bars_symbol_tf_ts "
        "ON bars(symbol, timeframe, ts);"
    );
}

void SQLiteStorage::exec(const std::string& sql) const
{
    char* err = nullptr;
    if(sqlite3_exec(static_cast<sqlite3*>(mDb), sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err ? err : "";
        sqlite3_free(err);
        throw std::runtime_error(msg);
    }
}

void SQLiteStorage::store(const MarketSeries& series)
{
    auto* db = static_cast<sqlite3*>(mDb);

    sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);

    const char* sql =
        "INSERT OR REPLACE INTO bars "
        "(symbol,timeframe,ts,open,high,low,close,volume)"
        "VALUES (?,?,?,?,?,?,?,?);";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    for(const auto& bar : series.bars) {
        sqlite3_bind_text(stmt, 1, series.symbol.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, static_cast<int>(series.timeframe));
        sqlite3_bind_int64(stmt, 3, toEpoch(bar.ts));
        sqlite3_bind_double(stmt, 4, bar.open);
        sqlite3_bind_double(stmt, 5, bar.high);
        sqlite3_bind_double(stmt, 6, bar.low);
        sqlite3_bind_double(stmt, 7, bar.close);
        sqlite3_bind_double(stmt, 8, bar.volume);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}

MarketSeries SQLiteStorage::load(
    const std::string& symbol,
    Timeframe timeframe
) const
{
    MarketSeries out;
    out.symbol = symbol;
    out.timeframe = timeframe;

    const char* sql =
        "SELECT ts,open,high,low,close,volume "
        "FROM bars WHERE symbol=? AND timeframe=? "
        "ORDER BY ts ASC;";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(mDb), sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, static_cast<int>(timeframe));

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        Bar b;
        b.ts     = fromEpoch(sqlite3_column_int64(stmt, 0));
        b.open   = sqlite3_column_double(stmt, 1);
        b.high   = sqlite3_column_double(stmt, 2);
        b.low    = sqlite3_column_double(stmt, 3);
        b.close  = sqlite3_column_double(stmt, 4);
        b.volume = sqlite3_column_double(stmt, 5);

        out.bars.push_back(b);
    }

    sqlite3_finalize(stmt);
    return out;
}

std::optional<TimePoint> SQLiteStorage::getLastTimestamp(
    const std::string& symbol,
    Timeframe timeframe
) const
{
    const char* sql =
        "SELECT MAX(ts) FROM bars WHERE symbol=? AND timeframe=?;";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(static_cast<sqlite3*>(mDb), sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, static_cast<int>(timeframe));

    std::optional<TimePoint> out;

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        if(sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
            out = fromEpoch(sqlite3_column_int64(stmt, 0));
        }
    }

    sqlite3_finalize(stmt);
    return out;
}

void SQLiteStorage::insertBar(const std::string& symbol,
                   Timeframe tf,
                   const Bar& bar)
{
    static constexpr const char* sql =
        "INSERT OR IGNORE INTO bars "
        "(symbol, timeframe, ts, open, high, low, close, volume) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(static_cast<sqlite3*>(mDb), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare insertBar statement");
    }

    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, static_cast<int>(tf));
    sqlite3_bind_int64(stmt, 3, bar.ts.time_since_epoch().count());
    sqlite3_bind_double(stmt, 4, bar.open);
    sqlite3_bind_double(stmt, 5, bar.high);
    sqlite3_bind_double(stmt, 6, bar.low);
    sqlite3_bind_double(stmt, 7, bar.close);
    sqlite3_bind_double(stmt, 8, bar.volume);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute insertBar");
    }

    sqlite3_finalize(stmt);
}

size_t SQLiteStorage::countBars(const std::string& symbol, Timeframe tf) const
{
    static constexpr const char* sql =
        "SELECT COUNT(*) "
        "FROM bars "
        "WHERE symbol = ? AND timeframe = ?;";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(static_cast<sqlite3*>(mDb), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare countBars statement");
    }

    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, static_cast<int>(tf));

    size_t count = 0;

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = static_cast<size_t>(sqlite3_column_int64(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return count;
}
