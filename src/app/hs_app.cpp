#include "hs_app.hpp"

#include "../behaviour/manual.hpp"
#include "../behaviour/line_follower.hpp"

#include <iostream>

using namespace hs::app;

#ifdef USE_BLUETOOTH

HSApp::HSApp(const robot::Robot& robot, behaviour::Manager& manager) : App(robot, manager)
{
    this->joystickX   = 0.0;
    this->joystickY   = 0.0;
    this->armX        = 0.0;
    this->armY        = 0.0;
    this->armZ        = 0.0;
    this->grip        = 0.0;
    this->cameraAngle = 0.0;
    this->socket      = 0;
    this->client      = 0;
    this->connect();
}

HSApp::~HSApp()
{
    std::cout << HSApp::DISCONNECTED_MSG << std::endl;
    ::close(this->client);
    ::close(this->socket);
}

double HSApp::getJoystickX() const
{
    return this->joystickX;
}

double HSApp::getJoystickY() const
{
    return this->joystickY;
}

double HSApp::getCameraAngle() const
{
    return this->cameraAngle;
}

void HSApp::update()
{
    ssize_t result;

    // Check if there's data to read from the socket
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(this->client, &rfd);
    timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    result          = select(this->client + 1, &rfd, nullptr, nullptr, &timeout);

    if (result > 0)
    {
        // Read magic number
        int8_t magicNumber[2];
        result = ::read(this->client, magicNumber, 2);
        if (result == 2 && magicNumber[0] == COMMAND_MAGIC_NUMBER_1 && magicNumber[1] == COMMAND_MAGIC_NUMBER_2)
            result = this->readCommand(); // Valid command, read it
    }

    // If any of the reads failed, reconnect
    if (result < 0)
    {
        std::cout << HSApp::CONNECTION_LOST_MSG << std::endl;
        this->connect();
    }
}

void HSApp::connect()
{
    for (;;) // Loop until connection is successful
    {
        // Close previous connection
        if (this->client != 0)
            ::close(this->client);
        if (this->socket != 0)
            ::close(this->socket);

        // Allocate socket
        this->socket = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // Bind socket to port 1 of the first available local bluetooth adapter
        sockaddr_rc locAddr = {0};
        locAddr.rc_family   = AF_BLUETOOTH;
        locAddr.rc_bdaddr   = {{0, 0, 0, 0, 0, 0}};
        locAddr.rc_channel  = HSApp::RFCOMM_CHANNEL;
        ::bind(this->socket, (sockaddr*)&locAddr, sizeof(locAddr));

        // Put socket into listening mode
        ::listen(this->socket, 1);

        // Accept one connection
        std::cout << HSApp::CONNECTING_MSG << std::endl;
        sockaddr_rc remAddr  = {0};
        socklen_t remAddrLen = sizeof(remAddr);
        this->client         = ::accept(this->socket, reinterpret_cast<sockaddr*>(&remAddr), &remAddrLen);
        if (this->client < 0)
        {
            std::cerr << HSApp::CONNECTION_FAILED_MSG << std::endl;
            continue;
        }

        // Get address string from client and print it
        char addrStr[100];
        ba2str(reinterpret_cast<const bdaddr_t*>(&remAddr), addrStr);
        std::cout << HSApp::CONNECTED_MSG << " (" << addrStr << ")" << std::endl;
        break;
    }
}

ssize_t HSApp::readCommand()
{
    ssize_t result;
    int8_t commandId = COMMAND_ERROR;
    int8_t i8[3];
    float f32[3];

    // Read command identifier
    result = ::read(this->client, &commandId, 1);

    // Execute command
    switch (commandId)
    {
    case COMMAND_SET_MODE:
        i8[0]  = MODE_ERROR;
        result = ::read(this->client, &i8[0], 1 * sizeof(int8_t));
        if (i8[0] == MODE_STOPPED)
            this->manager.set<behaviour::Behaviour>();
        else if (i8[0] == MODE_MANUAL)
            this->manager.set<behaviour::Manual>();
        else if (i8[0] == MODE_LINE_FOLLOWING)
            this->manager.set<behaviour::LineFollower>();
        else if (i8[0] == MODE_MAZE_SOLVING)
            ; // TODO
        break;

    case COMMAND_RESET:
        this->manager.reset();
        break;

    case COMMAND_SET_JOYSTICK:
        result = ::read(this->client, &f32[0], 2 * sizeof(float));
        if (result == 2 * sizeof(float))
        {
            this->joystickX = f32[0];
            this->joystickY = f32[1];
        }
        break;

    case COMMAND_SET_ARM:
        result = ::read(this->client, &f32[0], 3 * sizeof(float));
        if (result == 3 * sizeof(float))
        {
            this->armX = f32[0];
            this->armY = f32[1];
            this->armZ = f32[2];
        }
        break;

    case COMMAND_SET_GRIP:
        result = ::read(this->client, &f32[0], 3 * sizeof(float));
        if (result == 3 * sizeof(float))
        {
            this->armX = f32[0];
            this->armY = f32[1];
            this->armZ = f32[2];
        }
        break;

    case COMMAND_SET_CAMERA_ANGLE:
        result = ::read(this->client, &f32[0], 1 * sizeof(float));
        if (result == 1 * sizeof(float))
            this->cameraAngle = f32[0];
        break;

    case COMMAND_PLAY_AUDIO:
        // TODO
        break;

    case COMMAND_SET_AUDIO_VOL:
        // TODO
        break;

    case COMMAND_TAKE_PICTURE:
        // TODO
        break;

    case COMMAND_RECORD_VIDEO:
        // TODO
        break;

    case COMMAND_SET_LED_COLOR:
        // TODO
        break;
    }

    return result;
}

#else

HSApp::HSApp(const robot::Robot& robot, behaviour::Manager& manager) : App(robot, manager)
{
    abort(); // Unsupported operation
}

double HSApp::getJoystickX() const
{
    abort(); // Unsupported operation
}

double HSApp::getJoystickY() const
{
    abort(); // Unsupported operation
}

double HSApp::getCameraAngle() const
{
    abort(); // Unsupported operation
}

void HSApp::update()
{
    abort(); // Unsupported operation
}

#endif // USE_BLUETOOTH