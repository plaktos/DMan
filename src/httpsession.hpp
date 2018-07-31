#ifndef H_HTTPSESSION
#define H_HTTPSESSION

#include <curl/curl.h>
#include <iostream>

#include "dsession.hpp"
#include "progressbar.hpp"

class HTTPSession
: public DSession{
    //////
    public:
        HTTPSession(const char* url) : DSession(){
            get_filename(url);
            m_progressbar = std::make_unique<ProgressBar>(m_curl, m_filename.c_str());
            curl_easy_setopt(m_curl, CURLOPT_URL, url);
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, m_progressbar.get());
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION,
                    m_progressbar->bar_display_func());
        }

        bool start() override {
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(m_filename.c_str()));
            if(!curl_easy_perform(m_curl))
                return true;
            return false;
        }


    private:

        FILE* init_file(const char* fn) override {
            m_file = fopen(fn, "wb");
            return m_file;
        }

        const char* get_filename(const char* url) override {
            //-TODO request filename
            const char* cs;
            while(*url++ && *(url+1))
                if(*url== '/')
                    cs = url;
            cs++;
            if(*cs){
                m_filename = cs;
                if(m_filename.back() == '/')
                    m_filename.pop_back();
                return m_filename.c_str();
            }
            return nullptr;
        }

        std::unique_ptr<ProgressBar> m_progressbar;
};
#endif // H_HTTPSESSION
