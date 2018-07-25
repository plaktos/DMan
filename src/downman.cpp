#include "downman.hpp"

DownMan::DownMan(const int argc,const char* const * const argv)
: m_statuscode(StatusCode::OKAY),
    m_errorstring(),
    m_urllist(CliParser<std::deque<char*>>()(argc,argv)) {
    if(m_urllist.empty()){
        m_statuscode = StatusCode::ERROR;
        m_errorstring = "CliParser couldn't create url list";
    }
}

void
DownMan::start(){
    std::deque<char*>::const_iterator iter = m_urllist.begin();
    for(;iter != m_urllist.end(); ++iter){
        //-TODO support for different protocols based on cli arguments
        if(!HTTPSession(*iter).start()){
            m_statuscode = StatusCode::ERROR;
            m_errorstring = "DSession error";
            break;
        }
    }
}
