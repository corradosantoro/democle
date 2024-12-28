/*
 * url.h
 */

#ifndef __URL_HH
#define __URL_HH

#include <string>

struct url {
    url(const std::string& url_s) {
      this->parse(url_s);
    };
    std::string protocol, host, port, path, query;
private:
    void parse(const std::string& url_s);
};

#endif

