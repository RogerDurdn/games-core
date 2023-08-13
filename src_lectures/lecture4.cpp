#include <iostream>
#include <SFML/Graphics.hpp>


sf::Font fontLoad(const std::string &name) {
    sf::Font fontLoad;
    if (!fontLoad.loadFromFile(name)) {
        std::cerr << "Could not load font" << std::endl;
        exit(-1);
    }
    return fontLoad;
}

int main() {
    const int window_W = 480;
    const int window_H = 640;

    // window
    sf::RenderWindow window(sf::VideoMode(window_W, window_H), "SFML ");
    window.setFramerateLimit(60);

    //shapes
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Cyan);
    circle.setPosition(300.0f, 300.0f);
    auto speed = 0.6f;

    sf::Vector2f rectangleSize(100, 50);
    sf::RectangleShape rectangle(rectangleSize);
    rectangle.setPosition(20, 5);
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(10);

    auto fontTech = fontLoad("../assets/Roboto-Thin.ttf");
    auto textToSet = sf::Text("Some text", fontTech, 22);
    textToSet.setPosition(0, window_H - (float) textToSet.getCharacterSize());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) { // keep going until window is closed
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                std::cout << "key pressed:" << event.key.code << std::endl;
                auto keyPressed = event.key;
                if (keyPressed.code == sf::Keyboard::X) {
                    speed *= -1.0f; // reverse the direction of the shape
                }
            }
        }
        auto currentPosition = circle.getPosition();
        circle.setPosition(currentPosition.x - speed, currentPosition.y - speed);

        rectangle.rotate(0.1);

        window.clear();
        window.draw(circle);
        window.draw(textToSet);
        window.draw(rectangle);
        window.display();
    }
    return 0;
}