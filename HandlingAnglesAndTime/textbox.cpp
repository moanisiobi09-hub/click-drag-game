// Store the data for an animated floating textbox (derived from sf::Text) with a velocity (sf::Vector2f) and textcolor (sf::Color). The
// textbox will fade away over the course of its lifetime and be destroyed. Created in main.
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <format>
#include "textbox.hpp"
#include <iostream>

void Textbox::setColorAndMessage(int score) {
    std::vector<std::vector<std::string>> splashTexts = {
        {"through in the towel lil' bro", "time to quit", "yeesh!", "pack it up, bud", "bad pull", "DAMN, you suck!", "not your day", "GIVE UP!", "listen to the haters!", "who is this nigga?"},
        {"okay, I guess", "nice", "lil' cool", "good", "average", "hurry up", "you're trying...", "neat", "alright", "moderate", "there's potential", "you'll get there"},
        {"go you", "keep it up", "cool stuff", "kudos", "great", "nice work", "get it", "hit that", "nice to see"},
        {"that's the stuff", "work that!", "you're getting it", "fast fingers!", "reaction time", "cool ash", "cool asl", "great job"},
        {"you're cooking!", "incredible!", "good shit", "dunk after dunk", "no misses", "pro level", "elite baller", "D1 level", "master of the craft", "that's what i'm talking about!"},
        {"fucking incredible", "perfect", "masterpiece", "THEY DON'T KNOW YOU SON", "the best there ever was", "holy shit", "amazing", "I need to glaze this nigga", "absolute cinema", "Mr. President, they hit the second tower."}
    };
    // set the color and choose the message based on the score
    if (score < 0) {
        message = splashTexts[0][rand() % splashTexts[0].size()];
        color = sf::Color(212, 0, 0);
    }
    else if (score < 10) {
        message = splashTexts[1][rand() % splashTexts[1].size()];
        color = sf::Color(235, 176, 5);
    }
    else if (score < 30) {
        message = splashTexts[2][rand() % splashTexts[2].size()];
        color = sf::Color(182, 235, 15);
    }
    else if (score < 60) {
        message = splashTexts[3][rand() % splashTexts[3].size()];
        color = sf::Color(90, 194, 16);
    }
    else if (score < 90) {
        message = splashTexts[4][rand() % splashTexts[4].size()];
        color = sf::Color(103, 232, 12);
    }
    else {
        message = splashTexts[5][rand() % splashTexts[5].size()];
        color = sf::Color(37, 255, 0);
    }
    message += std::format(": {}", score);
}

sf::Text Textbox::create() const {
    sf::Text text(*font, message);
    text.setCharacterSize(25);
    text.setFillColor(color);
    return text;
}

void Textbox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // get the transform
    states.transform = getTransform();
    // create and draw the textbox to screen
    sf::Text textbox = create();
    target.draw(textbox, states);
}

Textbox::Textbox(sf::Font &font, sf::Vector2i spawnPoint, int score) : font(&font) {
    // set the position to the spawnPoint
    setPosition({(float)spawnPoint.x, (float)spawnPoint.y});
    // set the velocity [-3, 3]
    velocity = {(rand() % 7) - 3, (rand() % 7)  - 3};
    // set the life time [1.0, 2.0]
    lifeTime = ((rand() % 11) + 10) / 10;
    // set the color and message
    setColorAndMessage(score);
}

void Textbox::updateData() {
    float lifeLeft = lifeTime - lifeClock.getElapsedTime().asSeconds();
    // move according to the velocity
    move({(float)velocity.x, (float)velocity.y});
    // change the opacity
    color.a = (uint8_t)(255 / (lifeTime / lifeLeft));
    // signal to destroy this textbox at the end of its life
    if (lifeLeft <= 0) killMe = true;
}

bool Textbox::getKillMe() const {
    return killMe;
}