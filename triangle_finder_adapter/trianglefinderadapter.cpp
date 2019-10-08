#include "trianglefinderadapter.h"

TriangleFinderAdapter::TriangleFinderAdapter(QWidget* parent)
    : parent_(parent) {}



//***************************DRAG AND DROP***********************************//
void TriangleFinderAdapter::drag_and_drop_image_data_action(const QVariant& data)
{
    QImage tmp_image = qvariant_cast<QImage>(data);
    cv::Mat mat = qimage_to_mat(tmp_image, CV_8UC3);
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
    original_pixmap_ = current_pixmap_.copy();
    set_scene();
}

void TriangleFinderAdapter::method1_checked()
{
    selected_method_ = 1;
}

void TriangleFinderAdapter::method2_checked()
{
    selected_method_ = 2;
}


//***************************CANNY EDGE DETECTOR PREVIEW*********************//
void TriangleFinderAdapter::canny_upper_threshold_action(int upper)
{
    model_.set_upper_canny_threshold(upper);
    current_mat_ = model_.generate_canny_preview();
    QImage tmp_image = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(tmp_image);
    set_scene();
}
void TriangleFinderAdapter::canny_lower_threshold_action(int lower)
{
    model_.set_lower_canny_threshold(lower);
    current_mat_ = model_.generate_canny_preview();
    QImage tmp_image = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(tmp_image);
    set_scene();
}


//*************************FIND TRIANGLES ADAPTER****************************//
void TriangleFinderAdapter::find_triangles(bool show_steps)
{
    TriangleFinderInfoContainer triangle_info;
    if(selected_method_ == 1)
       triangle_info  = model_.find_triangles_approx_poly(show_steps);
    if(selected_method_ == 2)
        triangle_info = model_.find_triangles_shape_factor(show_steps);
    cv::Mat tmp_mat = triangle_info.final_image;
    cv::cvtColor(tmp_mat, current_mat_, cv::COLOR_BGR2RGB);
    QImage tmp_image = mat_to_qimage(current_mat_, QImage::Format_RGB888);
    current_pixmap_ = QPixmap::fromImage(tmp_image);
    set_scene();

    if(show_steps){
        steps_.clear();
        for(auto& step : triangle_info.steps){
            cv::Mat tmp_mat;
            cv::cvtColor(step, tmp_mat, cv::COLOR_BGR2RGB);
            QImage tmp = mat_to_qimage(tmp_mat, QImage::Format_RGB888);
            steps_.push_back(tmp);
        }
        emit steps_are_ready(&steps_);
    }
}

//********************************RESET VIEW OPTION*******************************//
void TriangleFinderAdapter::show_original()
{
    current_pixmap_ = original_pixmap_;
    set_scene();
}


//*******************************HELPER FUNCTIONS**********************************//
QImage TriangleFinderAdapter::mat_to_qimage(const cv::Mat &mat, const QImage::Format& format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format).copy();
}

cv::Mat TriangleFinderAdapter::qimage_to_mat(const QImage &qimage, int format)
{
    return cv::Mat(qimage.height(), qimage.width(), format,
                   const_cast<uchar*>(qimage.bits()),
                   static_cast<size_t>(qimage.bytesPerLine())).clone();
}

void TriangleFinderAdapter::set_scene()
{
    current_scene_.clear();
    current_scene_.addPixmap(current_pixmap_);
    current_scene_.setSceneRect(current_scene_.itemsBoundingRect());
    emit scene_changed(&current_scene_);
}


