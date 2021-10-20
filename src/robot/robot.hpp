#ifndef HS_ROBOT_ROBOT_HPP
#define HS_ROBOT_ROBOT_HPP

#include <opencv2/opencv.hpp>

namespace hs::robot
{
    /// Enum used to identify sensors.
    enum class Sensor
    {
        DistanceRF, ///< Right-front distance sensor.
        DistanceLF, ///< Left-front distance sensor.
        DistanceR,  ///< Right distance sensor.
        DistanceL,  ///< Left distance sensor.

        Count ///< Number of sensors.
    };

    /// Class used to control the robot.
    class Robot
    {
    public:
        Robot() = default;
        virtual ~Robot() = default;

        /// Sets the robot's movement. The values passed to this function correspond to the joystick X and Y axes.
        /// @param rotation    How should the robot turn, left (negative) or right (positive).
        /// @param translation Forward (positive) and backwards (negative) movement.
        virtual void setMovement(double rotation, double translation) = 0;

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
        virtual double readSensor(Sensor sensor) const = 0;

        /// Gets the last image captured by the robot camera.
        /// @return Captured image.
        virtual cv::Mat readCamera() const = 0;

        /// Should the controller shut down?
        /// @return True or false.
        virtual bool shouldStop() const = 0;

        /// Updates the robot's motors, servos, sensors and captures a image. Should be called on a loop.
        virtual void update() = 0;
    };
} // namespace hs::robot

#endif // HS_ROBOT_ROBOT_HPP