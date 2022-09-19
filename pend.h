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
            float m;    // mass
            float agl;  // angle
            float aVel; // angular velocity
            float aAcc; // angluar acceleration
            float l;    // lenght
    };

    segment seg1;
    segment seg2;
    bool move;         // Play or pause the simulation.
    bool hold;         // Represent if the mouse button is pressed or not.
    bool selecAnchor;  // Selects if the cursor move the anchor or a segment.
    bool lctrlPressed; // Help switching between anchor and segement movement (mode switch only happens when the mouse button isn't pressed or when it's released).

    segment* selected; // Segement currently selected by the cursor.
    sf::Vertex anchor; // Point where the pendulum is fixed.

public:
    pendulum(float m1, float m2);
    void render(sf::RenderWindow &window);
    void update(int dt, sf::Vector2i mouse);
    void clicked(sf::Vector2i mouse);
    void released();
    void stop();
    void pause();
    void restart(int width);
    void chgMass(int dm);
    void moveAnchor(bool mvAnchor);
    std::string info();
};

#endif
