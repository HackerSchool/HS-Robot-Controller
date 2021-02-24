#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

struct Joystick;

// Initializes the joystick and returns the handle used.
Joystick* joystick_init();

// Terminates the joystick using the handle returned by joystick_init().
void joystick_terminate(Joystick* joystick);

// Reads the joystick input and outputs the move_x & move_y that should be
// passed to robot_update.
void joystick_read(Joystick* joystick, double* move_x, double* move_y);

#endif