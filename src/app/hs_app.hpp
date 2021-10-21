#ifndef HS_APP_HS_APP_HPP
#define HS_APP_HS_APP_HPP

#include "app.hpp"

namespace hs::app
{
    /// Implements the abstract App class using the real HS Robot application.
    class HSApp : public App
    {
    public:
        /// @param robot   The robot which this app controls.
        /// @param manager The behaviour manager of the robot.
        HSApp(const robot::Robot& robot, behaviour::Manager& manager);
        virtual ~HSApp() override;

        // Abstract methods implementation
        virtual double getJoystickX() const override;
        virtual double getJoystickY() const override;
        virtual double getCameraAngle() const override;
        virtual void update() override;

    private:
#ifdef USE_BLUETOOTH

        static inline const uint8_t RFCOMM_CHANNEL = 1; ///< RFCOMM channel.

        /// Registers the SDP service so that it can be discovered by the client.
        void registerSDPService();

        int socket; ///< Listener socket.
        int client; ///< Client socket.

        double joystickX;   ///< Last joystick X axis value.
        double joystickY;   ///< Last joystick Y axis value.
        double cameraAngle; ///< Target camera angle.

#endif // USE_BLUETOOTH
    };
} // namespace hs::app

#endif // HS_APP_HS_APP_HPP
