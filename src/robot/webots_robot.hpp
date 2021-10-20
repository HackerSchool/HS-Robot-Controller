#ifndef HS_ROBOT_WEBOTS_ROBOT_HPP
#define HS_ROBOT_WEBOTS_ROBOT_HPP

#include "robot.hpp"

#ifdef USE_WEBOTS

#include <webots/robot.h>
#include <webots/camera.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#include <map>

#endif // USE_WEBOTS

namespace hs::robot
{
    /// Implements the abstract Robot class using Webots.
    class WebotsRobot : public Robot
    {
    public:
        /// @param timeStep Webots update time step.
        WebotsRobot(int timeStep = 64);
        virtual ~WebotsRobot() override;

        // Abstract methods implementation
        virtual void setMovement(double rotation, double translation) override;
        virtual void setArmPosition(double x, double y, double z) override;
        virtual void setCameraAngle(double angle) override;
        virtual double readSensor(Sensor sensor) const override;
        virtual cv::Mat readCamera() const override;
        virtual bool shouldStop() const override;
        virtual void update() override;

    private:
#ifdef USE_WEBOTS

        /// Webots motors.
        enum class Motor
        {
            WheelRF, ///< Right front wheel motor.
            WheelLF, ///< Left front wheel motor.
            WheelRM, ///< Right middle wheel motor.
            WheelLM, ///< Left middle wheel motor.
            WheelRB, ///< Right back wheel motor.
            WheelLB, ///< Left back wheel motor.

            Count, ///< Number of motors.

            WheelsL, ///< Left wheel motors.
            WheelsR, ///< Right wheel motors.
            WheelsF, ///< Front wheel motors.
            WheelsM, ///< Middle wheel motors.
            WheelsB, ///< Back wheel motors.
            Wheels,  ///< Wheel motors.
        };

        /// Webots servos.
        enum class Servo
        {
            WheelRF, ///< Right front wheel servo.
            WheelLF, ///< Left front wheel servo.
            WheelRM, ///< Right middle wheel servo.
            WheelLM, ///< Left middle wheel servo.
            WheelRB, ///< Right back wheel servo.
            WheelLB, ///< Left back wheel servo.

            Camera, ///< Camera servo.

            Count, ///< Number of servos.

            WheelsL, ///< Left wheel servos.
            WheelsR, ///< Right wheel servos.
            WheelsF, ///< Front wheel servos.
            WheelsM, ///< Middle wheel servos.
            WheelsB, ///< Back wheel servos.
            Wheels,  ///< Wheel servos.
        };

        /// Sets a Webots motor's velocity.
        /// @param motor Motor enum.
        /// @param velocity New velocity.
        void setMotor(Motor motor, double velocity);

        /// Sets a Webots servo's position.
        /// @param servo Servo enum.
        /// @param position New position.
        void setServo(Servo servo, double position);

        /// Webots motors enum to Webots device name.
        static inline const std::map<Motor, const char*> MOTOR_TO_DEVICE = {
            {Motor::WheelRF, "wheel1"}, {Motor::WheelLF, "wheel2"}, {Motor::WheelRM, "wheel3"},
            {Motor::WheelLM, "wheel4"}, {Motor::WheelRB, "wheel5"}, {Motor::WheelLB, "wheel6"},
        };

        /// Webots servo enum to Webots device name.
        static inline const std::map<Servo, const char*> SERVO_TO_DEVICE = {
            {Servo::WheelRF, "wheel1servo"}, {Servo::WheelLF, "wheel2servo"}, {Servo::WheelRM, "wheel3servo"},
            {Servo::WheelLM, "wheel4servo"}, {Servo::WheelRB, "wheel5servo"}, {Servo::WheelLB, "wheel6servo"},
            {Servo::Camera, "SERVO_CAMARA"},
        };

        /// Sensors enum to Webots device name.
        static inline const std::map<Sensor, const char*> SENSOR_TO_DEVICE = {
            {Sensor::DistanceRF, "ds_left_front"},
            {Sensor::DistanceLF, "ds_right_front"},
            {Sensor::DistanceR, "ds_left"},
            {Sensor::DistanceL, "ds_right"},
        };

        /// Webots camera device name.
        static inline const char* CAMERA_DEVICE = "camera";

        /// Webots wheel motor velocity multiplier.
        static inline const double WHEEL_VEL_MUL = 1.0;

        /// Webots wheel servo position multiplier.
        static inline const double WHEEL_POS_MUL = 0.87;

        int timeStep; ///< Webots time step.

        WbDeviceTag camera;                                   ///< Robot camera tag.
        WbDeviceTag motors[static_cast<int>(Motor::Count)];   ///< Robot motor tags.
        WbDeviceTag servos[static_cast<int>(Servo::Count)];   ///< Robot servo tags.
        WbDeviceTag sensors[static_cast<int>(Sensor::Count)]; ///< Robot sensor tags.

        float rotation;      ///< Stores last value set by setMovement().
        float translation;   ///< Stores last value set by setMovement().
        float arm[3];        ///< Stores last value set by setArmPosition().
        float cameraAngle;   ///< Stores last value set by setCameraAngle().
        bool shouldStopFlag; ///< Should the controller shut down?

#endif // USE_WEBOTS
    };
} // namespace hs::robot

#endif // HS_ROBOT_WEBOTS_ROBOT_HPP