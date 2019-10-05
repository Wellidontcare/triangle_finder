#ifndef TRIANGLEFINDERMODEL_H
#define TRIANGLEFINDERMODEL_H
#include <iostream>
#include <string>
#include "opencv2/imgcodecs.hpp"


class TriangleFinderModel
{
//constructors
public:
    TriangleFinderModel();

//memberfunctions
    void load_image(const cv::Mat& image);
    void load_image(const std::string& file_path);

private:
    cv::Mat image_;

};

#endif // TRIANGLEFINDERMODEL_H
