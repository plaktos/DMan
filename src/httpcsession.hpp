#ifndef H_HTTPCSESSION
#define H_HTTPCSESSION

#include <curl/curl.h>
#include <iostream>

#include "dsession.hpp"
#include "progressbar.hpp"

class HTTPCSession
: public DSession{
    //////
    public:
        HTTPCSession(const char* url) : DSession(){
            m_filename = get_filename(url);
            curl_easy_setopt(m_curl, CURLOPT_URL, url);
            //-TODO option to show progress or not
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION,
                    ProgressBar(m_curl).bar_display_func());

            long last_written = find_last_written_byte();
            curl_easy_setopt(m_curl,
                    last_written > 2147483648 ?
                        CURLOPT_RESUME_FROM_LARGE : CURLOPT_RESUME_FROM,
                    last_written);
        }

        ~HTTPCSession()
        {;}

        bool start(){
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(m_filename));
            if(!curl_easy_perform(m_curl)){
                // If download succeeded rename file from .downloading
                //-TODO have a .dman folder keep track of downloaded files
                std::string fn(m_filename);
                rename(m_filename, fn.substr(0,
                            fn.find_last_of('.')).c_str());
                return true;
            }
            //-TODO return status codes. so we can log
            //and handle partially downloaded files properly
            return false;
        }


    private:

        FILE* init_file(const char* fn){
            m_file = fopen(m_filename, "a+b");
            return m_file;
        }

        const char* get_filename(const char* url){
            //-TODO request filename
            const char* lastbs;
            while(*url++)
                if(*url== '/')
                    lastbs = url;
            ++lastbs;
            if(*lastbs){
                m_filename = lastbs;
                std::string retfn(m_filename);
                retfn.append(".downloading");
                return retfn.c_str();
            }
            return nullptr;
        }

        static size_t
            write_data(void *ptr, size_t size, size_t nmemb, void* stream){
                return fwrite(ptr, size, nmemb, (FILE *)stream);
            }

        long find_last_written_byte(){
            fseek(m_file, 0L, SEEK_END);  
            int last_written = ftell(m_file);
            rewind(m_file);
            return last_written;
        }
};
#endif // H_HTTPCSESSION
