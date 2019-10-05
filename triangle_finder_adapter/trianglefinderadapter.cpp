#include "trianglefinderadapter.h"

TriangleFinderAdapter::TriangleFinderAdapter(QWidget* parent)
    : parent_(parent) {}

TriangleFinderAdapter::~TriangleFinderAdapter() {}


//***************************DRAG AND DROP***********************************//
void TriangleFinderAdapter::drag_and_drop_image_data_action(const QVariant& data)
{
    current_qimage_ = qvariant_cast<QImage>(data);
    cv::Mat mat = qimage_to_mat(current_qimage_, CV_8UC3);
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    model_.load_image(mat);
    current_qpixmap_ = qvariant_cast<QPixmap>(data);
    set_scene();
}

void TriangleFinderAdapter::drag_and_drop_image_file_action(const QString &file_path)
{
    cv::Mat mat = cv::imread(file_path.toStdString());
    model_.load_image(mat);
    current_qpixmap_.load(file_path);
    set_scene();
}


//***************************CANNY EDGE DETECTOR PREVIEW*********************//
void TriangleFinderAdapter::canny_upper_threshold_action(const int& val)
{
    model_.set_upper_canny_threshold(val);
    model_.generate_canny_preview().copyTo(current_mat_);
    current_qimage_ = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_qpixmap_ = QPixmap::fromImage(current_qimage_);
    set_scene();
}
void TriangleFinderAdapter::canny_lower_threshold_action(const int &val)
{
    model_.set_lower_canny_threshold(val);
    model_.generate_canny_preview().copyTo(current_mat_);
    current_qimage_ = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_qpixmap_ = QPixmap::fromImage(current_qimage_);
    set_scene();
}

QImage TriangleFinderAdapter::mat_to_qimage(cv::Mat &mat, const QImage::Format &format)
{
    qDebug() << "mat to qimage called";
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
}

cv::Mat TriangleFinderAdapter::qimage_to_mat(QImage &qimage, int format)
{
    return cv::Mat(qimage.height(), qimage.width(), format,
                   const_cast<uchar*>(qimage.bits()),
                   static_cast<size_t>(qimage.bytesPerLine()));
}

void TriangleFinderAdapter::set_scene()
{
    current_scene_.clear();
    current_scene_.addPixmap(current_qpixmap_);
    qDebug() << "set scene called";
    emit scene_changed(&current_scene_);
}


