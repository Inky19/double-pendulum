#ifndef PEND_HPP
#define PEND_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Constants
const float g = 9.81;

// Classes
class pendulum : public sf::Transformable{
private:
    class segment{
        public:
            segment();
            segment(float mass);
            sf::CircleShape circle;
            float m;
            float agl;
            float aVel;
            float aAcc;
            float l;
    };

    segment seg1;
    segment seg2;
    bool move;
    bool hold;

    segment* selected;
    sf::Vertex anchor;
    sf::Texture texture;

public:
    pendulum(float m1, float m2);
    void render(sf::RenderWindow &window);
    void update(int dt, sf::Vector2i mouse);
    void clicked(sf::Vector2i mouse);
    void released();
    void stop();
    void pause();
    void restart(int width);
    std::string info();
};

#endif
