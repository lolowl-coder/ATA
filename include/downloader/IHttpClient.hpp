#pragma once

#include <string>

class IHttpClient {
public:
    virtual ~IHttpClient() = default;

    virtual std::string get(const std::string& url) const = 0;
};
