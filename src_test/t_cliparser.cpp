#define TEST

#include "../include/catch.hpp"
#include "../src/cliparser.hpp"
#include <vector>

TEST_CASE("CliParser parsing", "[parsing]") {
    const char* argv[4];
    const int argc = 4;
    argv[1] = "arg1";
    argv[2] = "arg2";
    argv[3] = "arg3";

    CliParser<std::vector<char*>> cp;
    std::vector<char*> v(cp.parse(argc,argv));

    REQUIRE( v[0] == "arg1" );
    REQUIRE( v[1] == "arg2" );
    REQUIRE( v[2] == "arg3" );
}

