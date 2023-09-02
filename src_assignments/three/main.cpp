#include <iostream>
#include "GameEngine.h"

int main() {
    std::cout << "Not Mario!" << std::endl;
    GameEngine g = GameEngine(std::getenv("THREE_CONFIG_PATH"));
    g.run();
    return 0;
}
