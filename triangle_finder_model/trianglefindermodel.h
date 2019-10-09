#ifndef TRIANGLEFINDERMODEL_H
#define TRIANGLEFINDERMODEL_H

#include <chrono>
#include <iostream>
#include <string>

#include "triangleinfocontainer.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


class TriangleFinderModel
{
public:
//constructors
    TriangleFinderModel() = default;

//memberfunctions
    void load_image(const cv::Mat& image);
    cv::Mat generate_canny_preview();

    TriangleFinderInfoContainer find_triangles_approx_poly(const bool& show_steps);
    TriangleFinderInfoContainer find_triangles_shape_factor(const bool& show_steps);

    void set_upper_canny_threshold(const int& val);
    void set_lower_canny_threshold(const int& val);

private:
    void update_preview();

private:
    //data
    cv::Mat original_image_;
    cv::Mat preview_image_;
    cv::Mat canny_preview_image_;

    int upper_canny_threshold_ = 255/3;
    int lower_canny_threshold_ = 255;
};

#endif // TRIANGLEFINDERMODEL_H
