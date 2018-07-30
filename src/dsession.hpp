#ifndef H_DSESSION
#define H_DSESSION

#include <curl/curl.h>
#include <iostream>
#include <memory>

#include "progressbar.hpp"

class DSession{
    public:
        using write_data_f_proto = size_t (*)(void*,size_t,size_t,void*);

        DSession() : m_filename(0), m_curl(curl_easy_init()), m_file(0){
#ifdef TEST /////////////////////////////////////////////
            curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
#endif      /////////////////////////////////////////////
    }
        DSession(const DSession&) = delete;
        DSession(DSession&&) = delete;
        DSession& operator=(const DSession&) = delete;

        virtual ~DSession(){
            curl_easy_cleanup(m_curl);
            if(m_file) fclose(m_file);
        }

        virtual bool start() = 0;

    protected:
        virtual FILE* init_file(const char* fn) = 0;
        virtual const char* get_filename(const char* url) = 0;
        virtual size_t write_data (void*,size_t,size_t,void*) = 0;

        const char* m_filename;
        CURL* m_curl;
        FILE* m_file;
};

#endif // H_DSESSION
