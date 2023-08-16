#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main(){
    std::cout <<"hello\n";
    Game g("config.txt");

    g.run();
}