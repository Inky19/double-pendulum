#include "pend.h"

int main(){
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Double pendulum");
    pendulum pend(10.f, 10.f);
    sf::Clock clock;
    sf::Text debugTxt;
    debugTxt.setString("FPS : ");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")){
        return 1;
    }
    debugTxt.setFont(font);
    debugTxt.setCharacterSize(15);
    debugTxt.setFillColor(sf::Color::White);
    bool debug = true;
    long double timer = 0.f;
    int frames = 0;
    int frate = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    pend.clicked(sf::Mouse::getPosition(window));
                    break;
                case sf::Event::MouseButtonReleased:
                    pend.released();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::F3){
                        debug = !debug;
                    } else if (event.key.code == sf::Keyboard::Space){
                        pend.pause();
                    } else if (event.key.code == sf::Keyboard::LControl){
                        pend.moveAnchor(true);
                    }
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::LControl){
                        pend.moveAnchor(false);
                    }
                    break;
                case sf::Event::MouseWheelScrolled:
                    pend.chgMass(event.mouseWheelScroll.delta);
                    break;
                case sf::Event::Resized:
                    pend.stop();
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    pend.restart(event.size.width);
                    break;
            }                
        }
    sf::Time time = clock.getElapsedTime();
    int dt = time.asMicroseconds();
    clock.restart().asMicroseconds();
    pend.update(dt, sf::Mouse::getPosition(window));
    timer += dt/1000000.f;
    frames++;
    if (timer >= 0.5){
        frate = frames*2;
        timer = 0;
        frames = 0;
    }
    debugTxt.setString("[PRESS F3 TO HIDE]\nFPS : " + std::to_string(frate) + pend.info());
    window.clear();
    pend.render(window);
    if (debug){
        window.draw(debugTxt);
    }
    window.display();
    }
    return 0;
}