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
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

class TriangleFinderAdapter : public QObject
{
    Q_OBJECT

public:
    TriangleFinderAdapter(QWidget* parent);
    ~TriangleFinderAdapter();

public slots:
    void drag_and_drop_image_data_action(const QVariant& data);
    void drag_and_drop_image_file_action(const QString& file_path);

    void canny_upper_threshold_action(const int& upper);
    void canny_lower_threshold_action(const int& lower);

    void show_original();

signals:
    void scene_changed(QGraphicsScene*);

private:
    QWidget* parent_;
    TriangleFinderModel model_;
    QPixmap current_pixmap_;
    QImage current_image_;
    cv::Mat current_mat_;
    QGraphicsScene current_scene_;
    QPixmap original_pixmap_;

    /*conversion functions implementation based on
    https://qtandopencv.blogspot.com/2013/08/how-to-convert-between-cvmat-and-qimage.html*/
    QImage mat_to_qimage(cv::Mat& mat, const QImage::Format& format);
    cv::Mat qimage_to_mat(QImage& qimage, int format);

    //helper functions
    void set_scene();


};

#endif // TRIANGLEFINDERADAPTER_H
