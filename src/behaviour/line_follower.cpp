#include "line_follower.hpp"

using namespace hs;
using namespace hs::behaviour;

void LineFollower::update()
{
    // Get captured image from robot.
    auto img = this->robot.readCamera();
}
