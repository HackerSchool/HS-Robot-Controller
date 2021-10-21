#ifndef HS_ROBOT_HS_ROBOT_HPP
#define HS_ROBOT_HS_ROBOT_HPP

#include "robot.hpp"

namespace hs::robot
{
    /// Implements the abstract Robot class using the real HS robot.
    class HSRobot : public Robot
    {
    public:
        HSRobot();
        virtual ~HSRobot() override;

        // Abstract methods implementation
        virtual void setMovement(double rotation, double translation) override;
        virtual void setArmPosition(double x, double y, double z) override;
        virtual void setCameraAngle(double angle) override;
        virtual double readSensor(Sensor sensor) const override;
        virtual cv::Mat readCamera() const override;
        virtual bool shouldStop() const override;
        virtual void update() override;
    };
} // namespace hs::robot

#endif // HS_ROBOT_HS_ROBOT_HPP
