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
            m_filename = get_filename(url);
            m_progressbar = std::make_unique<ProgressBar>(m_curl, m_filename);
            curl_easy_setopt(m_curl, CURLOPT_URL, url);
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, m_progressbar.get());
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION,
                    m_progressbar->bar_display_func());
        }

        ~HTTPSession()
        {;}

        bool start(){
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(m_filename));
            if(!curl_easy_perform(m_curl))
                return true;
            return false;
        }


    private:

        FILE* init_file(const char* fn){
            m_file = fopen(m_filename, "wb");
            return m_file;
        }

        const char* get_filename(const char* url){
            //-TODO request filename
            while(*url++)
                if(*url== '/')
                    m_filename = url;
            ++m_filename;
            if(*m_filename){
                return m_filename;
            }
            return nullptr;
        }

        static size_t
            write_data(void *ptr, size_t size, size_t nmemb, void* stream){
                return fwrite(ptr, size, nmemb, (FILE *)stream);
            }

        std::unique_ptr<ProgressBar> m_progressbar;
};
#endif // H_HTTPSESSION
