#include "Game.h"
#include <cstdlib>

int main() {
    Game g(std::getenv("TWO_CONFIG_PATH"));
    g.run();
}