#ifndef APP_HPP
#define APP_HPP

enum {
    MODE_JOYSTICK,
    MODE_LINE,
    MODE_MAZE,
};

struct App;

// Initializes the application communicator and returns the handle used.
App* app_init();

// Terminates the application comunicator using the handle returned by
// app_init().
void app_terminate(App* app);

// Receives information from the application and returns true if the current
// control mode changed.
bool app_update(App* app);

// Gets the current control mode.
int app_get_mode(App* app);

// Reads the app joystick input and outputs the move_x & move_y that should be
// passed to robot_update.
void app_read_joystick(App* app, double* move_x, double* move_y);

#endif