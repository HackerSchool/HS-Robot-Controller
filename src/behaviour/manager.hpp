#ifndef HS_BEHAVIOUR_MANAGER_HPP
#define HS_BEHAVIOUR_MANAGER_HPP

#include "behaviour.hpp"
#include "../robot/robot.hpp"

#include <map>

namespace hs::behaviour
{
    /// Class used to control robot behaviours.
    class Manager final
    {
    public:
        /// @param robot The robot which the manager controls.
        Manager(robot::Robot& robot);
        ~Manager();

        /// Registers a behaviour type.
        /// @tparam T Behaviour type.
        /// @tparam TArgs Behaviour constructor argument types.
        /// @param args Behaviour constructor arguments.
        template <typename T, typename... TArgs> void registerType(TArgs... args);

        /// Switches to another behaviour.
        /// The behaviour type must be registered first with registerType<T>(...).
        /// @tparam T Behaviour type.
        template <typename T> void set();

        /// Resets the current behaviour.
        void reset();

        /// Updates the current behaviour.
        void update();

    private:
        robot::Robot& robot; ///< Robot being controlled by the behaviour.

        std::map<const char*, Behaviour*> behaviours; ///< Registered behaviours.
        Behaviour* behaviour;                         ///< Current behaviour.
    };

    // Template methods implementation

    template <typename T, typename... TArgs> void Manager::registerType(TArgs... args)
    {
        if (this->behaviours.find(T::NAME) == this->behaviours.end())
            this->behaviours[T::NAME] = new T(this->robot, args...);
    }

    template <typename T> void Manager::set()
    {
        this->behaviour = this->behaviours[T::NAME];
    }

} // namespace hs::behaviour

#endif // HS_BEHAVIOUR_MANAGER_HPP
