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
        /// @param robot        The robot which this app controls.
        /// @param behaviourMgr The behaviour manager of the robot.
        App(robot::Robot& robot, behaviour::Manager& manager);
        virtual ~App() = default;

        /// Updates the UI.
        virtual void update() = 0;

    protected:
        robot::Robot& robot;
    };
} // namespace hs::app

#endif // HS_APP_APP_HPP