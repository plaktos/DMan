#include "downman.hpp"

DownMan::DownMan(const int argc,const char* const * const argv)
: m_statuscode(StatusCode::OKAY), m_errorstring(),
    m_cliparser()  {
           m_urllist = m_cliparser.parse(argc,argv);
}

void
DownMan::start(){
    for(const auto& url : m_urllist){
        if(m_cliparser.flag_continue()){
            HTTPCSession s(url);
            s.start();
        } else {
            HTTPSession s(url);
            s.start();
        }
    }
}
