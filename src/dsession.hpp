#ifndef H_DSESSION
#define H_DSESSION

#include <curl/curl.h>
#include <iostream>

#include "progressbar.hpp"

class DSession{
    public:
        DSession() : m_filename(0), m_curl(curl_easy_init()), m_file(0){
#ifdef TEST /////////////////////////////////////////////
            curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
#endif      /////////////////////////////////////////////
    }
        virtual ~DSession(){
            curl_easy_cleanup(m_curl);
            if(m_file) fclose(m_file);
        }

        virtual bool start() = 0;

    protected:
        virtual FILE* init_file(const char* fn) = 0;
        virtual const char* get_filename(const char* url) = 0;

        const char* m_filename;
        CURL* m_curl;
        FILE* m_file;
};

#endif // H_DSESSION
