#ifndef HS_ROBOT_ROBOT_HPP
#define HS_ROBOT_ROBOT_HPP

#include <opencv2/opencv.hpp>

namespace hs::robot
{
    /// Enum used to identify sensors.
    enum class Sensor
    {
        DLeftFront,  ///< Left-front distance sensor.
        DRightFront, ///< Right-front distance sensor.
        DLeft,       ///< Left distance sensor.
        DRight,      ///< Right distance sensor.

        Count ///< Number of sensors.
    };

    /// Class used to control the robot.
    class Robot
    {
    public:
        Robot() = default;
        virtual ~Robot() = default;

        /// Sets the robot's movement. The values passed to this function correspond to the
        /// joystick X and Y axes.
        /// @param translation Forward (positive) and backwards (negative) movement.
        /// @param rotation    How should the robot turn, left (negative) or right (positive).
        virtual void setMovement(double translation, double rotation) = 0;

        /// Sets the desired robot's arm position.
        /// @param x First DOF angle.
        /// @param y Second DOF angle.
        /// @param z Third DOF angle.
        virtual void setArmPosition(double x, double y, double z) = 0;

        /// Sets the robot's camera angle.
        /// @param angle New camera angle.
        virtual void setCameraAngle(double angle) = 0;

        /// Returns the value read by a sensor.
        /// @param sensor The sensor which will be read.
        /// @returns Value read.
        virtual double readSensor(Sensor sensor) = 0;

        /// Gets the last image captured by the robot camera.
        /// @return Captured image.
        virtual cv::Mat readCamera() = 0;
    };
} // namespace hs::robot

#endif // HS_ROBOT_ROBOT_HPP