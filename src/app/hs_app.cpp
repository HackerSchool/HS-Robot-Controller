#include "hs_app.hpp"

#include "../behaviour/manual.hpp"
#include "../behaviour/line_follower.hpp"

#include <iostream>

using namespace hs::app;

#ifdef USE_BLUETOOTH

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

HSApp::HSApp(const robot::Robot& robot, behaviour::Manager& manager) : App(robot, manager)
{
    this->joystickX = 0.0;
    this->joystickY = 0.0;
    this->cameraAngle = 0.0;

    // Allocate socket
    this->socket = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // Bind socket to port 1 of the first available local bluetooth adapter
    sockaddr_rc locAddr = {0};
    locAddr.rc_family = AF_BLUETOOTH;
    locAddr.rc_bdaddr = {{0, 0, 0, 0, 0, 0}};
    locAddr.rc_channel = HSApp::RFCOMM_CHANNEL;
    ::bind(this->socket, (sockaddr*)&locAddr, sizeof(locAddr));

    // Put socket into listening mode
    ::listen(this->socket, 1);

    // Accept one connection
    std::cout << "Waiting for HS Robot App to connect..." << std::endl;
    sockaddr_rc remAddr = {0};
    socklen_t remAddrLen = sizeof(remAddr);
    this->client = ::accept(this->socket, reinterpret_cast<sockaddr*>(&remAddr), &remAddrLen);
    if (this->client < 0)
    {
        std::cerr << "Connection to HS Robot App failed, couldn't accept socket connection (accept() returned < 0)"
                  << std::endl;
        abort();
    }

    // Get address string from client and print it
    char addrStr[100];
    ba2str(reinterpret_cast<const bdaddr_t*>(&remAddr), addrStr);
    std::cout << "Connected to HS Robot App (" << addrStr << ")" << std::endl;
}

HSApp::~HSApp()
{
    std::cout << "Closed HS Robot App connection" << std::endl;
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
    // TODO:
    // Implement bluetooth protocol
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