// A gamesquare is a clickable and dragable square as well as its "shadow" underneath (the square the user is trying to overlap). This
// square will manage drawing on screen shapes and calculating point outputs. It will not keep track of the overall score however.
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include "gamesquare.hpp"
#include <iostream>

sf::RectangleShape Gamesquare::create() const {
    // create a square, set its fields
    sf::RectangleShape square({SIDE_LEN, SIDE_LEN});
    square.setFillColor(currentColor);
    sf::Color borderColor = currentColor;
    borderColor.a /= 1.3;
    borderColor.g /= 1.3;
    borderColor.b /= 1.3;
    square.setOutlineColor(borderColor);
    square.setOutlineThickness(4);
    return square;
}

void Gamesquare::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // get the transform
    states.transform = getTransform();
    // create and draw the central square
    sf::RectangleShape square = create();
    target.draw(square, states);
}

Gamesquare::Gamesquare(Knob &knob1, Knob &knob2, Knob &knob3, Knob &knob4) : knob1(knob1), knob2(knob2), knob3(knob3), knob4(knob4) {
    // STOP THE CLOCK
    turnClock.stop();
    // set the origin of the gamesquare to the center of the square
    setOrigin({SIDE_LEN / 2, SIDE_LEN / 2});
}

void Gamesquare::rotateKnobs(sf::Angle angle) {
    knob1.getRotated(angle);
    knob2.getRotated(angle);
    knob3.getRotated(angle);
    knob4.getRotated(angle);
}

bool Gamesquare::nearAngle(sf::Angle angle) const {
    const int ERROR_MARGIN = 5; // the number of degrees off the user can be to score
    return abs(((int)getRotation().asDegrees() % 90) - ((int)angle.asDegrees() % 90)) <= ERROR_MARGIN;
}

int Gamesquare::calculatePoints() const {
    const int ONE_POINT_TIME = 15; // every time this quantity of time elapses, the user loses one point
    return 100 - ((int)(turnClock.getElapsedTime().asMilliseconds() - 500) / ONE_POINT_TIME);
}

void Gamesquare::newTurn() {
    // stop and start the clock
    turnClock.reset();
    // change the color of the tile
    int r = rand() % 256, g = rand() % 256, b = rand() % 256;
    currentColor = sf::Color(r, g, b);
    turnClock.start();
}

void Gamesquare::startClock() {
    turnClock.start();
}
////////////////////////////////////////////////////////////////////////////////////////////////
sf::CircleShape Knob::create() const {
    sf::CircleShape knob;
    // specify the knob's fields
    knob.setRadius(RADIUS);
    knob.setOrigin(knob.getGeometricCenter());
    // if the knob isn't opaque make it half alpha
    if (!isOpaque || isDragging) {
        sf::Color tempColor(FILL_COLOR);
        sf::Color tempBColor(BORDER_COLOR);
        tempColor.a = 127;
        tempBColor.a = 127;
        knob.setFillColor(tempColor);
        knob.setOutlineColor(tempBColor);
    }
    else {
        knob.setFillColor(FILL_COLOR);
        knob.setOutlineColor(BORDER_COLOR);
    }
    knob.setOutlineThickness(3);
    return knob;
}

void Knob::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // get the knob's transform
    states.transform = getTransform();
    // specify the knob and draw
    sf::CircleShape knob = create();
    target.draw(knob, states);
}

void Knob::init(sf::Vector2f offset) {
    setOrigin(offset);
    setPosition(center);
    drawnAt = {getPosition().x - offset.x, getPosition().y - offset.y};
}

bool Knob::getIsDragging() const {
    return isDragging;
}

void Knob::setIsDragging(bool newIsDragging) {
    isDragging = newIsDragging;
}

bool Knob::isUnder(sf::Vector2i mPos) const {
    return (mPos.x > drawnAt.x - RADIUS && mPos.x < drawnAt.x + RADIUS) && (mPos.y > drawnAt.y - RADIUS && mPos.y < drawnAt.y + RADIUS);
}

void Knob::setIsOpaque(bool newIsOpaque) {
    isOpaque = newIsOpaque;
}

void Knob::setP(sf::Vector2f newP) {
    P = newP;
}

