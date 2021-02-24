#include "robot.h"

// robot.h implementation using WeBots

#define TIME_STEP 64
#define VELOCITY 3.0

#include <malloc.h>

#include <webots/motor.h>
#include <webots/robot.h>
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
    SERVO_COUNT,
};

struct robot {
    WbDeviceTag motors[MOTOR_COUNT];
    WbDeviceTag servos[SERVO_COUNT];
    WbDeviceTag ds[DS_COUNT];        // Distance sensors
    double distances[DS_COUNT];       // Distances last measured by the sensors
};

struct robot* robot_init(void) {
    struct robot* robot = malloc(sizeof(struct robot));
    if (robot == NULL) {
        perror("Robot struct allocation failed");
        return NULL;
    }

    // Initialize WeBots and get device tags
    wb_robot_init();

    robot->servos[SERVO_WHEEL_1] = wb_robot_get_device("wheel1servo");
    robot->servos[SERVO_WHEEL_2] = wb_robot_get_device("wheel2servo");
    robot->servos[SERVO_WHEEL_5] = wb_robot_get_device("wheel5servo");
    robot->servos[SERVO_WHEEL_6] = wb_robot_get_device("wheel6servo");

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

void robot_terminate(struct robot* robot) {
    printf("WeBots Robot terminated\n");
    free(robot);
    wb_robot_cleanup();
}

int robot_update(struct robot* robot, double x, double y) {
    int ret = wb_robot_step(TIME_STEP);
    if (ret == -1) {
        return -1;
    }

    // Measure distances
    for (int i = 0; i < DS_COUNT; ++i) {
        robot->distances[i] = wb_distance_sensor_get_value(robot->ds[i]);
    }

    if (fabs(x) < 0.001 && fabs(y) < 0.001) {
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
    else if (fabs(x) > 0.001 && fabs(y) < 0.001) {
        // Rotate without moving
        wb_motor_set_position(robot->servos[SERVO_WHEEL_1], +0.87);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_2], -0.87);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_5], -0.87);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_6], +0.87);
        wb_motor_set_available_torque(robot->motors[SERVO_WHEEL_3], 0.0);
        wb_motor_set_available_torque(robot->motors[SERVO_WHEEL_4], 0.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_1], -x * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_2], +x * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_3], -x * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_4], +x * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_5], -x * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_6], +x * VELOCITY);
    }
    else if (fabs(x) < 0.001 && fabs(y) > 0.001) {
        // Go straight forward/backwards
        wb_motor_set_position(robot->servos[SERVO_WHEEL_1], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_2], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_5], 0.0);
        wb_motor_set_position(robot->servos[SERVO_WHEEL_6], 0.0);
        wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_3], 2.0);
        wb_motor_set_available_torque(robot->motors[MOTOR_WHEEL_4], 2.0);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_1], y * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_2], y * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_3], y * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_4], y * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_5], y * VELOCITY);
        wb_motor_set_velocity(robot->motors[MOTOR_WHEEL_6], y * VELOCITY);
    }
    else {
        // Mixed
        perror("Unsupported input, only axis aligned movement is supported");
    }

    return 0;
}

double* robot_get_distances(struct robot* robot) {
    return robot->distances;
}
