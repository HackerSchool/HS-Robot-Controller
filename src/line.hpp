#ifndef LINE_HPP
#define LINE_HPP

#include <opencv2/opencv.hpp>

// Takes an image and outputs the move_x & move_y that should be passed to
// robot_update.
void line_follow(const cv::Mat* img, double* move_x, double* move_y);

#endif