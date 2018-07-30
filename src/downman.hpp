#ifndef H_DOWNMAN
#define H_DOWNMAN

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <curl/curl.h>

#include "cliparser.hpp"
#include "httpsession.hpp"
#include "httpcsession.hpp"

class DownMan{
    public:
        enum class StatusCode{
            OKAY, ERROR
        };

        DownMan(const int argc, const char* const * const argv);
        ~DownMan(){ curl_global_cleanup(); }

        const std::vector<char*>& url_list(){return m_urllist;}
        StatusCode status(){ return m_statuscode; }
        const std::string& error_msg(){ return m_errorstring; }

        void start();

    private:
        StatusCode m_statuscode;
        std::string m_errorstring;

        CliParser<std::vector<char*>> m_cliparser;
        std::vector<char*> m_urllist;
};

#endif //  H_DOWNMAN
