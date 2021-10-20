#include "line_follower.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

using namespace hs;
using namespace hs::behaviour;

void LineFollower::update()
{
    // Height of the segment that will be cut and analyzed
    static const int SEGMENT_HEIGHT = 100;

    // Get captured image from robot
    auto img = this->robot.readCamera();

    // Cut section of the image that matters, and process the image
    auto cut = img(cv::Rect(0, img.rows - SEGMENT_HEIGHT, img.cols, SEGMENT_HEIGHT));
    cv::Mat processed;
    cv::cvtColor(cut, processed, cv::COLOR_BGR2GRAY);
    cv::blur(processed, processed, cv::Size(15.0, 15.0));
    cv::threshold(processed, processed, 60, 255, cv::THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(processed, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    // Get the rectangles
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
    if (max_area > 1000.0f) {
        // Get center lines
        cv::Point line1[2] = {
            (verts[0] + verts[3]) / 2.0f,
            (verts[1] + verts[2]) / 2.0f
        };

        cv::Point line2[2] = {
            (verts[0] + verts[1]) / 2.0f,
            (verts[2] + verts[3]) / 2.0f
        };

        cv::Vec2f dir1 = cv::Vec2f(line1[1].x - line1[0].x, line1[1].y - line1[0].y) * 0.02f;
        cv::Vec2f dir2 = cv::Vec2f(line2[0].x - line2[1].x, line2[0].y - line2[1].y) * 0.02f;
        cv::Vec2f center;
        cv::Vec2f dir;
        float deviation;

        if (dir1.dot(dir1) > dir2.dot(dir2)) {
#ifdef SHOW_OPENCV_WINDOWS
            cv::line(cut, line1[0], line1[1], cv::Scalar(0, 0, 255), 2);
#endif // SHOW_OPENCV_WINDOWS
            center = cv::Vec2f(line1[0].x + line1[1].x, line1[0].y + line1[1].y) * 0.5f;
            dir = dir1;
        }
        else {
#ifdef SHOW_OPENCV_WINDOWS
            cv::line(cut, line2[0], line2[1], cv::Scalar(0, 0, 255), 2);
#endif // SHOW_OPENCV_WINDOWS
            center = cv::Vec2f(line2[0].x + line2[1].x, line2[0].y + line2[1].y) * 0.5f;
            dir = dir2;
        }

        // Calculate deviation
        deviation = (center(0) / static_cast<float>(img.rows)) * 2.0 - 1.0;

        // Move robot
        auto result = cv::Vec2f(deviation * 3.0f, -dir(1));
        this->robot.setMovement(result(0), result(1));
        std::cout << result << std::endl;
    }

#ifdef SHOW_OPENCV_WINDOWS
    // Show the line found
    cv::imshow("Lines", cut);
#endif // SHOW_OPENCV_WINDOWS
}
