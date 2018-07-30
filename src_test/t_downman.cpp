#define TEST

#include "../include/catch.hpp"
#include "../src/downman.hpp"

TEST_CASE("DownMan creation", "[creation]"){
    const char* argv[4];
    const int argc = 4;
    argv[1] = "arg1";
    argv[2] = "arg2";
    argv[3] = "arg3";

    DownMan dm(argc, argv);
    std::vector<char*> ul(dm.url_list());

    REQUIRE( ul[0] == "arg1" );
    REQUIRE( ul[1] == "arg2" );
    REQUIRE( ul[2] == "arg3" );
}
