#include <iostream>
#include "GameEngine.h"

int main() {
    std::cout << "Not Mario!" << std::endl;
    GameEngine g = GameEngine(std::getenv("MEGA_ASSETS_PATH"));
    try {
        g.run();
    } catch (std::exception &e) {
        std::cout << "Error on:" << e.what() << std::endl;
        exit(-1);
    }
    return 0;
}
