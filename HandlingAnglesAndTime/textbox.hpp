// Store the data for an animated floating texbox (derived from sf::Text) with a velocity (sf::Vector2f) and textcolor (sf::Color). The
// textbox will fade away over the course of its lifetime and be destroyed. Created in main.
#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <SFML/Graphics.hpp>

// a textbox that generates and despawns over the course of its lifetime
class Textbox : public sf::Drawable, public sf::Transformable {
    private:
        sf::Font* font; // the font of this textbox
        sf::Vector2i velocity; // speed in a direction (you know it)
        sf::Color color; // the color of the text
        float lifeTime; // the total time (in seconds) this textbox will exist
        sf::Clock lifeClock; // the time since this textbox was born
        bool killMe = false; // whether or not to run this objects desctructor
        std::string message; // the text on this textbox
        void setColorAndMessage(int score); // specify the color and message
        sf::Text create() const; // specify the textbox to be drawn on screen
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        Textbox(sf::Font &font, sf::Vector2i spawnPoint, int score); // the constructor will define the fields randomly
        void updateData(); // update all the fields in accordance with the remaining life
        bool getKillMe() const; // return killMe
};

#endif