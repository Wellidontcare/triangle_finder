#ifndef TRIANGLEFINDERMODEL_H
#define TRIANGLEFINDERMODEL_H
#include <iostream>
#include <string>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"


class TriangleFinderModel
{
//constructors
public:
    TriangleFinderModel();

//memberfunctions
    void load_image(const cv::Mat& image);
    cv::Mat generate_canny_preview();

//setter
    void set_upper_canny_threshold(const int& val);
    void set_lower_canny_threshold(const int& val);


private:
    cv::Mat original_image_;
    cv::Mat preview_image_;
    cv::Mat canny_preview_image_;

    int upper_canny_threshold_;
    int lower_canny_threshold_;
};

#endif // TRIANGLEFINDERMODEL_H
