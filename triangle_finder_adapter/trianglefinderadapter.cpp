#include "trianglefinderadapter.h"

TriangleFinderAdapter::TriangleFinderAdapter(QWidget* parent)
    : parent_(parent) {}

TriangleFinderAdapter::~TriangleFinderAdapter() {}


//***************************DRAG AND DROP***********************************//
void TriangleFinderAdapter::drag_and_drop_image_data_action(const QVariant& data)
{
    current_image_ = qvariant_cast<QImage>(data);
    cv::Mat mat = qimage_to_mat(current_image_, CV_8UC3);
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    model_.load_image(mat);
    current_pixmap_ = qvariant_cast<QPixmap>(data);
    original_pixmap_ = current_pixmap_;
    set_scene();
}

void TriangleFinderAdapter::drag_and_drop_image_file_action(const QString &file_path)
{
    cv::Mat mat = cv::imread(file_path.toStdString());
    model_.load_image(mat);
    current_pixmap_.load(file_path);
    original_pixmap_ = current_pixmap_;
    set_scene();
}


//***************************CANNY EDGE DETECTOR PREVIEW*********************//
void TriangleFinderAdapter::canny_upper_threshold_action(const int& val)
{
    model_.set_upper_canny_threshold(val);
    model_.generate_canny_preview().copyTo(current_mat_);
    current_image_ = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(current_image_);
    set_scene();
}
void TriangleFinderAdapter::canny_lower_threshold_action(const int &val)
{
    model_.set_lower_canny_threshold(val);
    model_.generate_canny_preview().copyTo(current_mat_);
    current_image_ = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(current_image_);
    set_scene();
}

//********************************RESET VIEW OPTION*******************************//
void TriangleFinderAdapter::show_original()
{
    current_pixmap_ = original_pixmap_;
    set_scene();
}


//*******************************HELPER FUNCTIONS**********************************//
QImage TriangleFinderAdapter::mat_to_qimage(cv::Mat &mat, const QImage::Format &format)
{
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
    current_scene_.addPixmap(current_pixmap_);
    current_scene_.setSceneRect(current_scene_.itemsBoundingRect());
    emit scene_changed(&current_scene_);
}


