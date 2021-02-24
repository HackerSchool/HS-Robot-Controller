#include "robot.hpp"
#include "joystick.hpp"

#include <opencv2/opencv.hpp>
#include <cstdio>

int main(int argc, char** argv) {
    Robot* robot = robot_init();
    if (robot == nullptr) {
        perror("robot_init() failed");
        return 1;
    }

    Joystick* joystick = joystick_init();
    if (joystick == nullptr) {
        perror("joystick_init() failed");
        return 2;
    }

    double move_x = 0.0, move_y = 0.0;
    while (robot_update(robot, move_x, move_y) != -1) {
        // Show camera image
        cv::Mat img = robot_get_image(robot);
        cv::imshow("Camera", img);

        // Update movement using joystick
        joystick_read(joystick, &move_x, &move_y);

        // Handles OpenCV events
        cv::waitKey(1);
    }

    cv::destroyAllWindows();
    robot_terminate(robot);
    return 0;
}
