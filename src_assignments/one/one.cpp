#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <fstream>
#include <utility>

class Wrapper {
    std::unique_ptr<sf::Text> innerText;
    std::shared_ptr<sf::Shape> innerShape;
    sf::Vector2f speed;

    static bool xCollision(const sf::FloatRect bound, float width){
        return bound.left < 0 || bound.left+bound.width > width;
    }

    static bool yCollision(const sf::FloatRect bound, float height){
        return bound.top < 0 || bound.top+bound.height > height;
    }

public:
    Wrapper(const sf::Text &text, std::shared_ptr<sf::Shape> shape, float sx, float sy) :
            innerText(std::make_unique<sf::Text>(text)),
            innerShape(std::move(shape)),
            speed(sf::Vector2f(sx, sy)){
    }

    sf::Shape  &refreshShape(sf::Vector2u windowSize){
        auto shapeBound = this->innerShape->getGlobalBounds();
        std::cout << "lf:"<<shapeBound.left<<" tp:"<<shapeBound.top<<"\n";
        std::cout << "h:"<<shapeBound.height<<" w:"<<shapeBound.width<<"\n";
        if(Wrapper::xCollision(shapeBound, windowSize.x)){
            this->speed.x *= -1;
        }
        if(Wrapper::yCollision(shapeBound, windowSize.y)){
            this->speed.y *= -1;
        }
        this->innerShape->setPosition(
                innerShape->getPosition().x+this->speed.x,
                innerShape->getPosition().y+this->speed.y);
        return *this->innerShape;
    }

    sf::Text & refreshShapeText(){
        sf::Text *text = this->innerText.get();
        sf::Vector2f shapePos = this->innerShape->getPosition();
        auto shapeBound = this->innerShape->getLocalBounds();
        auto bound = text->getLocalBounds();
        float newX = shapePos.x+((shapeBound.width-bound.width)/2);
        float newY = shapePos.y+((shapeBound.height-bound.height)/2) - text->getCharacterSize()/2;
        text->setPosition(newX, newY );
        return *text;
    }
};

class FontInner {
    std::shared_ptr<sf::Font> font;
    int size, r, g, b;
public:
    FontInner(const sf::Font& font, int size, int r, int g, int b)
            : font(std::make_shared<sf::Font>(font)),
              size(size), r(r), g(g), b(b) {
    }
    sf::Text textOf(const std::string &msg){
        auto text =  sf::Text(msg, *this->font, this->size);
        text.setFillColor(sf::Color(r,g,b));
        return text;
    }
};

class ItemsBuilder {
    std::unique_ptr<sf::RenderWindow> renderWindow;
    std::shared_ptr<FontInner> fontInner;
    std::vector<Wrapper*> shapes;

    void buildWindow(std::ifstream &fileIn){
        int window_w, window_h;
        fileIn >> window_w >> window_h;
        this->renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(window_w, window_h), "SFML ");
        this->renderWindow->setFramerateLimit(60);
    }

    void buildFont(std::ifstream &fileIn){
        std::string fontPath;
        int fontSize, r,g,b;
        fileIn >> fontPath >> fontSize >> r >> g >> b;
        sf::Font fontLoaded;
        if (!fontLoaded.loadFromFile(std::getenv(fontPath.c_str()))) {
            std::cerr << "Could not load font" << std::endl;
            exit(-1);
        }
        this->fontInner = std::make_unique<FontInner>( fontLoaded, fontSize, r, g, b);
    }

    void buildCircle(std::ifstream &fileIn){
        std::string name;
        int r, g, b;
        float x, y,sx, sy, radius;
        fileIn >> name >> x >> y >> sx >> sy >> r >> g >> b >> radius;

        auto text = this->fontInner->textOf(name);
        auto circle = sf::CircleShape(radius);
        circle.setPosition(x, y);
        circle.setFillColor(sf::Color(r,g,b));
        this->shapes.push_back(new Wrapper(text, std::make_shared<sf::CircleShape>(circle), sx, sy));
    }

    void buildRectangle(std::ifstream &fileIn){
        std::string name;
        int r, g, b;
        float x, y, sx, sy, w, h;
        fileIn >> name >> x >> y >> sx >> sy >> r >> g >> b >> w >> h;

        auto text = this->fontInner->textOf(name);
        auto rectangle = sf::RectangleShape(sf::Vector2f(w, h));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color(r,g,b));
        this->shapes.push_back(new Wrapper(text, std::make_shared<sf::RectangleShape>(rectangle), sx, sy));
    }

public:
    void buildItemsFrom(const std::string &path) {
        std::ifstream fileIn(path);
        if(!fileIn.is_open()){
            std::cerr <<"error getting resource:"<<path <<std::endl;
            std::exit(-1);
        }
        std::string type;
        while (fileIn >> type) {
            if (type == "Window") {
                buildWindow(fileIn);
            }
            if (type == "Font") {
                buildFont(fileIn);
            }
            if (type == "Circle") {
                buildCircle(fileIn);
            }
            if (type == "Rectangle") {
                buildRectangle(fileIn);
            }
        }
    }
    std::unique_ptr<sf::RenderWindow> getWindow(){
        return std::move(this->renderWindow);
    }
    std::vector<Wrapper*> getShapes(){
        return this->shapes;
    }
};


int main(){
    auto builder = std::make_unique<ItemsBuilder>();
    builder->buildItemsFrom(std::getenv("ONE_CONFIG"));
    auto window = builder->getWindow();
    auto shapesWrapper = builder->getShapes();
    while(window->isOpen()){
        sf::Event event{};
        while(window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        window->clear();
        for (auto &wrapper: shapesWrapper) {
            window->draw(wrapper->refreshShape(window->getSize()));
            window->draw(wrapper->refreshShapeText());
        }
        window->display();
    }
    return 0;
}
