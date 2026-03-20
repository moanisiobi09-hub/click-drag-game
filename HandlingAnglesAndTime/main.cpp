// Create a game in C++ with SFML. The objective will be to gain as many points as possible. This program will make use of time and angles
// to function.
#include <SFML/Graphics.hpp>
#include <format>
#include <array>
#include <vector>
#include <algorithm>
#include "gamesquare.hpp"
#include "textbox.hpp"
#include <iostream>

int main() {
    const sf::Vector2f CENTER = {400.f, 300.f};
    const int ROUND_TIME = 60;
    // create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Handling Angles and Time");
    window.setFramerateLimit(60);
    // load the font
    sf::Font font;
    if (!font.openFromFile("font/LiberationSans-Regular.ttf")) return -1;
    // create a clock and string to track time since the beginning of a round
    sf::Clock clock;
    sf::Text timeElapsedLabel(font);
    // this clock is used to track if the player has been oriented right long enough
    sf::Clock orientationClock;
    orientationClock.stop();
    // store an int for the player's total score
    int score = 0, highScore = 0;
    sf::Text scoreLabel(font);
    scoreLabel.setPosition({630.f, 0.f});
    // create four knobs
    const float SIDE_LEN = 200;
    sf::Vector2f pos = CENTER;
    std::array<Knob, 4> knobs;
    knobs[0].init({100, 100});
    knobs[1].init({-100, 100});
    knobs[2].init({100, -100});
    knobs[3].init({-100, -100});
    // create the gamesquare
    Gamesquare gamesquare(knobs[0], knobs[1], knobs[2], knobs[3]);
    gamesquare.setPosition(CENTER);
    // create the shadow
    Shadow shadow;
    shadow.setPosition(CENTER);
    // create a vector to hold all the textboxes
    std::vector<Textbox> textboxes;
    // seed the randomizer
    srand(time(NULL));
    // store if a round is waiting to happen
    bool awaitingStart = true;
    // game loop
    while (window.isOpen()) {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        // check for events
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // check for closed events
            if (event->is<sf::Event::Closed>()) window.close();
            // check for mousePressed events
            if (const sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                // start the round
                if (awaitingStart) {
                    awaitingStart = false;
                    score = 0;
                    shadow.randomRotate();
                    clock.reset();
                    clock.start();
                    gamesquare.startClock();
                }
                // check if any of the knobs were clicked
                for (Knob &knob : knobs) {
                    if (knob.isUnder(mPos)) {
                        knob.setP(knob.drawnAt);
                        knob.setIsDragging(true);
                    }
                }
            }
            // check for mouseReleased events
            if (const sf::Event::MouseButtonReleased* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                for (Knob &knob : knobs) if (knob.getIsDragging()) knob.setIsDragging(false);
            }
        }
        window.clear();
        // update the dynamic knob data
        for (Knob &knob : knobs) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                if (knob.getIsDragging()) knob.turn(gamesquare, mPos);}
            if (knob.isUnder(mPos)) knob.setIsOpaque(false);
            else knob.setIsOpaque(true);
        }
        // detect scoring some points and act on it
        if (!awaitingStart && gamesquare.nearAngle(shadow.getRotation())) {
            if (!orientationClock.isRunning()) orientationClock.start();
            else if (orientationClock.getElapsedTime().asSeconds() > 0.5) {
                int roundScore = gamesquare.calculatePoints();
                score += roundScore;
                Textbox textbox(font, mPos, roundScore);
                textboxes.push_back(textbox);
                shadow.randomRotate();
                gamesquare.newTurn();
                orientationClock.reset();
            }
        } if (!gamesquare.nearAngle(shadow.getRotation())) orientationClock.reset();
        // draw the shadow
        window.draw(shadow);
        // draw the Gamesquare
        window.draw(gamesquare);
        // draw all the knobs
        for (Knob &knob : knobs) window.draw(knob);
        // update and draw all the textboxes
        for (auto it = textboxes.begin(); it != textboxes.end();) {
            if (it->getKillMe()) it = textboxes.erase(it);
            else {
                it->updateData();
                window.draw(*it);
                ++it;
            }
        }
        // update the time and draw to the window
        if (!awaitingStart) {
            timeElapsedLabel.setString(std::format("TIME: {}", ROUND_TIME - (int)clock.getElapsedTime().asSeconds()));
            window.draw(timeElapsedLabel);
        }
        // when time runs out
        if (clock.getElapsedTime().asSeconds() > ROUND_TIME) {
            awaitingStart = true;
            highScore = std::max(score, highScore);
        }
        // update the score and draw to the window
        scoreLabel.setString(std::format("Score: {}", score));
        window.draw(scoreLabel);
        // draw a big black cover (bbc) if the round hasn't started
        if (awaitingStart){
            sf::RectangleShape rect({800, 500});
            rect.setFillColor(sf::Color::Black);
            rect.move({0, 100});
            window.draw(rect);
            sf::Text text(font, std::format("Click to start\nHigh score: {}", highScore));
            text.setPosition({330, 300});
            window.draw(text);
        } 

        window.display();
    }
}