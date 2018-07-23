#ifndef H_DOWNMAN
#define H_DOWNMAN

#include <iostream>
#include <string>
#include <deque>
#include <curl/curl.h>

#include "cliparser.hpp"
#include "dsession.hpp"

class DownMan{
    public:
        enum class StatusCode{
            OKAY, ERROR
        };

        DownMan(const int argc, const char* const * const argv);
        ~DownMan(){ curl_global_cleanup(); }

        std::deque<char*>& url_list() { return m_urllist; }
        StatusCode status(){ return m_statuscode; }
        const std::string& error_msg(){ return m_errorstring; }

        void start();

    private:
        StatusCode m_statuscode;
        std::string m_errorstring;

        std::deque<char*> m_urllist;
};

#endif //  H_DOWNMAN
