#include "robot.hpp"
#include "app.hpp"
#include "line.hpp"
#include "maze.hpp"

#include <opencv2/opencv.hpp>
#include <cstdio>

int main(int argc, char** argv) {
    // Connect to robot
    Robot* robot = robot_init();
    if (robot == nullptr) {
        perror("robot_init() failed");
        return 1;
    }

    // Connect to app
    App* app = app_init();
    if (app == nullptr) {
        perror("app_init() failed");
        return 2;
    }

    Maze* maze = nullptr;
    int mode = app_get_mode(app);
    if (mode == MODE_MAZE) {
        maze = maze_init();
        if (maze == nullptr) {
            perror("maze_init() failed");
            return 3;
        }
    }

    double move_x = 0.0, move_y = 0.0, move_z = 0.0;
    
    while (robot_update(robot, move_x, move_y, move_z) != -1) {
        // Get camera image and show it
        cv::Mat img = robot_get_image(robot);
        cv::imshow("Camera", img);
        
        // Get data from the app
        if (app_update(app)) {
            // Control mode changed
            if (mode == MODE_MAZE) {
                maze_terminate(maze);
            }

            mode = app_get_mode(app);

            if (mode == MODE_MAZE) {
                maze = maze_init();
                if (maze == nullptr) {
                    perror("maze_init() failed");
                    return 3;
                }
            }
        }

        switch (mode) {
        case MODE_JOYSTICK:
            // Update movement using joystick
            app_read_joystick(app, &move_x, &move_y, &move_z);
            break;

        case MODE_LINE:
            line_follow(&img, &move_x, &move_y);
            break;

        case MODE_MAZE:
            maze_follow(maze, robot_get_distances(robot), &move_x, &move_y);
            break;
        }

        // Handles OpenCV events
        cv::waitKey(1);
    }

    if (mode == MODE_MAZE) {
        maze_terminate(maze);
    }

    cv::destroyAllWindows();
    robot_terminate(robot);
    return 0;
}
