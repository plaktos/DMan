#define TEST

#include "../include/catch.hpp"
#include "../src/httpsession.hpp"
#include "../src/httpcsession.hpp"

TEST_CASE("DSession http download", "[download]"){
    const char* url = "https://github.com/plaktos/DMan";
    HTTPSession s(url);
    REQUIRE(s.start());
}

TEST_CASE("DSession txt download", "[download]"){
    const char* url = "https://raw.githubusercontent.com/plaktos/DMan/master/res_test/download_test.txt";
    HTTPSession s(url);
    REQUIRE(s.start());

}

TEST_CASE("DSession continuable download start", "[download_cont]"){
    const char* url = "https://github.com/plaktos/DMan/raw/master/res_test/10mb_random";
    HTTPSession s(url);
    REQUIRE(s.start());
}