float distance(sf::Vector2f point1, sf::Vector2f point2) {
    return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
}

sf::Vector2f Knob::findD(sf::Vector2i mPos) const {
    const float SCREEN_LEN = 800;
    float z = (float)mPos.x, n = (float)mPos.y;
    float h = center.x, k = center.y;
    // the radius of the circle this knob is bound to
    float radius = distance(drawnAt, center);
    // find the X and Y of both points on the line passing through
    // (h, k) and (z, n) that intersect circle (h, k)
    float posA = h + (radius / sqrt(pow((n - k) / (z - h), 2) + 1));
    float posB = ((n - k) / (z - h)) * (posA - h) + k;
    sf::Vector2f D1 = {posA, posB};
    // get the negative versions
    float negA = h + (radius / (-1 * sqrt(pow((n - k) / (z - h), 2) + 1)));
    float negB = ((n - k) / (z - h)) * (negA - h) + k;
    sf::Vector2f D2 = {negA, negB};
    // choose the version closer to the mouse
    sf::Vector2f D;
    if (mPos.x > SCREEN_LEN / 2) D = D1;
    else D = D2;
    return D;
}

sf::Vector2f Knob::findPClockwise90() const {
    // find the point in a coordinate grid centered at the center
    sf::Vector2f localP = {P.x - center.x, (-1 * P.y) + center.y};
    // perform a 90 degree rotation (clockwise)
    sf::Vector2f localN = {localP.y, -1 * localP.x};
    // convert N to screen coordinates
    sf::Vector2f N = {localN.x + center.x, -localN.y + center.y};
    return N;
}

sf::Angle Knob::findTheta(sf::Vector2f point1, sf::Vector2f point2) const {
    // the radius of the circle this knob is bound to
    float radius = distance(drawnAt, center);
    sf::Angle theta = sf::radians(std::acos((pow(distance(point2, point1), 2) - 2 * pow(radius, 2)) / (-2 * pow(radius, 2))));
    return theta;
}

void Knob::rotateGS(Gamesquare &gs, sf::Angle angle) {
    gs.rotate(angle);
    gs.rotateKnobs(angle);
}

void Knob::turn(Gamesquare &gs, sf::Vector2i mPos) {
    // the rotation value is undefined when the mouse is over the center
    if (mPos.x != center.x) {
        // find the point that is a 90 clockwise rotation of P
        sf::Vector2f N = findPClockwise90();
        // find the closest point on the imaginary circle to the mouse
        sf::Vector2f D = findD(mPos);
        // find the angle between P and D
        sf::Angle theta = findTheta(D, P);
        // find the angle between N and D
        sf::Angle gamma = findTheta(N, D);
        // rotate Gamesquare by theta, with the correct sign
        if (gamma.asDegrees() > 90) rotateGS(gs, -theta);
        else rotateGS(gs, theta);
        // change P so theta never reaches (or "exceeds") 180 degrees
        P = D;
    }
}

void Knob::getRotated(sf::Angle angle) {
    rotate(angle);
    sf::Transform transform;
    transform.rotate(angle, center);
    drawnAt = transform.transformPoint(drawnAt);
}

////////////////////////////////////////////////////////////////////////////////////////////////
sf::RectangleShape Shadow::create() const {
    // create a square, set its data
    sf::RectangleShape square({SIDE_LEN, SIDE_LEN});
    square.setFillColor(sf::Color::Black);
    square.setOutlineColor(sf::Color::White);
    square.setOutlineThickness(2);
    return square;
}

void Shadow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // apply the transform
    states.transform = getTransform();
    // get the body square and draw
    sf::RectangleShape square = create();
    target.draw(square, states);
}

Shadow::Shadow() {
    // set the center of the gamesquare to the center of the square
    setOrigin({SIDE_LEN / 2, SIDE_LEN / 2});
}

void Shadow::randomRotate() {
    const int NECESSARY_OFFSET = 30;
    // the new angle will be at least NECESSARY_OFFSET degrees away from the last one
    int newAngle = newAngle = (rand() % (91 - 2 * NECESSARY_OFFSET)) + NECESSARY_OFFSET;
    // except occasionally
    if (rand() % 5 == 4) newAngle /= 3;
    rotate(sf::degrees(newAngle));
}