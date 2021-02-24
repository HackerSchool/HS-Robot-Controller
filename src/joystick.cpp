#include "joystick.hpp"

#ifdef USE_WEBOTS
#define TIME_STEP 64
#include <webots/keyboard.h>
#endif

struct Joystick {
    // There still isn't any state that needs to be stored here
};

Joystick* joystick_init() {
    Joystick* joystick = new Joystick();

#ifdef USE_WEBOTS
    wb_keyboard_enable(TIME_STEP);
#endif

    // Open socket for communication with the app
    // TODO

    return joystick;
}

void joystick_terminate(Joystick* joystick) {
#ifdef USE_WEBOTS
    wb_keyboard_disable();
#endif
    
    // Close communication socket
    // TODO

    delete joystick;
}

void joystick_read(Joystick* joystick, double* move_x, double* move_y) {
#ifdef USE_WEBOTS
    int key = wb_keyboard_get_key();
    
    switch (key) {
    case 'A':
        *move_x = -1.0;
        *move_y = 0.0;
        break;
    case 'D':
        *move_x = +1.0;
        *move_y = 0.0;
        break;
    case 'W':
        *move_x = 0.0;
        *move_y = +1.0;
        break;
    case 'S':
        *move_x = 0.0;
        *move_y = -1.0;
        break;
    }
#endif

    // Receive joystick data from communication socket
    // TODO
}