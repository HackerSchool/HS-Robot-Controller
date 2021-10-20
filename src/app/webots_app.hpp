#ifndef HS_APP_WEBOTS_APP_HPP
#define HS_APP_WEBOTS_APP_HPP

#include "app.hpp"

#ifdef USE_WEBOTS

#endif // USE_WEBOTS

namespace hs::app
{
    /// Implements the abstract App class using Webots.
    class WebotsApp : public App
    {
    public:
        /// @param robot   The robot which this app controls.
        /// @param manager The behaviour manager of the robot.
        /// @param timeStep Webots update time step.
        WebotsApp(const robot::Robot& robot, behaviour::Manager& manager, int timeStep = 64);

        // Abstract methods implementation
        virtual double getJoystickX() const override;
        virtual double getJoystickY() const override;
        virtual double getCameraAngle() const override;
        virtual void update() override;

    private:
#ifdef USE_WEBOTS

        double joystickX;   ///< Last joystick X axis value.
        double joystickY;   ///< Last joystick Y axis value.
        double cameraAngle; ///< Target camera angle.

        int timeStep; ///< Webots time step.

#endif // USE_WEBOTS
    };
} // namespace hs::app

#endif // HS_APP_WEBOTS_APP_HPP