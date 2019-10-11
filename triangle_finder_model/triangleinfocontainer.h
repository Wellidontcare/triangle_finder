#ifndef TRIANGLEINFOCONTAINER_H
#define TRIANGLEINFOCONTAINER_H

#include <vector>
#include <opencv2/core.hpp>

struct TriangleFinderInfoContainer{
    std::vector<cv::Mat> steps;
    cv::Mat final_image;
    std::vector<std::vector<cv::Point>> triangle_locations;
    int time_taken_my;
};

#endif // TRIANGLEINFOCONTAINER_H
