#ifndef HS_BEHAVIOUR_LINE_FOLLOWER_HPP
#define HS_BEHAVIOUR_LINE_FOLLOWER_HPP

#include "behaviour.hpp"

namespace hs::behaviour
{
    /// Line following robot behaviour.
    ///
    /// Uses the robot's camera to detect a black line and makes the robot follow it.
    class LineFollower : public Behaviour
    {
    public:
        /// Behaviour type name. Custom behaviour must define this constant too.
        static inline const char* NAME = "LineFollower";

        // Use default constructor.
        using Behaviour::Behaviour;

        /// Updates the behaviour.
        virtual void update() override;
    };
} // namespace hs::behaviour

#endif // HS_BEHAVIOUR_LINE_FOLLOWER_HPP
