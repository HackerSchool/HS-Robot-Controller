#include "webots_robot.hpp"

#include <iostream>

using namespace hs::robot;

#ifdef USE_WEBOTS

WebotsRobot::WebotsRobot(int timeStep)
{
    this->timeStep = timeStep;
    this->rotation = 0.0;
    this->translation = 0.0;
    this->arm[0] = this->arm[1] = this->arm[2] = 0.0;
    this->cameraAngle = 0.0;
    this->shouldStopFlag = false;

    wb_robot_init();

    // Get Webots device tags for motors, servos, sensors and the camera
    for (int i = 0; i < static_cast<int>(Motor::Count); ++i)
        this->motors[i] = wb_robot_get_device(WebotsRobot::MOTOR_TO_DEVICE.at(static_cast<Motor>(i)));
    for (int i = 0; i < static_cast<int>(Servo::Count); ++i)
        this->servos[i] = wb_robot_get_device(WebotsRobot::SERVO_TO_DEVICE.at(static_cast<Servo>(i)));
    for (int i = 0; i < static_cast<int>(Sensor::Count); ++i)
        this->sensors[i] = wb_robot_get_device(WebotsRobot::SENSOR_TO_DEVICE.at(static_cast<Sensor>(i)));
    this->camera = wb_robot_get_device(WebotsRobot::CAMERA_DEVICE);

    // Initialize camera, motors and sensors
    wb_camera_enable(this->camera, this->timeStep);
    for (int i = 0; i < static_cast<int>(Motor::Count); ++i)
        wb_motor_set_position(this->motors[i], INFINITY);
    for (int i = 0; i < static_cast<int>(Sensor::Count); ++i)
        wb_distance_sensor_enable(this->sensors[i], this->timeStep);

    // Stop robot
    this->setMotor(Motor::Wheels, 0.0);
    this->setServo(Servo::Wheels, 0.0);

    std::cout << "Webots robot initialized" << std::endl;
}

WebotsRobot::~WebotsRobot()
{
    wb_robot_cleanup();

    std::cout << "Webots robot terminated" << std::endl;
}

void WebotsRobot::setMovement(double rotation, double translation)
{
    this->rotation = rotation;
    this->translation = translation;
}

void WebotsRobot::setArmPosition(double x, double y, double z)
{
    this->arm[0] = x;
    this->arm[1] = y;
    this->arm[2] = z;
}

void WebotsRobot::setCameraAngle(double angle)
{
    this->cameraAngle = angle;
}

double WebotsRobot::readSensor(Sensor sensor) const
{
    return wb_distance_sensor_get_value(this->sensors[static_cast<int>(sensor)]) /
           wb_distance_sensor_get_max_value(this->sensors[static_cast<int>(sensor)]);
}

cv::Mat WebotsRobot::readCamera() const
{
    int width = wb_camera_get_width(this->camera);
    int height = wb_camera_get_height(this->camera);
    const unsigned char* buf = wb_camera_get_image(this->camera);
    return cv::Mat(height, width, CV_8UC4, (unsigned char*)buf);
}

bool WebotsRobot::shouldStop() const
{
    return this->shouldStopFlag;
}

void WebotsRobot::update()
{
    // Length of the movement vector
    double speed = sqrt(this->rotation * this->rotation + this->translation * this->translation);

    if (speed < 0.05) // Stop the robot
    {
        // Update motors and servos
        this->setMotor(Motor::Wheels, 0.0);
        this->setServo(Servo::Wheels, 0.0);

        // For some reason the robot explodes without this
        wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelRM)], 2.0);
        wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelLM)], 2.0);
    }
    else // The robot is moving
    {
        // Normalize movement vector
        double x = rotation / speed;
        double y = translation / speed;
        double xSign = x > 0.0 ? 1.0 : (x < 0.0 ? -1.0 : 0.0);
        double ySign = y > 0.0 ? 1.0 : (y < 0.0 ? -1.0 : 0.0);

        if (y < 0.2) // Rotate only
        {
            // Update motors and servos
            this->setMotor(Motor::WheelsR, -speed * xSign);
            this->setMotor(Motor::WheelsL, +speed * xSign);
            this->setServo(Servo::WheelRF, +1.0);
            this->setServo(Servo::WheelLF, -1.0);
            this->setServo(Servo::WheelRB, -1.0);
            this->setServo(Servo::WheelLB, +1.0);

            // For some reason the robot explodes without this
            wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelRM)], 0.0);
            wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelLM)], 0.0);
        }
        else // The robot is moving and rotating at the same time
        {
            // Update motors and servos
            this->setMotor(Motor::Wheels, speed * ySign);
            this->setServo(Servo::WheelsF, -x);
            this->setServo(Servo::WheelsB, +x);

            // For some reason the robot explodes without this
            wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelRM)], 0.0);
            wb_motor_set_available_torque(this->motors[static_cast<int>(Motor::WheelLM)], 0.0);
        }
    }

    // Step simulation.
    if (wb_robot_step(this->timeStep) == -1)
        this->shouldStopFlag = true;
}

