#ifndef H_DSESSION
#define H_DSESSION

#include <curl/curl.h>
#include <iostream>

#include "progressbar.hpp"

class DSession{
    public:
        enum class SessionType{
            http
        };

        DSession(const char* const url, SessionType t)
            : m_url(url), m_sessiontype(t){ 
                init_curl(url, t);
        }
        ~DSession(){
            curl_easy_cleanup(m_curl);
            if(m_file) fclose(m_file);
        }

        bool start(){
            return start(m_sessiontype);
        }

    private:
        void init_curl(const char *const url, SessionType t){
            m_curl = curl_easy_init();
            curl_easy_setopt(m_curl, CURLOPT_URL, url);
#ifdef TEST
            curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
#endif 
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);

            if(t == SessionType::http){
                curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_http);
                curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, ProgressBar(m_curl).dfunc());

            }
        }

        FILE* init_file(const char* fn){
            m_file = fopen(m_filename, "wb");
            return m_file;
        }

        const char* get_filename_http(const char* url){
            const char* lastbs;
            while(*url++)
                if(*url== '/')
                    lastbs = url;
            ++lastbs;
            if(*lastbs){
                m_filename = lastbs;
                return lastbs;
            }
            return nullptr;
        }

        bool start(SessionType t){
            if(t == SessionType::http){
                curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(get_filename_http(m_url)));
                curl_easy_perform(m_curl);
                return true;
            }
            return false;
        }

        static size_t
            write_http(void *ptr, size_t size, size_t nmemb, void* stream){
                return fwrite(ptr, size, nmemb, (FILE *)stream);
            }

        const char *const m_url;
        const char* m_filename;
        SessionType m_sessiontype;
        CURL* m_curl;
        FILE* m_file;
};
#endif // H_DSESSION
