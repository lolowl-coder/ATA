#pragma once

#include "IHttpClient.hpp"

class CurlHttpClient final : public IHttpClient {
public:
    std::string get(const std::string& url) const override;
};
