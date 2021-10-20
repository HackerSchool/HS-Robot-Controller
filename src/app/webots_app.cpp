#include "webots_app.hpp"

#include "../behaviour/manual.hpp"
#include "../behaviour/line_follower.hpp"

using namespace hs::app;

#ifdef USE_WEBOTS

#include <webots/keyboard.h>

WebotsApp::WebotsApp(const robot::Robot& robot, behaviour::Manager& manager, int timeStep) : App(robot, manager)
{
    this->joystickX = 0.0;
    this->joystickY = 0.0;
    this->cameraAngle = 0.0;
    this->timeStep = timeStep;

    wb_keyboard_enable(this->timeStep);
}

double WebotsApp::getJoystickX() const
{
    return this->joystickX;
}

double WebotsApp::getJoystickY() const
{
    return this->joystickY;
}

double WebotsApp::getCameraAngle() const
{
    return this->cameraAngle;
}

void WebotsApp::update()
{
    // Get pressed key
    int key = wb_keyboard_get_key();

    // Handle key input
    switch (key)
    {
    case 'A': // Rotate left
        this->joystickX = -1.0;
        this->joystickY = 0.0;
        break;
    case 'D': // Rotate right
        this->joystickX = +1.0;
        this->joystickY = 0.0;
        break;
    case 'W': // Move forward
        this->joystickX = 0.0;
        this->joystickY = +1.0;
        break;
    case 'S': // Move backwards
        this->joystickX = 0.0;
        this->joystickY = -1.0;
        break;

    case 'P': // Stop the robot
        this->joystickX = 0.0;
        this->joystickY = 0.0;
        break;

    // Camera angle controls
    case 'U':
        this->cameraAngle = 0.001;
        break;
    case 'I':
        this->cameraAngle = 0.0;
        break;
    case 'O':
        this->cameraAngle = 0.002;
        break;

    // Change modes
    case 'J':
        this->manager.set<behaviour::Manual>();
        this->joystickX = 0.0;
        this->joystickY = 0.0;
        break;
    case 'L':
        this->manager.set<behaviour::LineFollower>();
        break;
    }
}

#else

WebotsApp::WebotsApp(const robot::Robot& robot, behaviour::Manager& manager, int timeStep) : App(robot, manager)
{
    abort(); // Unsupported operation
}

double WebotsApp::getJoystickX() const
{
    abort(); // Unsupported operation
}

double WebotsApp::getJoystickY() const
{
    abort(); // Unsupported operation
}

double WebotsApp::getCameraAngle() const
{
    abort(); // Unsupported operation
}

void WebotsApp::update()
{
    abort(); // Unsupported operation
}

#endif // USE_WEBOTS