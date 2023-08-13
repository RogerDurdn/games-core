#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Font fontLoad(const string &name) {
    Font fontLoad;
    if (!fontLoad.loadFromFile(name)) {
        cerr << "Could not load font" << endl;
        exit(-1);
    }
    return fontLoad;
}

int main() {
    const int window_W = 480;
    const int window_H = 640;

    // window
    RenderWindow window(VideoMode(window_W, window_H), "SFML ");
    window.setFramerateLimit(60);

    //shapes
    CircleShape circle(50);
    circle.setFillColor(Color::Cyan);
    circle.setPosition(300.0f, 300.0f);
    auto speed = 0.6f;

    Vector2f rectangleSize(100, 50);
    RectangleShape rectangle(rectangleSize);
    rectangle.setPosition(20, 5);
    rectangle.setFillColor(Color::Blue);
    rectangle.setOutlineColor(Color::Red);
    rectangle.setOutlineThickness(10);

    auto fontTech = fontLoad("../assets/Roboto-Thin.ttf");
    auto textToSet = Text("Some text", fontTech, 22);
    textToSet.setPosition(0, window_H - (float) textToSet.getCharacterSize());

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) { // keep going until window is closed
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                cout << "key pressed:" << event.key.code << endl;
                auto keyPressed = event.key;
                if (keyPressed.code == Keyboard::X) {
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