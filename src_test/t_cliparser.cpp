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

TEST_CASE("CliParser continue flag", "[parsing]") {
    const char* argv[4];
    const int argc = 4;
    CliParser<std::vector<char*>> cp;

    SECTION("-c flag at beginning"){
        argv[1] = "-c";
        argv[2] = "arg2";
        argv[3] = "arg3";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg2" );
        REQUIRE( v[1] == "arg3" );
        REQUIRE( cp.flag_continue() );
    }
    SECTION("-c flag between arguments"){
        argv[1] = "arg1";
        argv[2] = "-c";
        argv[3] = "arg3";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg1" );
        REQUIRE( v[1] == "arg3" );
        REQUIRE( cp.flag_continue() );
    }
    SECTION("-c flag at end"){
        argv[1] = "arg1";
        argv[2] = "arg2";
        argv[3] = "-c";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg1" );
        REQUIRE( v[1] == "arg2" );
        REQUIRE( cp.flag_continue() );
    }
}

TEST_CASE("CliParser thread flag", "[parsing]") {
    const char* argv[4];
    const int argc = 4;
    CliParser<std::vector<char*>> cp;

    SECTION("-t flag at beginning"){
        argv[1] = "-t";
        argv[2] = "arg2";
        argv[3] = "arg3";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg2" );
        REQUIRE( v[1] == "arg3" );
        REQUIRE( cp.flag_threads() );
    }
    SECTION("-t flag between arguments"){
        argv[1] = "arg1";
        argv[2] = "-t";
        argv[3] = "arg3";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg1" );
        REQUIRE( v[1] == "arg3" );
        REQUIRE( cp.flag_threads() );
    }
    SECTION("-t flag at end"){
        argv[1] = "arg1";
        argv[2] = "arg2";
        argv[3] = "-t";
        std::vector<char*> v(cp.parse(argc,argv));

        REQUIRE( v[0] == "arg1" );
        REQUIRE( v[1] == "arg2" );
        REQUIRE( cp.flag_threads() );
    }
}
