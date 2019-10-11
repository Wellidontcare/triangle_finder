#include "trianglefinderadapter.h"

TriangleFinderAdapter::TriangleFinderAdapter(QWidget* parent)
    : parent_(parent) {
    load_sample(SAMPLE_1);
}



//***************************DRAG AND DROP***********************************//
void
TriangleFinderAdapter::on_drag_and_drop(const QString &file_path)
{
    cv::Mat mat = cv::imread(file_path.toStdString());
    model_.load_image(mat);
    current_pixmap_.load(file_path);
    original_pixmap_ = current_pixmap_.copy();
    set_scene();
}

void
TriangleFinderAdapter::on_method1_checked()
{
    selected_method_ = 1;
}

void
TriangleFinderAdapter::on_method2_checked()
{
    selected_method_ = 2;
}


//***************************CANNY EDGE DETECTOR PREVIEW*********************//
void
TriangleFinderAdapter::on_canny_u_slider_moved(const int& upper)
{
    model_.set_upper_canny_threshold(upper);
    current_mat_ = model_.generate_canny_preview();
    QImage tmp_image = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(tmp_image);
    set_scene();
}

void
TriangleFinderAdapter::on_canny_l_slider_moved(const int& lower)
{
    model_.set_lower_canny_threshold(lower);
    current_mat_ = model_.generate_canny_preview();
    QImage tmp_image = mat_to_qimage(current_mat_, QImage::Format_Grayscale8);
    current_pixmap_ = QPixmap::fromImage(tmp_image);
    set_scene();
}


/**************************FIND TRIANGLES ADAPTER*****************************/
void
TriangleFinderAdapter::on_find_triangles_button_clicked(const bool& show_steps)
{
    TriangleFinderInfoContainer triangle_info;
    if(selected_method_ == 0) {
        QMessageBox::warning(parent_,
                             "No method selected", "Please select a method");
        return;
    }
    if(selected_method_ == 1)
        triangle_info  = model_.find_triangles_approx_poly(show_steps);
    else if(selected_method_ == 2)
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
        emit steps_are_ready(steps_);
    }
}

void
TriangleFinderAdapter::on_compare_methods_button_clicked()
{
    TriangleFinderInfoContainer data1 = model_.find_triangles_approx_poly(false);
    TriangleFinderInfoContainer data2 = model_.find_triangles_shape_factor(false);

    QImage left_image = mat_to_qimage(data1.final_image, QImage::Format_RGB888).rgbSwapped();
    QImage right_image = mat_to_qimage(data2.final_image, QImage::Format_RGB888).rgbSwapped();

    int left_t = data1.time_taken_my;
    int right_t = data2.time_taken_my;

    emit compare_ready(left_image, right_image, left_t, right_t);
    return;
}

void
TriangleFinderAdapter::on_reset_view_button_clicked()
{
    current_pixmap_ = original_pixmap_;
    set_scene();
}


void
TriangleFinderAdapter::set_scene()
{
    current_scene_.clear();
    current_scene_.addPixmap(current_pixmap_);
    current_scene_.setSceneRect(current_scene_.itemsBoundingRect());
    emit scene_changed(&current_scene_);
}

void
TriangleFinderAdapter::load_sample(const int &idx)
{
    QString file_path = ":/resources/sample" + QString::number(idx) + ".jpg";
    QFile tmp_image_file(file_path);
    tmp_image_file.open(QIODevice::ReadOnly);
    QByteArray buf = tmp_image_file.readAll();
    std::vector<char> img_data(buf.begin(), buf.end());
    cv::Mat tmp_mat = cv::imdecode(img_data, cv::IMREAD_COLOR);
    current_mat_ = tmp_mat.clone();
    model_.load_image(current_mat_);

    current_pixmap_.load(file_path);
    original_pixmap_ = current_pixmap_.copy();
    set_scene();
}

QImage
TriangleFinderAdapter::mat_to_qimage(const cv::Mat &mat, const QImage::Format& format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format).copy();
}

cv::Mat
TriangleFinderAdapter::qimage_to_mat(const QImage &qimage, int format)
{
    return cv::Mat(qimage.height(), qimage.width(), format,
                   const_cast<uchar*>(qimage.bits()),
                   static_cast<size_t>(qimage.bytesPerLine())).clone();
}