void WebotsRobot::setMotor(Motor motor, double velocity)
{
    if (motor == Motor::WheelRF || motor == Motor::WheelsR || motor == Motor::WheelsF || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelRF)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (motor == Motor::WheelLF || motor == Motor::WheelsL || motor == Motor::WheelsF || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelLF)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (motor == Motor::WheelRM || motor == Motor::WheelsR || motor == Motor::WheelsM || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelRM)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (motor == Motor::WheelLM || motor == Motor::WheelsL || motor == Motor::WheelsM || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelLM)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (motor == Motor::WheelRB || motor == Motor::WheelsR || motor == Motor::WheelsB || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelRB)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (motor == Motor::WheelLB || motor == Motor::WheelsL || motor == Motor::WheelsB || motor == Motor::Wheels)
        wb_motor_set_velocity(this->motors[static_cast<int>(Motor::WheelLB)], velocity * WebotsRobot::WHEEL_VEL_MUL);
    if (static_cast<int>(motor) > static_cast<int>(Motor::WheelLB) &&
        static_cast<int>(motor) < static_cast<int>(Motor::Count))
        wb_motor_set_velocity(this->motors[static_cast<int>(motor)], velocity);
}

void WebotsRobot::setServo(Servo servo, double position)
{
    if (servo == Servo::WheelRF || servo == Servo::WheelsR || servo == Servo::WheelsF || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelRF)], position * WebotsRobot::WHEEL_POS_MUL);
    if (servo == Servo::WheelLF || servo == Servo::WheelsL || servo == Servo::WheelsF || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelLF)], position * WebotsRobot::WHEEL_POS_MUL);
    if (servo == Servo::WheelRM || servo == Servo::WheelsR || servo == Servo::WheelsM || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelRM)], position * WebotsRobot::WHEEL_POS_MUL);
    if (servo == Servo::WheelLM || servo == Servo::WheelsL || servo == Servo::WheelsM || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelLM)], position * WebotsRobot::WHEEL_POS_MUL);
    if (servo == Servo::WheelRB || servo == Servo::WheelsR || servo == Servo::WheelsB || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelRB)], position * WebotsRobot::WHEEL_POS_MUL);
    if (servo == Servo::WheelLB || servo == Servo::WheelsL || servo == Servo::WheelsB || servo == Servo::Wheels)
        wb_motor_set_position(this->servos[static_cast<int>(Servo::WheelLB)], position * WebotsRobot::WHEEL_POS_MUL);
    if (static_cast<int>(servo) > static_cast<int>(Servo::WheelLB) &&
        static_cast<int>(servo) < static_cast<int>(Servo::Count))
        wb_motor_set_position(this->servos[static_cast<int>(servo)], position);
}

#else

// The controller was built without Webots support, provide empty implementation

WebotsRobot::WebotsRobot(int timeStep)
{
    abort(); // Unsupported operation
}

WebotsRobot::~WebotsRobot()
{
    abort(); // Unsupported operation
}

void WebotsRobot::setMovement(double translation, double rotation)
{
    abort(); // Unsupported operation
}

void WebotsRobot::setArmPosition(double x, double y, double z)
{
    abort(); // Unsupported operation
}

void WebotsRobot::setCameraAngle(double angle)
{
    abort(); // Unsupported operation
}

double WebotsRobot::readSensor(Sensor sensor) const
{
    abort(); // Unsupported operation
}

cv::Mat WebotsRobot::readCamera() const
{
    abort(); // Unsupported operation
}

bool WebotsRobot::shouldStop() const
{
    abort(); // Unsupported operation
}

void WebotsRobot::update()
{
    abort(); // Unsupported operation
}

#endif // USE_WEBOTS