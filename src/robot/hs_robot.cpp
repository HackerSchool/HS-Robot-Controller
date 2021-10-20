#include "hs_robot.hpp"

using namespace hs::robot;

HSRobot::HSRobot()
{
    // TODO
}

HSRobot::~HSRobot()
{
    // TODO
}

void HSRobot::setMovement(double rotation, double translation)
{
    // TODO
}

void HSRobot::setArmPosition(double x, double y, double z)
{
    // TODO
}

void HSRobot::setCameraAngle(double angle)
{
    // TODO
}

double HSRobot::readSensor(Sensor sensor) const
{
    return 0.0; // TODO
}

cv::Mat HSRobot::readCamera() const
{
    return cv::Mat(); // TODO
}

bool HSRobot::shouldStop() const
{
    return true; // TODO
}

void HSRobot::update()
{
    // TODO
}
