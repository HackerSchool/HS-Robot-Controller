#ifndef MAZE_HPP
#define MAZE_HPP

struct Maze;

// Initializes the maze following algorithm and returns the handle used.
Maze* maze_init();

// Terminates the maze following algorithm using the handle returned by
// maze_init().
void maze_terminate(Maze* maze);

// Takes the distances measured and outputs the move_x & move_y that should be
// passed to robot_update.
void maze_follow(Maze* maze, double* distances, double* move_x, double* move_y);

#endif