#define TEST

#include "../include/catch.hpp"
#include "../src/dsession.hpp"

TEST_CASE("DSession http download", "[download]"){
    const char* url = ""
    DSession::SessionType t = DSession::SessionType::http;
    DSession s(url, t);
    REQUIRE(s.start());
}

TEST_CASE("DSession txt download", "[download]"){
    const char* url = ""
    DSession::SessionType t = DSession::SessionType::http;
    DSession s(url, t);
    REQUIRE(s.start());
}
