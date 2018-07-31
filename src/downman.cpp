#include "downman.hpp"

int ProgressBar::instances = 0;
std::mutex ProgressBar::printlock;

DownMan::DownMan(const int argc,const char* const * const argv)
: m_statuscode(StatusCode::OKAY), m_errorstring(),
    m_cliparser()  {
           m_urllist = m_cliparser.parse(argc,argv);
           init_sessions();
}

void
DownMan::init_sessions(){
    for(const auto& url : m_urllist){
        if(m_cliparser.flag_continue()){
            m_sessions.push_back(std::make_unique<HTTPCSession>(url));
        } else {
            m_sessions.push_back(std::make_unique<HTTPSession>(url));
        }
    }
}

void
DownMan::start(){
    std::vector<std::thread> threads;

    for(auto& s : m_sessions){
        if(m_cliparser.flag_threads()){
            threads.push_back(std::thread([&s](){
                s.get()->start();
            }));
        }
        else{
            s.get()->start();
        }
    }
    for(auto& t : threads){
        t.join();
    }
}
