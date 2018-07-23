#include "downman.hpp"


int main(int argc, char** argv){
    DownMan dm(argc, argv);
    if(static_cast<int>(dm.status())){
        std::cerr << dm.error_msg();
        return 1;
    }
    dm.start();
    return 0;
}
