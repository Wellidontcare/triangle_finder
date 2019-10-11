#ifndef TRIANGLEFINDERADAPTER_H
#define TRIANGLEFINDERADAPTER_H

#include <vector>

#include <QDropEvent>
#include <QFileDialog>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "triangle_finder_model/trianglefindermodel.h"

class TriangleFinderAdapter : public QObject
{
    QWidget* parent_ = nullptr;
    TriangleFinderModel model_{};
    QPixmap current_pixmap_{};
    cv::Mat current_mat_{};
    QGraphicsScene current_scene_{};
    QPixmap original_pixmap_{};
    std::vector<QImage> steps_{};

    int selected_method_ = 0;
    bool meassure_performance_ = false;
    bool show_steps_ = false;

    std::vector<cv::Point> found_triangles_{};
    QPixmap final_image_{};
    Q_OBJECT

public:
    TriangleFinderAdapter(QWidget* parent);
    TriangleFinderAdapter() = default;
    ~TriangleFinderAdapter() override = default;

public slots:

    void on_drag_and_drop(const QString& file_path);

    void on_method1_checked();
    void on_method2_checked();

    void on_canny_u_slider_moved(const int& upper);
    void on_canny_l_slider_moved(const int& lower);

    void on_find_triangles_button_clicked(const bool& show_steps);

    void on_compare_methods_button_clicked();

    void on_reset_view_button_clicked();

signals:
    void scene_changed(QGraphicsScene*);
    void steps_are_ready(const std::vector<QImage>& steps);
    void compare_ready(const QImage& left_image, const QImage& right_image,
                       const int& left_t, const int& right_t);

private:
    /*conversion functions implementation based on
    https://qtandopencv.blogspot.com/2013/08/how-to-convert-between-cvmat-and-qimage.html*/
    QImage mat_to_qimage(const cv::Mat& mat, const QImage::Format& format);
    cv::Mat qimage_to_mat(const QImage& qimage, int format);


    //helper functions
    void set_scene();
    void load_sample(const int& idx);

private:
    //member data
};

enum{
    SAMPLE_1 = 0,
    SAMPLE_2,
    SAMPLE_3,
    SAMPLE_4,
    SAMPLE_5,
    SAMPLE_6,
};

#endif // TRIANGLEFINDERADAPTER_H
