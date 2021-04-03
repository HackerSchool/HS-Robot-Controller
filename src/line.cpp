#include "line.hpp"

const int SEGMENT_HEIGHT = 100;

void line_follow(cv::Mat img, double* move_x, double* move_y) {
    cv::Mat line = img(cv::Rect(0, img.rows - SEGMENT_HEIGHT, img.cols, SEGMENT_HEIGHT));
    cv::cvtColor(line, line, cv::COLOR_BGR2GRAY);
    cv::blur(line, line, cv::Size(15.0, 15.0));
    cv::threshold(line, line, 65, 255, cv::THRESH_BINARY_INV);

    // Get average line point
    float mx = 0.0f, my = 0.0f;
    int count = 0;
    for (int x = 0; x < line.cols; ++x) {
        for (int y = 0; y < line.rows; ++y) {
            if (line.at<uchar>(y, x)) {
                mx += x;
                my += y;
                count += 1;
            }
        }
    }
    mx /= count;
    my /= count;

    cv::circle(line, cv::Point((int)mx, (int)my), 10, 155);

    cv::imshow("Line", line);

    mx = (mx / line.cols) * 2.0 - 1.0;

    // Only change movement if there are enough samples
    if (count > 100) {
        *move_x = mx;
        *move_y = 1.0 - my / line.rows;
    }
}
