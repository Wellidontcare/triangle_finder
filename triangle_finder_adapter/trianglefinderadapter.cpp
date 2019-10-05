#include "trianglefinderadapter.h"

TriangleFinderAdapter::TriangleFinderAdapter(QWidget* parent)
    : parent_(parent) {}

TriangleFinderAdapter::~TriangleFinderAdapter() {}

void TriangleFinderAdapter::drag_and_drop_image_data(const QVariant& data)
{
    QImage image = qvariant_cast<QImage>(data);
    cv::Mat mat = qimage_to_mat(current_qimage_, CV_8UC3);
    cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    model_.load_image(mat);
}

void TriangleFinderAdapter::drag_and_drop_image_file(const QString &file_path)
{
    cv::Mat mat = cv::imread(file_path.toStdString());
    model_.load_image(mat);
}

QImage TriangleFinderAdapter::mat_to_qimage(cv::Mat &mat, QImage::Format &format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
}

cv::Mat TriangleFinderAdapter::qimage_to_mat(QImage &qimage, int format)
{
    return cv::Mat(qimage.height(), qimage.width(), format,
                   const_cast<uchar*>(qimage.bits()),
                   static_cast<size_t>(qimage.bytesPerLine()));
}


