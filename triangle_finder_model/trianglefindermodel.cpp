#include "trianglefindermodel.h"

constexpr int MIN_AREA = 20;
constexpr int MIN_ARC_LENGTH = 20;
constexpr double D_EPSILON = 0.04;

/*!
 * \brief TriangleFinderModel::load_image
 * Load the current image and generates a downscaled
 * version for the preview
 * \param image
 */
void
TriangleFinderModel::load_image(const cv::Mat &image)
{
    original_image_ = image;
    update_preview();
}

/*!
 * \brief TriangleFinderModel::generate_canny_preview
 * Runs the Canny Edge detector on the preview image
 * and returns the result
 * \return
 */
cv::Mat TriangleFinderModel::generate_canny_preview()
{
    cv::Canny(preview_image_, canny_preview_image_, lower_canny_threshold_,
              upper_canny_threshold_);
    return canny_preview_image_;
}

/*!
 * \brief TriangleFinderModel::find_triangles_approx_poly
 * Finds triangles in an image by approximating polygons around
 * each found contour and selecting contours that can be approximated
 * by 3 points
 * \param show_steps
 * Determines if the returned container holds the steps in a vector
 * else the steps vector is empty
 * \return
 * A container containing a vector of cv::Mats representing the steps,
 * a cv::Mat representing the image after the algorithm,
 * a vector of vector of points containing the information of the selected
 * contours (currently unused), the time taken by the algorithm (time taken
 * differs if show_steps is true)
 *
 */
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

/*!
 * \brief TriangleFinderModel::find_triangles_shape_factor
 * Finds triangles by determning the shape factor (area/diamter^2)
 * of all found contours and selecting contours with a shapefactor
 * between 0.43 and 0.49 as triangles
 *
 * \param show_steps
 * Determines if the returned container holds the steps in a vector
 * else the steps vector is empty
 * \return
 * A container containing a vector of cv::Mats representing the steps,
 * a cv::Mat representing the image after the algorithm,
 * a vector of vector of points containing the information of the selected
 * contours (currently unused), the time taken by the algorithm (time taken
 * differs if show_steps is true)
 */
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
        return sf >= 0.43 && sf <= 0.49;};

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

/*!
 * \brief TriangleFinderModel::set_upper_canny_threshold
 * Sets the internal state of the upper_canny_threshold
 * to the provided value
 * \param val
 * Value for the upper_canny_threshold should not be smaller than
 * 0 nor greater than 255
 */
void
TriangleFinderModel::set_upper_canny_threshold(const int &val)
{
    upper_canny_threshold_ = val;
}

/*!
 * \brief TriangleFinderModel::set_lower_canny_threshold
 * Sets the internal state of the lower_canny_threshold
 * to the provided value
 * \param val
 * Value for the lower_canny_threshold should not be smaller than
 * 0 nor greater than 255
 */
void
TriangleFinderModel::set_lower_canny_threshold(const int &val)
{
    lower_canny_threshold_ = val;
}

/*!
 * \brief TriangleFinderModel::update_preview
 * Scales the loaded image to a preview image with a height of 500
 * and same aspect ratio
 */
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

/*!
 * \brief TriangleFinderModel::diameter
 * Calcualtes the diameter as the distance between the two furthest
 * points of the provided conture
 * \param conture
 * \return
 */
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
