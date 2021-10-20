#include <iostream>

#include "robot/webots_robot.hpp"

#include "behaviour/manager.hpp"
#include "behaviour/manual.hpp"
#include "behaviour/line_follower.hpp"

#include "app/webots_app.hpp"

using namespace hs;
using namespace hs::robot;
using namespace hs::behaviour;
using namespace hs::app;

int main()
{
    // Setup robot
    Robot* robot = new WebotsRobot();

    // Create behaviour manager
    Manager* manager = new Manager(*robot);

    // Setup app
    App* app = new WebotsApp(*robot, *manager);

    // Register behaviour types
    manager->registerType<Manual>(std::ref(*app));
    manager->registerType<LineFollower>();

    // Robot update loop
    while (!robot->shouldStop())
    {
        robot->update();
        app->update();
        manager->update();
    }

    delete app;
    delete manager;
    delete robot;
    return 0;
}