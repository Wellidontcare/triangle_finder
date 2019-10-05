#include "trianglefindermodel.h"

TriangleFinderModel::TriangleFinderModel()
{

}

void TriangleFinderModel::load_image(const cv::Mat &image)
{
 image.copyTo(image_);
 std::cout << "Image Loaded:)\n";
}
