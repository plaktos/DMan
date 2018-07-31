#ifndef H_PROGRESSBAR
#define H_PROGRESSBAR

#include <curl/curl.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <mutex>

class ProgressBar{
    public:
    using curl_progress_f_proto = int (*)(void*,curl_off_t, curl_off_t,
                curl_off_t, curl_off_t);

        ProgressBar(CURL* cp, const char *const fn, double dloffset = 0, int timepinterval = 5, curl_progress_f_proto pf = 0)
        : m_t_lastrun(0), m_pcurl(cp), 
        m_filename(fn), m_timeprintinterval(timepinterval),
        f_dltotal(0), f_dlnow(0), m_dloffset(dloffset),
        m_instance(instances)
        {  
            if(!pf)
                m_progfunc = &default_prog_func;
            memset(dltotal_dtype,0,255);
            memset(dlnow_dtype,0,255);
            ++instances;
        }

        ~ProgressBar(){ printf("\n"); --instances; }

        curl_progress_f_proto bar_display_func(){ return m_progfunc; }

        // prevent copying
        ProgressBar(const ProgressBar& other) = delete;
        ProgressBar(ProgressBar&& other) = delete;
        ProgressBar& operator=(const ProgressBar& other) = delete;

        static int instances;
    private:
        // Default progressbar; can be switched out by passing a 
        // function ptr to the constructor
        static int default_prog_func(void* clientp,
                curl_off_t dltotal, curl_off_t dlnow,
                curl_off_t ultotal, curl_off_t ulnow){
            ;
            ProgressBar* pb = (ProgressBar*)clientp;
            std::strcpy(pb->dlnow_dtype,"B"); std::strcpy(pb->dltotal_dtype,"B");
            pb->f_dltotal = dltotal + pb->m_dloffset;
            if(pb->f_dltotal > 1024){pb->f_dltotal /= 1024; std::strcpy(pb->dltotal_dtype,"KB");
                if(pb->f_dltotal > 1024){pb->f_dltotal /= 1024; std::strcpy(pb->dltotal_dtype,"MB");}
            }
            pb->f_dlnow = dlnow + pb->m_dloffset;
            if(pb->f_dlnow > 1024){ pb->f_dlnow /= 1024; std::strcpy(pb->dlnow_dtype,"KB");
                if(pb->f_dlnow > 1024){pb->f_dlnow /= 1024; std::strcpy(pb->dlnow_dtype,"MB");}
            }

            std::lock_guard<std::mutex> lg(printlock);
            printf("\033[s\033[%iB\33[2K\r[%f %s / %f %s] %s\033[u",
                    pb->m_instance,
                    pb->f_dlnow, pb->dlnow_dtype,
                    pb->f_dltotal, pb->dltotal_dtype,
                    pb->m_filename.c_str());
            fflush(stdout);
            return 0;
        }

        static std::mutex printlock;
        curl_off_t m_t_lastrun;
        CURL* m_pcurl;
        curl_progress_f_proto m_progfunc;

        std::string m_filename;
        int m_timeprintinterval;

        char dltotal_dtype[255];
        char dlnow_dtype[255];
        double f_dltotal;
        double f_dlnow;
        double m_dloffset;
        int m_instance;
};

#endif // H_PROGRESSBAR
