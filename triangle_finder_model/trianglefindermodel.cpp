#include "trianglefindermodel.h"

TriangleFinderModel::TriangleFinderModel(){}

void TriangleFinderModel::load_image(const cv::Mat &image)
{
 image.copyTo(original_image_);
 //load a scaled down preview image

 double ratio = static_cast<double>(original_image_.cols) / original_image_.rows;
 int width = static_cast<int>(500*ratio);
 int height = 500;
 cv::resize(original_image_, preview_image_, cv::Size(width, height));
}

cv::Mat TriangleFinderModel::generate_canny_preview()
{
    cv::Canny(preview_image_, canny_preview_image_, lower_canny_threshold_, upper_canny_threshold_);
    return canny_preview_image_;
}

void TriangleFinderModel::set_upper_canny_threshold(const int &val)
{
    upper_canny_threshold_ = val;
}

void TriangleFinderModel::set_lower_canny_threshold(const int &val)
{
    lower_canny_threshold_ = val;
}
