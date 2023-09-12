#include <iostream>
#include "GameEngine.h"

int main() {
    GameEngine g = GameEngine(std::getenv("NOTLINK_ASSETS_PATH"));
    try{
        g.run();
    }catch (const std::exception& e){
        std::cout << "Error at:" << e.what() << std::endl;
        exit(-1);
    }
    return 0;
}
