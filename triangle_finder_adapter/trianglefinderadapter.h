#ifndef TRIANGLEFINDERADAPTER_H
#define TRIANGLEFINDERADAPTER_H

#include <vector>

#include <QDropEvent>
#include <QFileDialog>
#include <QFile>
#include <QGraphicsScene>
#include <QImage>
#include <QLayout>
#include <QMessageBox>
#include <QMimeData>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "triangle_finder_model/trianglefindermodel.h"

class TriangleFinderAdapter : public QObject
{
    Q_OBJECT

public:
    TriangleFinderAdapter(QWidget* parent);
    ~TriangleFinderAdapter() override = default;

public slots:

    void drag_and_drop_image_data_action(const QVariant& data);
    void drag_and_drop_image_file_action(const QString& file_path);

    void method1_checked_action();
    void method2_checked_action();

    void canny_upper_threshold_action(int upper);
    void canny_lower_threshold_action(int lower);

    void find_triangles_action(bool show_steps);

    void compare_triangles_action();

    void show_original();

signals:
    void scene_changed(QGraphicsScene*);
    void steps_are_ready(std::vector<QImage>* steps);

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
    QWidget* parent_;

    TriangleFinderModel model_;

    QPixmap current_pixmap_;
    cv::Mat current_mat_;
    QGraphicsScene current_scene_;
    QPixmap original_pixmap_;
    std::vector<QImage> steps_;

    int selected_method_ = 0;
    bool meassure_performance_ = false;
    bool show_steps_ = false;

    std::vector<cv::Point> found_triangles_;
    QPixmap final_image_;
};

enum{
    SAMPLE_0 = 0;
};

#endif // TRIANGLEFINDERADAPTER_H
