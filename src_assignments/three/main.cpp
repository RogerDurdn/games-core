#include <iostream>
#include "GameEngine.h"

int main() {
    std::cout << "Not Mario!" << std::endl;
    GameEngine g = GameEngine(std::getenv("MEGA_ASSETS_PATH"));
    g.run();
    return 0;
}
