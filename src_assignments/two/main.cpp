#include "Game.h"
#include <cstdlib>

int main() {
    Game g(std::getenv("CONFIG_PATH"));
    g.run();
}