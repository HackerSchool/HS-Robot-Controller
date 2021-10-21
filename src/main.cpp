#include <iostream>
#include <opencv2/opencv.hpp>

#include "robot/webots_robot.hpp"

#include "behaviour/manager.hpp"
#include "behaviour/manual.hpp"
#include "behaviour/line_follower.hpp"

#include "app/webots_app.hpp"
#include "app/hs_app.hpp"

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
    App* app = new HSApp(*robot, *manager);

    // Register behaviour types
    manager->registerType<Manual>(std::ref(*app));
    manager->registerType<LineFollower>();

    // Robot update loop
    while (!robot->shouldStop())
    {
        robot->update();
        app->update();
        manager->update();

#ifdef SHOW_OPENCV_WINDOWS
        cv::waitKey(1); // Handle OpenCV events, required for showing OpenCV windows
#endif // SHOW_OPENCV_WINDOWS
    }

    delete app;
    delete manager;
    delete robot;
    return 0;
}