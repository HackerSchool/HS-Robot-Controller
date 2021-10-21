#ifndef HS_APP_HS_APP_HPP
#define HS_APP_HS_APP_HPP

#include "app.hpp"

#ifdef USE_BLUETOOTH

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#endif // USE_BLUETOOTH

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

        /// Connecting message.
        static inline const char* CONNECTING_MSG = "Waiting for HS Robot App to connect...";
        /// Connected message.
        static inline const char* CONNECTED_MSG = "Connected to HS Robot App";
        /// Connection failed message.
        static inline const char* CONNECTION_FAILED_MSG = "Connection to HS Robot App failed";
        /// Disconnected message.
        static inline const char* DISCONNECTED_MSG = "Closed HS Robot App connection";
        /// Connection lost message.
        static inline const char* CONNECTION_LOST_MSG = "Lost connection to HS Robot App!";

        static inline const uint8_t RFCOMM_CHANNEL = 1; ///< RFCOMM channel.

        // Protocol command codes.
        enum
        {
            COMMAND_MAGIC_NUMBER_1   = 'H',  ///< First byte that precedes every command.
            COMMAND_MAGIC_NUMBER_2   = 'S',  ///< Second byte that precedes every command.
            COMMAND_SET_MODE         = 0x01, ///< Sets the robot mode (mode: i8).
            COMMAND_RESET            = 0x02, ///< Resets the current mode.
            COMMAND_SET_JOYSTICK     = 0x03, ///< Updates the joystick (x: f32, y: f32).
            COMMAND_SET_ARM          = 0x04, ///< Updates the arm's position (x: f32, y: f32, z: f32).
            COMMAND_SET_GRIP         = 0x05, ///< Updates the grip's position (grip: f32).
            COMMAND_SET_CAMERA_ANGLE = 0x06, ///< Updates the camera's angle (angle: f32).
            COMMAND_PLAY_AUDIO       = 0x07, ///< Plays a certain audio file (id: i8).
            COMMAND_SET_AUDIO_VOL    = 0x08, ///< Sets the audio volume used to play audio (vol: i8).
            COMMAND_TAKE_PICTURE     = 0x09, ///< Takes a picture and stores it.
            COMMAND_RECORD_VIDEO     = 0x0A, ///< Sets whether the robot is recording video or not (on: i8).
            COMMAND_SET_LED_COLOR    = 0x0B, ///< Sets the LED color (r: i8, g: i8, b: i8).
            COMMAND_ERROR            = 0xFF, ///< Used when the command couldn't be read.
        };

        // Protocol mode codes.
        enum
        {
            MODE_STOPPED        = 0x00, ///< Empty behaviour.
            MODE_MANUAL         = 0x01, ///< Manual behaviour.
            MODE_LINE_FOLLOWING = 0x02, ///< Line follower behaviour.
            MODE_MAZE_SOLVING   = 0x03, ///< Maze solver behaviour.
            MODE_ERROR          = 0xFF, ///< Used when the mode couldn't be read.
        };

        /// Connects to the HS robot app.
        void connect();

        /// Reads a command from the socket.
        ssize_t readCommand();

        int socket; ///< Listener socket.
        int client; ///< Client socket.

        double joystickX;   ///< Last joystick X axis value.
        double joystickY;   ///< Last joystick Y axis value.
        double armX;        ///< Last arm X value.
        double armY;        ///< Last arm Y value.
        double armZ;        ///< Last arm Z value.
        double grip;        ///< Last grip value.
        double cameraAngle; ///< Target camera angle.

#endif // USE_BLUETOOTH
    };
} // namespace hs::app

#endif // HS_APP_HS_APP_HPP
