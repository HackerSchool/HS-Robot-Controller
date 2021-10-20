#ifndef HS_BEHAVIOUR_MANUAL_HPP
#define HS_BEHAVIOUR_MANUAL_HPP

#include "behaviour.hpp"
#include "../app/app.hpp"

namespace hs::behaviour
{
    /// Robot behaviour used for manual control.
    ///
    /// Uses the robot's camera to detect a black line and makes the robot follow it.
    class Manual : public Behaviour
    {
    public:
        /// Behaviour type name. Custom behaviour must define this constant too.
        static inline const char* NAME = "Manual";

        /// @param robot The robot which this behaviour controls.
        /// @param app   The app used to control the robot.
        Manual(robot::Robot& robot, const app::App& app);

        /// Updates the behaviour.
        virtual void update() override;

    private:
        const app::App& app;
    };
} // namespace hs::behaviour

#endif // HS_BEHAVIOUR_MANUAL_HPP
