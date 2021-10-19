#include <iostream>

#include "robot/webots_robot.hpp"
#include "behaviour/manager.hpp"

using namespace hs;
using namespace hs::robot;
using namespace hs::behaviour;

int main()
{
    Robot* robot = new WebotsRobot();
    Manager* manager = new Manager(*robot);

    while (!robot->shouldStop())
    {
        robot->update();
        manager->update();
    }

    delete manager;
    delete robot;
    return 0;
}