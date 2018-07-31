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
            get_filename(url);
            m_lastwritten = find_last_written_byte();
            m_progressbar = std::make_unique<ProgressBar>(m_curl, m_filename.c_str(), m_lastwritten);

            curl_easy_setopt(m_curl, CURLOPT_URL, url);
            //-TODO option to show progress or not
            curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);

            curl_easy_setopt(m_curl,
                    m_lastwritten > 2147483648 ?
                        CURLOPT_RESUME_FROM_LARGE : CURLOPT_RESUME_FROM,
                    m_lastwritten);

            curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA,
                    m_progressbar.get());
            curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, m_progressbar->bar_display_func());
        }

        bool start() override {
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, init_file(m_inprogfilename.c_str()));
            if(!curl_easy_perform(m_curl)){
                // If download succeeded rename file from .downloading
                //-TODO have a .dman folder keep track of downloaded files
                rename(m_inprogfilename.c_str(), m_filename.c_str());
                return true;
            }
            //-TODO return status codes. so we can log
            //and handle partially downloaded files properly
            return false;
        }


    private:

        FILE* init_file(const char* fn) override {
            m_file = fopen(fn, "ab");
            return m_file;
        }

        const char* get_filename(const char* url) override {
            //-TODO request filename
            const char* cs;
            while(*url++ && *(url+1))
                if(*url == '/')
                    cs = url;
            cs++;
            if(*cs){
                m_filename = cs;
                if(m_filename.back() == '/')
                    m_filename.pop_back();
                m_inprogfilename = m_filename;
                m_inprogfilename.append(".downloading");
                return m_filename.c_str();
            }
            return nullptr;
        }

        long find_last_written_byte(){
            std::unique_ptr<FILE, decltype(&fclose)> f(fopen(m_inprogfilename.c_str(), "r"), &fclose);
            if(!f.get())
                return 0L;

            fseek(f.get(), 0L, SEEK_END);  
            long ret = ftell(f.get());
            
            return ret;
        }

        std::unique_ptr<ProgressBar> m_progressbar;
        std::string m_inprogfilename;
        long m_lastwritten;
};
#endif // H_HTTPCSESSION
