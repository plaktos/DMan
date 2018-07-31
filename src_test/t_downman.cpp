#define TEST

#include "../include/catch.hpp"
#include "../src/downman.hpp"

TEST_CASE("non-continuable without threads"){
    const char* argv[5];
    const int argc = 2;

    argv[0] = "dman";
    argv[1] = "https://github.com/plaktos/DMan/raw/master/res_test/download_test.txt";
    DownMan dm(argc, argv);
    dm.start();

    {
    std::unique_ptr<FILE, decltype(&fclose)> f1(fopen("download_test.txt","r"), &fclose);
    REQUIRE(f1.get());
    }
    std::remove("dowmload_test.txt");
}

TEST_CASE("continuable, without threads"){
    const char* argv[5];
    const int argc = 2;

    argv[0] = "dman";
    argv[1] = "-c";
    argv[2] = "https://github.com/plaktos/DMan/raw/master/res_test/download_test.txt";
    DownMan dm(argc, argv);
    dm.start();

    {
    std::unique_ptr<FILE, decltype(&fclose)> f1(fopen("download_test.txt","r"), &fclose);
    REQUIRE(f1.get());
    }
    std::remove("dowmload_test.txt");
}

TEST_CASE("non-continuable, with threads"){
    const char* argv[5];
    const int argc = 2;

    argv[0] = "dman";
    argv[1] = "-t";
    argv[2] = "https://github.com/plaktos/DMan/raw/master/res_test/download_test.txt";
    DownMan dm(argc, argv);
    dm.start();

    {
    std::unique_ptr<FILE, decltype(&fclose)> f1(fopen("download_test.txt","r"), &fclose);
    REQUIRE(f1.get());
    }
    std::remove("dowmload_test.txt");
}

TEST_CASE("continuable, with threads"){
    const char* argv[5];
    const int argc = 2;

    argv[0] = "dman";
    argv[1] = "-t";
    argv[2] = "-c";
    argv[3] = "https://github.com/plaktos/DMan/raw/master/res_test/download_test.txt";
    DownMan dm(argc, argv);
    dm.start();

    {
    std::unique_ptr<FILE, decltype(&fclose)> f1(fopen("download_test.txt","r"), &fclose);
    REQUIRE(f1.get());
    fseek(f1.get(), 0L, SEEK_END);  
    }
    std::remove("dowmload_test.txt");
}
