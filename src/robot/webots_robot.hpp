#ifndef HS_ROBOT_WEBOTS_ROBOT_HPP
#define HS_ROBOT_WEBOTS_ROBOT_HPP

#include "robot.hpp"

namespace hs::robot
{
    /// Implements the abstract Robot class using Webots.
    class WebotsRobot : public Robot
    {
    public:
        WebotsRobot();
        virtual ~WebotsRobot() override;

        // Abstract methods implementation
        virtual void setMovement(double translation, double rotation) override;
        virtual void setArmPosition(double x, double y, double z) override;
        virtual void setCameraAngle(double angle) override;
        virtual double readSensor(Sensor sensor) override;
        virtual cv::Mat readCamera() override;
    };
} // namespace hs::robot

#endif // HS_ROBOT_WEBOTS_ROBOT_HPP