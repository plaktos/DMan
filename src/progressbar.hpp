#ifndef H_PROGRESSBAR
#define H_PROGRESSBAR

#include <curl/curl.h>
#include <stdio.h>
#include <cstring>

typedef int (*curl_progress_f_proto)(void*,curl_off_t, curl_off_t,
                curl_off_t, curl_off_t);

class ProgressBar{
    public:
        ProgressBar(CURL* cp, const char *const fn,
                int timepinterval = {5}, curl_progress_f_proto pf = 0)
        : m_t_lastrun(), m_pcurl(cp), m_progfunc(pf),
        m_filename(fn), m_timeprintinterval(timepinterval),
        f_dltotal(), f_dlnow()
        {  
            if(!pf)
                m_progfunc = &default_prog_func;
            memset(dltotal_dtype,0,255);
            memset(dlnow_dtype,0,255);
        }

        curl_progress_f_proto bar_display_func(){ return m_progfunc; }

    private:
        // prevent copying
        ProgressBar(const ProgressBar& other);
        ProgressBar(ProgressBar&& other);
        ProgressBar&
            operator=(const ProgressBar& other);

        // Default progressbar; can be switched out by passing a 
        // function ptr to the constructor
        static int default_prog_func(void* clientp,
                curl_off_t dltotal, curl_off_t dlnow,
                curl_off_t ultotal, curl_off_t ulnow){
            ;
            ProgressBar* pb = (ProgressBar*)clientp;
            curl_off_t currtime;
            curl_easy_getinfo(pb->m_pcurl, CURLINFO_TOTAL_TIME_T, currtime);

            if((currtime - pb->m_t_lastrun) >= pb->m_timeprintinterval){
                pb->m_t_lastrun = currtime;
                printf("Total Time: %f \n", currtime);
            }

            std::strcpy(pb->dlnow_dtype,"B"); std::strcpy(pb->dltotal_dtype,"B");
            pb->f_dltotal = dltotal;
            if(pb->f_dltotal > 1024){pb->f_dltotal /= 1024; std::strcpy(pb->dltotal_dtype,"KB");
                if(pb->f_dltotal > 1024){pb->f_dltotal /= 1024; std::strcpy(pb->dltotal_dtype,"MB");}
            }
            pb->f_dlnow = dlnow;
            if(pb->f_dlnow > 1024){ pb->f_dlnow /= 1024; std::strcpy(pb->dlnow_dtype,"KB");
                if(pb->f_dlnow > 1024){pb->f_dlnow /= 1024; std::strcpy(pb->dlnow_dtype,"MB");}
            }

            printf("\33[2K\r[%f %s / %f %s] %s",
                    pb->f_dlnow, pb->dlnow_dtype,
                    pb->f_dltotal, pb->dltotal_dtype,
                    pb->m_filename.c_str());
            fflush(stdout);
            return 0;
        }

        curl_off_t m_t_lastrun;
        CURL* m_pcurl;
        curl_progress_f_proto m_progfunc;

        std::string m_filename;
        int m_timeprintinterval;

        char dltotal_dtype[255];
        char dlnow_dtype[255];
        double f_dltotal;
        double f_dlnow;
};

#endif // H_PROGRESSBAR
