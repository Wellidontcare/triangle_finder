#include "trianglefindermodel.h"

constexpr int MIN_AREA = 20;
constexpr int MIN_ARC_LENGTH = 20;
constexpr double D_EPSILON = 0.04;

void
TriangleFinderModel::load_image(const cv::Mat &image)
{
    original_image_ = image;
    update_preview();
}

cv::Mat TriangleFinderModel::generate_canny_preview()
{
    cv::Canny(preview_image_, canny_preview_image_, lower_canny_threshold_,
              upper_canny_threshold_);
    return canny_preview_image_;
}

TriangleFinderInfoContainer
TriangleFinderModel::find_triangles_approx_poly(const bool &show_steps)
{
    cv::RNG rng;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<cv::Mat> steps;
    cv::Mat step1;
    cv::Canny(preview_image_, step1,
              lower_canny_threshold_, upper_canny_threshold_);

    if(show_steps) steps.push_back(step1);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(step1, contours, hierarchy, cv::RETR_LIST,
                     cv::CHAIN_APPROX_SIMPLE);

    if(show_steps){
        cv::Mat step2;
        preview_image_.copyTo(step2);
        for(size_t i = 0; i < contours.size(); ++i)
            cv::drawContours(step2, contours, static_cast<int>(i),
                             cv::Scalar(rng.uniform(0, 255),
                                        rng.uniform(0, 255),
                                        rng.uniform(0, 255)), 2, cv::LINE_AA);
        steps.push_back(step2);
    }

    std::vector<std::vector<cv::Point>> triangles;
    //filter out all contours with 3 edges -> those are triangles
    auto is_triangle = [](const std::vector<cv::Point>& contour)  {
        std::vector<cv::Point> approx_contour;
        //epsilon was found by trial and error
        cv::approxPolyDP(contour, approx_contour,
                         /*epsilon*/D_EPSILON*cv::arcLength(contour, true), true);
        return approx_contour.size() == 3;};

    std::copy_if(contours.begin(),
                 contours.end(),
                 std::back_inserter(triangles),
                 is_triangle);

    if(show_steps){
        cv::Mat step3;
        preview_image_.copyTo(step3);
        cv::drawContours(step3, triangles, -1, cv::Scalar(0, 255, 0), 4,
                         cv::LINE_AA);
        steps.push_back(step3);
    }

    //filter small triangles (probably false positives)
    auto is_too_small = [](const std::vector<cv::Point>& triangle){
        return (cv::arcLength(triangle, true) < MIN_ARC_LENGTH
                || cv::contourArea(triangle) < MIN_AREA);
    };


    triangles.erase(std::remove_if(triangles.begin(), triangles.end(), is_too_small),
                    triangles.end());

    cv::Mat final_image = preview_image_.clone();

    if(!triangles.empty())
        cv::drawContours(final_image, triangles, -1, cv::Scalar(0, 255, 0), 4,
                         cv::LINE_AA);

    auto end = std::chrono::high_resolution_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    int elapsed = static_cast<int>(t.count());

    if(show_steps) steps.push_back(final_image);

    return {steps, final_image, triangles, elapsed};
}

TriangleFinderInfoContainer
TriangleFinderModel::find_triangles_shape_factor(const bool &show_steps)
{
    cv::RNG rng;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<cv::Mat> steps;
    cv::Mat step1;
    cv::Canny(preview_image_, step1,
              lower_canny_threshold_, upper_canny_threshold_);

    if(show_steps) steps.push_back(step1);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(step1, contours,
                     hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    if(show_steps){
        cv::Mat step2;
        preview_image_.copyTo(step2);
        for(size_t i = 0; i < contours.size(); ++i)
            cv::drawContours(step2, contours, static_cast<int>(i),
                             cv::Scalar(rng.uniform(0, 255),
                                        rng.uniform(0, 255),
                                        rng.uniform(0, 255)), 2, cv::LINE_AA);
        steps.push_back(step2);
    }

    std::vector<std::vector<cv::Point>> triangles;
    //filter out all contours with 3 edges -> those are triangles
    auto is_triangle = [this](std::vector<cv::Point> cnt){
        double area = cv::contourArea(cnt);
        double d = diameter(cnt);
        double sf = area / (d*d);
        return sf >= 0.44 && sf <= 0.483;};

    std::copy_if(contours.begin(), contours.end(),
                 std::back_inserter(triangles), is_triangle);

    if(show_steps){
        cv::Mat step3;
        preview_image_.copyTo(step3);
        cv::drawContours(step3, triangles,
                         -1, cv::Scalar(0, 255, 0), 4, cv::LINE_AA);
        steps.push_back(step3);
    }

    //filter small triangles (probably false positives)
    auto is_too_small = [](const std::vector<cv::Point>& triangle){
        return (cv::arcLength(triangle, true) < MIN_ARC_LENGTH || cv::contourArea(triangle) < MIN_AREA);
    };


    triangles.erase(std::remove_if(triangles.begin(), triangles.end(), is_too_small),
                    triangles.end());

    cv::Mat final_image = preview_image_.clone();

    if(!triangles.empty())
        cv::drawContours(final_image, triangles, -1, cv::Scalar(0, 255, 0), 4, cv::LINE_AA);

    auto end = std::chrono::high_resolution_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    int elapsed = static_cast<int>(t.count());
    if(show_steps) steps.push_back(final_image);
    return {steps, final_image, triangles, elapsed};
}

void
TriangleFinderModel::set_upper_canny_threshold(const int &val)
{
    upper_canny_threshold_ = val;
}

void
TriangleFinderModel::set_lower_canny_threshold(const int &val)
{
    lower_canny_threshold_ = val;
}

void
TriangleFinderModel::update_preview()
{
    constexpr int FIXED_MAX_HEIGHT = 500;
    //load a scaled down preview image
    double ratio = static_cast<double>(original_image_.cols) / original_image_.rows;
    int width = static_cast<int>(FIXED_MAX_HEIGHT*ratio);
    int height = FIXED_MAX_HEIGHT;
    cv::resize(original_image_, preview_image_, cv::Size(width, height));
}

double
TriangleFinderModel::diameter(const std::vector<cv::Point> &conture) const
{
    double max = 0;
    for(size_t i = 0; i < conture.size(); ++i){
        for(size_t j = 0; j < conture.size(); ++j){
            double dist = cv::norm(conture[i] - conture[j]);
            if(dist > max) max = dist;
        }
    }
    return max;
}
