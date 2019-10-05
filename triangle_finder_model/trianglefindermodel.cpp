#include "trianglefindermodel.h"

TriangleFinderModel::TriangleFinderModel(){}

void TriangleFinderModel::load_image(const cv::Mat &image)
{
 image.copyTo(original_image_);
 original_image_.copyTo(preview_image_);
 {
 int cols = preview_image_.cols;
 int rows = preview_image_.rows;

 double x_scale = static_cast<double>(cols) / (10 * cols);
 double y_scale = static_cast<double>(rows) / (10 * rows);
 cv::resize(preview_image_, preview_image_, cv::Size(), x_scale, y_scale);
 }
}

cv::Mat TriangleFinderModel::generate_canny_preview()
{
    cv::Canny(original_image_, preview_image_, lower_canny_threshold_, upper_canny_threshold_);
    return preview_image_;
}

void TriangleFinderModel::set_upper_canny_threshold(const int &val)
{
    upper_canny_threshold_ = val;
}

void TriangleFinderModel::set_lower_canny_threshold(const int &val)
{
    lower_canny_threshold_ = val;
}
