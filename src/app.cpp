#include "app.hpp"

#ifdef USE_WEBOTS
#define TIME_STEP 64
#include <webots/keyboard.h>
#endif

struct App {
    int mode;
    double joystick_x, joystick_y;
    double camera_z;
};

App* app_init() {
    App* app = new App();

    app->mode = MODE_JOYSTICK;
    app->joystick_x = 0.0;
    app->joystick_y = 0.0;

#ifdef USE_WEBOTS
    wb_keyboard_enable(TIME_STEP);
#endif

    // Open socket for communication with the app
    // TODO

    return app;
}

void app_terminate(App* app) {
#ifdef USE_WEBOTS
    wb_keyboard_disable();
#endif
    
    // Close communication socket
    // TODO

    delete app;
}

bool app_update(App* app) {
    bool changed_mode = false;

    // Receive data from communication socket
    // TODO

#ifdef USE_WEBOTS
    int key = wb_keyboard_get_key();

    switch (key) {
    // Movement keys:
    case 'A':
        app->joystick_x = -1.0;
        app->joystick_y = 0.0;
        break;
    case 'D':
        app->joystick_x = +1.0;
        app->joystick_y = 0.0;
        break;
    case 'W':
        app->joystick_x = 0.0;
        app->joystick_y = +1.0;
        break;
    case 'S':
        app->joystick_x = 0.0;
        app->joystick_y = -1.0;
        break;

    case 'P':
        app->joystick_x = 0.0;
        app->joystick_y = 0.0;
        break;

    // Camera movement keys:
    case 'U':
        app->camera_z = 0.001;
        break;

    case 'I':
        app->camera_z = 0.0;
        break;

    case 'O':
        app->camera_z = 0.002;
        break;

    // Mode change keys:
    case 'J':
        app->mode = MODE_JOYSTICK;
        changed_mode = true;
        break;
    case 'L':
        app->mode = MODE_LINE;
        app->joystick_x = 0.0;
        app->joystick_y = 0.0;
        changed_mode = true;
        break;
    case 'M':
        app->mode = MODE_MAZE;
        app->joystick_x = 0.0;
        app->joystick_y = 0.0;
        changed_mode = true;
        break;
    }
#endif

    return changed_mode;
}

int app_get_mode(App* app) {
    return app->mode;
}

void app_read_joystick(App* app, double* move_x, double* move_y, double* move_z) {
    *move_x = app->joystick_x;
    *move_y = app->joystick_y;
    *move_z = app->camera_z;
}