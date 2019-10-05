#ifndef TRIANGLEFINDERADAPTER_H
#define TRIANGLEFINDERADAPTER_H
#include <vector>

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
    void drag_and_drop_image_data(const QVariant& data);
    void drag_and_drop_image_file(const QString& file_path);

signals:
    void scene_changed(QGraphicsScene*);

private:
    QWidget* parent_;
    TriangleFinderModel model_;
    QPixmap current_qpixmap_;
    QImage current_qimage_;
    cv::Mat current_mat_;
    QGraphicsScene current_scene_;

    //conversion functions implementation based on https://qtandopencv.blogspot.com/2013/08/how-to-convert-between-cvmat-and-qimage.html
    QImage mat_to_qimage(cv::Mat& mat, QImage::Format& format);
    cv::Mat qimage_to_mat(QImage& qimage, int format);


};

#endif // TRIANGLEFINDERADAPTER_H
