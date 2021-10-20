#ifndef HS_BEHAVIOUR_BEHAVIOUR_HPP
#define HS_BEHAVIOUR_BEHAVIOUR_HPP

#include "../robot/robot.hpp"

namespace hs::behaviour
{
    /// Class used to represent abstract robot behaviours.
    ///
    /// These correspond to a 'robot mode', for example, there can be a manual control behaviour, where the robot is
    /// controlled through the app manually, and a line following behaviour, where the robot follows a line
    /// automatically.
    /// The default behaviour doesn't do anything.
    class Behaviour
    {
    public:
        /// Behaviour type name. Custom behaviour must define this constant too.
        static inline const char* NAME = "Empty";

        /// @param robot The robot which this behaviour controls.
        Behaviour(robot::Robot& robot);
        virtual ~Behaviour() = default;

        /// Resets the behaviour to its original state.
        virtual void reset();

        /// Updates the behaviour.
        virtual void update();

    protected:
        robot::Robot& robot; ///< Robot being controlled by the behaviour.
    };
} // namespace hs::behaviour

#endif // HS_BEHAVIOUR_BEHAVIOUR_HPP
