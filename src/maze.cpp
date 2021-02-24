#include "maze.hpp"

struct Maze {
    // TODO: store path points?
};

Maze* maze_init() {
    Maze* maze = new Maze();

    return maze;
}

void maze_terminate(Maze* maze) {
    delete maze;
}

void maze_follow(Maze* maze, double* distances, double* move_x, double* move_y) {
    // Placeholder
    *move_x = 0.0;
    *move_y = 0.0;

    // TODO: implement maze follow algorithm
}
