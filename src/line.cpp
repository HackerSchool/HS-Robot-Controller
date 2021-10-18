#include "line.hpp"

const int SEGMENT_HEIGHT = 100;

void line_follow(cv::Mat img, double* move_x, double* move_y) {
    cv::Mat cut = img(cv::Rect(0, img.rows - SEGMENT_HEIGHT, img.cols, SEGMENT_HEIGHT));
    cv::Mat processed;
    cv::cvtColor(cut, processed, cv::COLOR_BGR2GRAY);
    cv::blur(processed, processed, cv::Size(15.0, 15.0));
    cv::threshold(processed, processed, 65, 255, cv::THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(processed, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    
    // Get the rects
    float max_area = 0;
    cv::Point verts[4];
    for (size_t i = 0; i < contours.size(); i++) {
        auto rect = cv::minAreaRect(contours[i]);
        if (max_area < rect.size.area()) {
            max_area = rect.size.area();
            cv::Point2f verts2f[4];
            rect.points(verts2f);
            for (int i = 0; i < 4; ++i) {
                verts[i] = verts2f[i];
            }
        }
    }
    
    // Only process if the rectangle is large enough
    if (max_area > 200.0f) {
        // Get center lines
        cv::Point line1[2] = {
            (verts[0] + verts[3]) / 2.0f,
            (verts[1] + verts[2]) / 2.0f
        };
        cv::Point line2[2] = {
            (verts[0] + verts[1]) / 2.0f,
            (verts[2] + verts[3]) / 2.0f
        };

        cv::Vec2f dir1 = cv::Vec2f(line1[1].x - line1[0].x, line1[1].y - line1[0].y);
        cv::Vec2f dir2 = cv::Vec2f(line2[1].x - line2[0].x, line2[1].y - line2[0].y);

        if (dir1.dot(dir1) > dir2.dot(dir2)) {
            cv::line(cut, line1[0], line1[1], cv::Scalar(0, 0, 255), 2);
        }
        else {
            cv::line(cut, line2[0], line2[1], cv::Scalar(0, 0, 255), 2);
        }
    }
    cv::imshow("Lines", cut);

    //cv::imshow("Lines", edges);

    // Get average line point
    /*float mx = 0.0f, my = 0.0f;
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
    if (count > 200) {
        *move_x = mx;
        *move_y = 1.0 - my / line.rows;
    }*/
}
