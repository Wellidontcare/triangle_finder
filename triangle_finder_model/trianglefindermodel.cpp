#include "trianglefindermodel.h"

TriangleFinderModel::TriangleFinderModel(){}

void TriangleFinderModel::load_image(const cv::Mat &image)
{
    original_image_ = image.clone();
    update_preview();
}

cv::Mat TriangleFinderModel::generate_canny_preview()
{
    cv::Canny(preview_image_, canny_preview_image_, lower_canny_threshold_, upper_canny_threshold_);
    return canny_preview_image_;
}

TriangleFinderInfoContainer TriangleFinderModel::find_triangles_approx_poly(const bool &show_steps)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<cv::Mat> steps;
    cv::Mat step1;
    cv::Canny(preview_image_, step1, lower_canny_threshold_, upper_canny_threshold_);

    if(show_steps) steps.push_back(step1);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(step1, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    if(show_steps){
        cv::Mat step2;
        preview_image_.copyTo(step2);
        cv::drawContours(step2, contours, -1, cv::Scalar(0, 255, 0), 4, cv::LINE_AA);
        steps.push_back(step2);
    }

    std::vector<std::vector<cv::Point>> triangles;
    //filter out all contours with 3 edges -> those are triangles
    auto is_triangle = [](const std::vector<cv::Point>& contour)  {
        std::vector<cv::Point> approx_contour;
        //epsilon was found by trial and error
        cv::approxPolyDP(contour, approx_contour,/*epsilon*/0.01*cv::arcLength(contour, true), true);
        return approx_contour.size() == 3;};

    std::copy_if(contours.begin(), contours.end(), std::back_inserter(triangles), is_triangle);

    if(show_steps){
        cv::Mat step3;
        preview_image_.copyTo(step3);
        cv::drawContours(step3, triangles, -1, cv::Scalar(0, 255, 0), 4, cv::LINE_AA);
        steps.push_back(step3);
    }

    //filter small triangles (probably false positives)
    auto is_too_small = [](const std::vector<cv::Point>& triangle){
        return cv::arcLength(triangle, true) < 20 && cv::contourArea(triangle) < 20;
    };

    std::remove_if(triangles.begin(), triangles.end(), is_too_small);

    cv::Mat final_image(preview_image_.clone());

    cv::drawContours(final_image, triangles, -1, cv::Scalar(0, 255, 0), 4, cv::LINE_AA);

    auto end = std::chrono::high_resolution_clock::now();
    int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

    TriangleFinderInfoContainer ret_val;
    ret_val.steps = steps;
    ret_val.final_image = final_image.clone();
    ret_val.triangle_locations = triangles;
    ret_val.time_taken_my = elapsed;
    return ret_val;
}

TriangleFinderInfoContainer TriangleFinderModel::find_triangles_shape_factor(const bool &show_steps)
{
    std::cout << show_steps;
    return {};
}

void TriangleFinderModel::set_upper_canny_threshold(const int &val)
{
    upper_canny_threshold_ = val;
}

void TriangleFinderModel::set_lower_canny_threshold(const int &val)
{
    lower_canny_threshold_ = val;
}

void TriangleFinderModel::update_preview()
{
    //load a scaled down preview image
    double ratio = static_cast<double>(original_image_.cols) / original_image_.rows;
    int width = static_cast<int>(500*ratio);
    int height = 500;
    cv::resize(original_image_, preview_image_, cv::Size(width, height));
}
