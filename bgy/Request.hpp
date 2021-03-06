
#pragma once

#include <utility>
#include <string>
#include <vector>
#include "Aside.hpp"

namespace bgy {

class Client;

class Request
{
    friend class Client;
public:
    explicit Request(const std::string& _url, HttpMethod _method = GET):
        noClean(false), url(_url), method(_method), noSign(false),
        queryStringBegan(url.find('?') != url.npos)
    {}

    Request(const std::string& _url, HttpMethod _method, const StrPairList& _params):
        noClean(false), url(_url), method(_method),
        params(_params), noSign(false),
        queryStringBegan(url.find('?') != url.npos)
    {}

    Request(const std::string& _url, const StrPairList& _params, const StrPairList& _uploads):
        noClean(false), url(_url), method(POST),
        params(_params), uploads(_uploads), noSign(false),
        queryStringBegan(url.find('?') != url.npos)
    {}

    Request& addParam(const std::string& key, const std::string& value)
    {
        params.push_back(std::make_pair(key, value));
        return *this;
    }

    Request& addFile(const std::string& key, const std::string& path)
    {
        uploads.push_back(std::make_pair(key, path));
        if (method == GET)
        {
            setMethod(POST);
        }
        return *this;
    }

    Request& addHeader(const std::string& name, const std::string& value)
    {
        headers.push_back(std::make_pair(name, value));
        return *this;
    }

    Request& setPost()
    {
        return setMethod(POST);
    }

    Request& setGet()
    {
        return setMethod(GET);
    }

    Request& setMethod(HttpMethod _method)
    {
        method = _method;
        return *this;
    }

    // 请求结束后 是否 不释放CURL资源。true：不释放；false：释放。
    // 设置成不释放时 <Response>.curl 才会有效；否则 <Response>.curl == NULL。
    Request& setNoClean(bool _noClean = true)
    {
        noClean = _noClean;
        return *this;
    }

    // 设置 请求是否 不需要参数签名。true：不需要；false：需要。
    Request& setNoSign(bool _noSign = true)
    {
        noSign = _noSign;
        return *this;
    }

private:
    bool noClean;        // 请求结束后不做 curl_easy_cleanup(curl);

    std::string url;
    HttpMethod method;
    StrPairList headers, params;
    StrPairList uploads;
    bool noSign;

    std::string download;       // 下载文件保存路径

public:
    const bool queryStringBegan;
};

}
