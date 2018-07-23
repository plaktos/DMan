#define TEST

#include "../include/catch.hpp"
#include "../src/cliparser.hpp"
#include <deque>

TEST_CASE("CliParser parsing", "[parsing]") {
    const char* argv[4];
    const int argc = 4;
    argv[1] = "arg1";
    argv[2] = "arg2";
    argv[3] = "arg3";

    std::deque<char*> cp(CliParser<std::deque<char*>>(argc, argv)());

    REQUIRE( cp.front() == "arg1" );
    cp.pop_front();

    REQUIRE( cp.front() == "arg2" );
    cp.pop_front();

    REQUIRE( cp.front() == "arg3" );
}

