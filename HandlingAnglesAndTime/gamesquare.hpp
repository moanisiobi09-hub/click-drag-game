// A gamesquare is a clickable and dragable square as well as its "shadow" underneath (the square the user is trying to overlap). This
// square will manage drawing on screen shapes and calculating point outputs. It will not keep track of the overall score however.
#ifndef GAMESQUARE_HPP
#define GAMESQUARE_HPP
#include <SFML/Graphics.hpp>

class Knob;

// the background outlined square
class Shadow : public sf::Drawable, public sf::Transformable {
    private:
        const float SIDE_LEN = 200;
        sf::RectangleShape create() const; // draw the black outlined square
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        Shadow(); // default constructor must change some fields
        void randomRotate(); // give the Shadow a random new orientation
};

// the central colored square and all of its knobs
class Gamesquare : public sf::Drawable, public sf::Transformable {
    private:
        const float SIDE_LEN = 200;
        // topLeft, topRight, bottomRight, bottomLeft 
        Knob& knob1;
        Knob& knob2;
        Knob& knob3;
        Knob& knob4;
        sf::Color currentColor = sf::Color(110, 219, 57); // the current color of the central square
        sf::Clock turnClock; // store the time since a turn has begun
        sf::RectangleShape create() const; // specify the square that composed this Gamesquare
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        Gamesquare(Knob &knob1, Knob &knob2, Knob &knob3, Knob &knob4); // constructor must get knobs that it owns (topLeft, topRight, bottomRight, bottomLeft)
        void rotateKnobs(sf::Angle angle); // move the four knobs to be oriented to the Gamesquare
        bool nearAngle(sf::Angle angle) const; // return if the Gamesquare is rotated within the margin of error of the shadow
        int calculatePoints() const; // calculate and return the number of points the user got this turn
        void newTurn(); // begin a new turn, reset necessary values, output spash text
        void startClock(); // start the gamesquare clock
};

// a knob that occupies a position of the Gamesquare
class Knob : public sf::Drawable, public sf::Transformable {
    private:
        const float RADIUS = 10;
        sf::Color FILL_COLOR = sf::Color(114, 147, 173), BORDER_COLOR = sf::Color(58, 77, 130);
        sf::Vector2f P; // that starting position of this knob during a click and drag
        const sf::Vector2f center = {400, 300}; // the center of the Gamesquare from which local rotations are calculate
        bool isDragging = false; // store whether or not the knob is being dragged
        bool isOpaque = true; // store whether or not the knob is opaque
        sf::CircleShape create() const; // specify a circle
        sf::Vector2f findD(sf::Vector2i mPos) const; // find the closest point on the circle this knob is bound to to the mouse
        sf::Vector2f findPClockwise90() const; // find the point that is a 90 degree rotation of P
        sf::Angle findTheta(sf::Vector2f point1, sf::Vector2f point2) const; // find the angle formed between two points on a circle centered at this->center
        void rotateGS(Gamesquare &gs, sf::Angle angle); // rotate the Gamesquare. it will then rotate all four knobs
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    public:
        sf::Vector2f drawnAt; // the position that the circle is drawn at
        void init(sf::Vector2f offset); // set the origin and where to draw the knob of this knob
        bool getIsDragging() const; // get the value of isDragging
        void setIsDragging(bool newIsDragging); // change the value of isDragging
        bool isUnder(sf::Vector2i mPos) const; // return if the mouse is on this knob
        void setIsOpaque(bool newIsOpaque); // toggle the opacity of this knob (255 or 127)
        void setP(sf::Vector2f newP); // change the value of P
        void turn(Gamesquare &gs, sf::Vector2i mPos); // turn everything on screen in accordance with the mouse position
        void getRotated(sf::Angle angle); // rotate this knob entity and rotate the drawnAt point
};
#endif