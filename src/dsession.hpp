#ifndef H_DSESSION
#define H_DSESSION

#include <curl/curl.h>
#include <iostream>

#include "progressbar.hpp"

class DSession{
    public:
        enum class SessionType{
            http, http_cont
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
        // prevent copying
        DSession(const DSession& other);
        DSession(DSession&& other);
        DSession& operator=(const DSession& other);


        void init_curl(const char *const url, SessionType t){
            m_curl = curl_easy_init();
            std::cout << curl_version() << std::endl;
            curl_easy_setopt(m_curl, CURLOPT_URL, url);
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
#ifdef TEST /////////////////////////////////////////////
            curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
#endif      /////////////////////////////////////////////

            if(t == SessionType::http || t == SessionType::http_cont){
                curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_http);
                curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, ProgressBar(m_curl).dfunc());

                if(t == SessionType::http_cont){
                    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_http_cont);
                    //-TODO ProgressBar to represent cointinued file
                }
            }
        }

        FILE* init_file(const char* fn){
            if(m_sessiontype == SessionType::http){
                m_file = fopen(m_filename.c_str(), "wb");
            }
            else if(m_sessiontype == SessionType::http_cont){
                m_file = fopen(m_filename.c_str(), "a+b");
            }

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

        const char* get_filename_http_cont(const char* url){
            get_filename_http(url);
            m_filename.append(".downloading");
            return m_filename.c_str();
        }

        long find_last_written_byte(){
            fseek(m_file, 0L, SEEK_END);  
            int last_written = ftell(m_file);
            rewind(m_file);
            return last_written;
        }

        bool start(SessionType t){
            if(t == SessionType::http)
                curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(get_filename_http(m_url)));

            else if(t == SessionType::http_cont){
                curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(get_filename_http_cont(m_url)));
                long last_written = find_last_written_byte();
                // if file is larger than 2gb use RESOME_FROM_LARGE
                curl_easy_setopt(m_curl,
                        last_written > 2147483648 ? CURLOPT_RESUME_FROM_LARGE : CURLOPT_RESUME_FROM,
                        last_written);
            }
            if(!curl_easy_perform(m_curl))
                if(t == SessionType::http_cont)
                    rename(m_filename.c_str(), m_filename.substr(0,
                                m_filename.find_last_of('.')).c_str());
                return true;
            return false;
        }

        static size_t
            write_http(void *ptr, size_t size, size_t nmemb, void* stream){
                return fwrite(ptr, size, nmemb, (FILE *)stream);
            }

        static size_t
            write_http_cont(void *ptr, size_t size, size_t nmemb, void* stream){
                return fwrite(ptr, size, nmemb, (FILE *)stream);
            }

        const char *const m_url;
        std::string m_filename;
        SessionType m_sessiontype;
        CURL* m_curl;
        FILE* m_file;
};
#endif // H_DSESSION
