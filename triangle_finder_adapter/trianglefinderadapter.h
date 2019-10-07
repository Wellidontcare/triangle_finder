#ifndef TRIANGLEFINDERADAPTER_H
#define TRIANGLEFINDERADAPTER_H

#include "opencv2/highgui.hpp"

#include <vector>

#include <QDebug>
#include <QDropEvent>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QMessageBox>
#include <QMimeData>

#include "triangle_finder_model/trianglefindermodel.h"
#include "custom_widget/stepwindow.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

class TriangleFinderAdapter : public QObject
{
    Q_OBJECT

public:
    TriangleFinderAdapter(QWidget* parent);
    ~TriangleFinderAdapter() override = default;

public slots:
    void drag_and_drop_image_data_action(const QVariant& data);
    void drag_and_drop_image_file_action(const QString& file_path);

    void method1_checked();
    void method2_checked();

    void canny_upper_threshold_action(int upper);
    void canny_lower_threshold_action(int lower);

    void find_triangles(bool show_steps);

    void show_original();

signals:
    void scene_changed(QGraphicsScene*);

    void step_window_created(StepWindow *);

private:
    QWidget* parent_;
    TriangleFinderModel model_;
    QPixmap current_pixmap_;
    QImage current_image_;
    cv::Mat current_mat_;
    QGraphicsScene current_scene_;
    QPixmap original_pixmap_;
    int selected_method_ = 0;
    bool meassure_performance = false;

    std::vector<cv::Point> found_triangles_;
    std::vector<QPixmap> steps_;
    QPixmap final_image_;

    /*conversion functions implementation based on
    https://qtandopencv.blogspot.com/2013/08/how-to-convert-between-cvmat-and-qimage.html*/
    QImage mat_to_qimage(const cv::Mat& mat, const QImage::Format& format);
    cv::Mat qimage_to_mat(const QImage& qimage, int format);


    //helper functions
    void set_scene();


};

#endif // TRIANGLEFINDERADAPTER_H
