#include <curl/curl.h>
#include <stdio.h>

typedef int (*curl_progress_f_proto)(void*,curl_off_t, curl_off_t,
                curl_off_t, curl_off_t);

class ProgressBar{
    public:
        ProgressBar(CURL* cp, int timepinterval = {5},
                curl_progress_f_proto pf= {})
        : m_t_lastrun(), m_pcurl(cp),m_progfunc(pf), m_timeprintinterval(timepinterval)
        {
            if(!pf)
                pf = (curl_progress_f_proto)&ProgressBar::default_prog_func;
        }

        curl_progress_f_proto dfunc(){ return m_progfunc; }

    private:
        // prevent copying
        ProgressBar(const ProgressBar& other);
        ProgressBar(ProgressBar&& other);
        ProgressBar&
            operator=(const ProgressBar& other);

        // Default progressbar; can be switched out by passing a 
        // function ptr to the constructor
        int default_prog_func(void* /*_*/,
                curl_off_t dltotal, curl_off_t dlnow,
                curl_off_t ultotal, curl_off_t ulnow){
            ;
            curl_off_t currtime;
            curl_easy_getinfo(m_pcurl, CURLINFO_TOTAL_TIME_T, currtime);

            if((currtime - m_t_lastrun) >= m_timeprintinterval){
                m_t_lastrun = currtime;
                printf("Total Time: %f \n", currtime);
            }

            printf(" DOWN: %" CURL_FORMAT_CURL_OFF_T
                    "of %" CURL_FORMAT_CURL_OFF_T "\n",
                    dlnow, dltotal);
            return 0;
        }

        curl_off_t m_t_lastrun;
        CURL* m_pcurl;
        curl_progress_f_proto m_progfunc;
        int m_timeprintinterval;

};
