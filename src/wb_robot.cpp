#include "robot.hpp"

// robot.hpp implementation for WeBots

#define TIME_STEP 64
#define VELOCITY 10.0

#include <malloc.h>
#include <cmath>

#include <webots/robot.h>
#include <webots/camera.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

enum {
    MOTOR_WHEEL_1,
    MOTOR_WHEEL_2,
    MOTOR_WHEEL_3,
    MOTOR_WHEEL_4,
    MOTOR_WHEEL_5,
    MOTOR_WHEEL_6,
    MOTOR_COUNT
};

enum {
    SERVO_WHEEL_1,
    SERVO_WHEEL_2,
    SERVO_WHEEL_3,
    SERVO_WHEEL_4,
    SERVO_WHEEL_5,
    SERVO_WHEEL_6,
    SERVO_CAMARA,
    SERVO_COUNT,
};

struct Robot {
    WbDeviceTag camera;
    WbDeviceTag motors[MOTOR_COUNT];
    WbDeviceTag servos[SERVO_COUNT];
    WbDeviceTag ds[DS_COUNT];        // Distance sensors
    double distances[DS_COUNT];       // Distances last measured by the sensors
};

Robot* robot_init() {
    Robot* robot = new Robot();

    // Initialize WeBots and get device tags
    wb_robot_init();

    robot->camera = wb_robot_get_device("camera");

    robot->servos[SERVO_WHEEL_1] = wb_robot_get_device("wheel1servo");
    robot->servos[SERVO_WHEEL_2] = wb_robot_get_device("wheel2servo");
    robot->servos[SERVO_WHEEL_5] = wb_robot_get_device("wheel5servo");
    robot->servos[SERVO_WHEEL_6] = wb_robot_get_device("wheel6servo");
    robot->servos[SERVO_CAMARA] = wb_robot_get_device("SERVO_CAMARA");

    robot->motors[MOTOR_WHEEL_1] = wb_robot_get_device("wheel1");
    robot->motors[MOTOR_WHEEL_2] = wb_robot_get_device("wheel2");
    robot->motors[MOTOR_WHEEL_3] = wb_robot_get_device("wheel3");
    robot->motors[MOTOR_WHEEL_4] = wb_robot_get_device("wheel4");
    robot->motors[MOTOR_WHEEL_5] = wb_robot_get_device("wheel5");
    robot->motors[MOTOR_WHEEL_6] = wb_robot_get_device("wheel6");

    robot->ds[DS_LEFT] = wb_robot_get_device("ds_left");
    robot->ds[DS_LEFT_FRONT] = wb_robot_get_device("ds_left_front");
    robot->ds[DS_RIGHT] = wb_robot_get_device("ds_right");
    robot->ds[DS_RIGHT_FRONT] = wb_robot_get_device("ds_right_front");

    // Enable camera
    wb_camera_enable(robot->camera, TIME_STEP);

    // Initialize motors
    for (int i = 0; i < MOTOR_COUNT; ++i) {
        wb_motor_set_position(robot->motors[i], INFINITY);
    }

    // Enable distance sensors
    wb_distance_sensor_enable(robot->ds[DS_LEFT], TIME_STEP);
    wb_distance_sensor_enable(robot->ds[DS_LEFT_FRONT], TIME_STEP);
    wb_distance_sensor_enable(robot->ds[DS_RIGHT], TIME_STEP);
    wb_distance_sensor_enable(robot->ds[DS_RIGHT_FRONT], TIME_STEP);

    printf("WeBots Robot initialized\n");

    return robot;
}

void robot_terminate(Robot* robot) {
    printf("WeBots Robot terminated\n");
    delete robot;
    wb_robot_cleanup();
}

int robot_update(Robot* robot, double x, double y, double z) {
    int ret = wb_robot_step(TIME_STEP);
    if (ret == -1) {
        return -1;
    }

    // Measure distances
    for (int i = 0; i < DS_COUNT; ++i) {
        robot->distances[i] = wb_distance_sensor_get_value(robot->ds[i]);
    }

    // Get movement vector
    double speed = sqrt(x * x + y * y);
    if (speed < 0.05) {
        // Stop robot
        wb_motor_set_position(robot->servos[SERVO_WHEEL_1], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_2], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_5], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_6], 0.0);
        wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_3], 2.0);
        wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_4], 2.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_1], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_2], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_3], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_4], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_5], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_6], 0.0);
    }
    else {
        // Normalize movement vector
        x /= speed;
        y /= speed;
        double x_sign = x > 0.0 ? 1.0 : (x < 0.0 ? -1.0 : 0.0);
        double y_sign = y > 0.0 ? 1.0 : (y < 0.0 ? -1.0 : 0.0);

        // Rotate only
        if (y < 0.2) {
            // Rotate without moving
            wb_motor_set_position(robot->servos[SERVO_WHEEL_1], +0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_2], -0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_5], -0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_6], +0.87);
            wb_motor_set_available_torque(robot->motors[SERVO_WHEEL_3], 0.0);
            wb_motor_set_available_torque(robot->motors[SERVO_WHEEL_4], 0.0);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_1], -speed * x_sign * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_2], +speed * x_sign * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_3], -speed * x_sign * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_4], +speed * x_sign * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_5], -speed * x_sign * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_6], +speed * x_sign * VELOCITY);
        }
        // Rotate and move at the same time
        else {
            // Set wheel angles
            wb_motor_set_position(robot->servos[SERVO_WHEEL_1], -x * 0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_2], -x * 0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_5], x * 0.87);
            wb_motor_set_position(robot->servos[SERVO_WHEEL_6], x * 0.87);

            // Set speed
            wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_3], 0.0);
            wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_4], 0.0);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_1], speed * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_2], speed * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_3], speed * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_4], speed * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_5], speed * VELOCITY);
            wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_6], speed * VELOCITY);
        }
    }

    if (fabs(z) == 0.001) {
        //Rotate camera to left
        wb_motor_set_position(robot->servos[SERVO_CAMARA], +0.87);
    }
    else if (fabs(z) == 0) {
        //Rotate camera to center
        wb_motor_set_position(robot->servos[SERVO_CAMARA], 0.0);
    }
    else if (fabs(z) == 0.002) {
        //Rotate camera to right
        wb_motor_set_position(robot->servos[SERVO_CAMARA], -0.87);
    }

    return 0;
}

double* robot_get_distances(Robot* robot) {
    return robot->distances;
}

cv::Mat robot_get_image(Robot* robot) {
    int width = wb_camera_get_width(robot->camera);
    int height = wb_camera_get_height(robot->camera);
    const unsigned char* buf = wb_camera_get_image(robot->camera);
    return cv::Mat(height, width, CV_8UC4, (unsigned char*)buf);
}
