#define TEST

#include "../include/catch.hpp"
#include "../src/dsession.hpp"

TEST_CASE("DSession http download", "[download]"){
    const char* url = "https://github.com/plaktos/DMan";
    DSession::SessionType t = DSession::SessionType::http;
    DSession s(url, t);
    REQUIRE(s.start());
}

TEST_CASE("DSession txt download", "[download]"){
    const char* url = "https://raw.githubusercontent.com/plaktos/DMan/master/res_test/download_test.txt";
    DSession::SessionType t = DSession::SessionType::http;
    DSession s(url, t);
    REQUIRE(s.start());

}

TEST_CASE("DSession continuable download start", "[download_cont]"){
    const char* url = "https://github.com/plaktos/DMan/raw/master/res_test/10mb_random";
    DSession::SessionType t = DSession::SessionType::http_cont;
    DSession s(url, t);
    REQUIRE(s.start());
}
