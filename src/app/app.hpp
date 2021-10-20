#ifndef HS_APP_APP_HPP
#define HS_APP_APP_HPP

#include "../robot/robot.hpp"
#include "../behaviour/manager.hpp"

namespace hs::app
{
    /// Class used to represent controller user input.
    class App
    {
    public:
        /// @param robot   The robot which this app controls.
        /// @param manager The behaviour manager of the robot.
        App(const robot::Robot& robot, behaviour::Manager& manager);
        virtual ~App() = default;

        /// Gets the X axis of the joystick.
        /// @return Joystick X axis.
        virtual double getJoystickX() const = 0;

        /// Gets the Y axis of the joystick.
        /// @return Joystick Y axis.
        virtual double getJoystickY() const = 0;

        /// Gets the target camera angle.
        /// @return Target camera angle.
        virtual double getCameraAngle() const = 0;

        /// Updates the UI.
        virtual void update() = 0;

    protected:
        const robot::Robot& robot;   ///< Robot being controlled by the application.
        behaviour::Manager& manager; ///< Behaviour manager used by the application.
    };
} // namespace hs::app

#endif // HS_APP_APP_HPP