#include "pend.h"
#include <cmath>

pendulum::segment::segment(){
    this->m = 0;
    this->circle.setRadius(10);
    this->agl = 1.f;
    this->aVel = 0;
    this->aAcc = 0;
    this->l = 300.f;
}

pendulum::segment::segment(float mass){
    this->m = mass;
    this->circle.setRadius(mass);
    this->agl = 1.f;
    this->aVel = 0;
    this->aAcc = 0;
    this->l = 300.f;
}

pendulum::pendulum(float m1, float m2){
    this->seg1 = segment(m1);
    this->seg2 = segment(m2);
    this->anchor.position = sf::Vector2f(500.f, 50.f);

    this->move = true;
    this->hold = false;
}

// Render the pendulum
void pendulum::render(sf::RenderWindow &window){
    sf::Vector2f center1 = seg1.circle.getPosition();
    center1.x += seg1.m;
    center1.y += seg1.m;
    sf::Vector2f center2 = seg2.circle.getPosition();
    center2.x += seg2.m;
    center2.y += seg2.m;
    sf::Vertex verticesL1[2] = { anchor, center1};
    sf::Vertex verticesL2[2] = { center1, center2};
    window.draw(verticesL1, 2, sf::Lines);
    window.draw(verticesL2, 2, sf::Lines);
    window.draw(seg1.circle);
    window.draw(seg2.circle);
}

// Update the coordinates of the pendulum
void pendulum::update(int dt, sf::Vector2i mouse){
    if (move){
        long double deltaT = dt/1000000.f;
        float l1r = seg1.l/100;
        float l2r = seg2.l/100;

        float den = 2*seg1.m+seg2.m-seg2.m*cos(2*seg1.agl-2*seg2.agl);
        float n1 = -g*(2*seg1.m+seg2.m)*sin(seg1.agl);
        float n2 = -seg2.m*g*sin(seg1.agl-2*seg2.agl);
        float n3 = -2*sin(seg1.agl-seg2.agl)*seg2.m*(pow(seg2.aVel,2)*l2r+pow(seg1.aVel,2)*l1r*cos(seg1.agl-seg2.agl));
        seg1.aAcc = (n1+n2+n3)/(l1r*den);

        n1 = pow(seg1.aVel,2)*l1r*(seg1.m+seg2.m);
        n2 = g*(seg1.m+seg2.m)*cos(seg1.agl);
        n3 = pow(seg2.aVel,2)*l2r*seg2.m*cos(seg1.agl-seg2.agl);
        seg2.aAcc = (2*sin(seg1.agl-seg2.agl)*(n1+n2+n3)/(l2r*den));

        seg1.aVel += seg1.aAcc*deltaT;
        seg1.agl += seg1.aVel*deltaT;
        seg2.aVel += seg2.aAcc*deltaT;
        seg2.agl += seg2.aVel*deltaT;
        float x1 = anchor.position.x + seg1.l*sin(seg1.agl);
        float y1 = anchor.position.y + seg1.l*cos(seg1.agl);
        seg1.circle.setPosition(x1-seg1.m, y1-seg1.m);
        seg2.circle.setPosition(x1 + seg2.l*sin(seg2.agl)-seg2.m, y1 + seg2.l*cos(seg2.agl)-seg2.m);
    } else if (hold){

        sf::Vector2f corner = sf::Vector2f(mouse);
        corner.x -= (*selected).m;
        corner.y -= (*selected).m;
        (*selected).circle.setPosition(corner);

        segment *segs[2] = {&seg1, &seg2};
        segment *seg;
        float sideVert;
        float sideHor;
        sf::Vector2f center;
        sf::Vector2f prevCenter;
        for (int i=0; i<2; i++){
            seg = segs[i];
            center = (*seg).circle.getPosition();
            center.x += (*seg).m;
            center.y += (*seg).m;
            if (i<1){
                sideVert = center.y-anchor.position.y;
                sideHor = center.x-anchor.position.x;
            } else {
                prevCenter = (*segs[i-1]).circle.getPosition();
                prevCenter.x += (*segs[i-1]).m;
                prevCenter.y += (*segs[i-1]).m;
                sideVert = center.y-prevCenter.y;
                sideHor = center.x-prevCenter.x;
            }

            (*seg).l = sqrt(pow((sideHor),2) + pow((sideVert),2));
            (*seg).agl = acos(sideVert/(*seg).l);
            if (sideHor<0){
                (*seg).agl = -(*seg).agl;
            }
            (*seg).aVel = 0;
            (*seg).aAcc = 0;
        }
    }
}

// Return a string with various information of the pendulum.
std::string pendulum::info(){
    std::string data = "\nlenght1 : " + std::to_string(seg1.l/100);
    data += "    angle1 : " + std::to_string(seg1.agl) + " rad";
    data += "\nagl velocity1 : " + std::to_string(seg1.aVel) + " rad/s";
    data += "\nagl acceleration1 : " + std::to_string(seg1.aAcc) + " rad/s^2";
    data += "\nlenght2 : " + std::to_string(seg2.l/100) + "    angle2 : " + std::to_string(seg2.agl) + " rad";
    data += "\nagl velocity2 : " + std::to_string(seg2.aVel) + " rad/s";
    data += "\nagl acceleration2 : " + std::to_string(seg2.aAcc) + " rad/s^2";
    return data;
}

// User interactions :

// Stop the simulation and enable tracking of cursor.
void pendulum::clicked(sf::Vector2i mouse){
    sf::Vector2f center1 = seg1.circle.getPosition();
    center1.x +=seg1.m;
    center1.y +=seg1.m;
    sf::Vector2f center2 = seg2.circle.getPosition();
    center1.x +=seg2.m;
    center1.y +=seg2.m;
    float d1 = pow(mouse.x-center1.x,2) + pow(mouse.y-center1.y,2);
    float d2 = pow(mouse.x-center2.x,2) + pow(mouse.y-center2.y,2);
    if (d1<d2){
        selected=&seg1;
    } 
    else {
        selected=&seg2;
    }
    move = false;
    hold = true;
}

// Start the simulation and disable tracking of cursor.
void pendulum::released(){
    hold = false;
}

// Stop the simulation and enable tracking of cursor.
void pendulum::stop(){
    move = false;
}

// Pause or resume the simulation.
void pendulum::pause(){
    move = !move;
}

// Start the simulation and calculate the position of the anchor.
void pendulum::restart(int width){
    anchor.position.x = width/2;
    move = true;
}