#include "manual.hpp"

using namespace hs;
using namespace hs::behaviour;

Manual::Manual(robot::Robot& robot, const app::App& app) : Behaviour(robot), app(app)
{
}

void Manual::update()
{
    this->robot.setMovement(app.getJoystickX(), app.getJoystickY());
    this->robot.setCameraAngle(app.getCameraAngle());
}
