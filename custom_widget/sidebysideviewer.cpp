#include "sidebysideviewer.h"

SideBySideViewer::SideBySideViewer(QWidget *parent)
    : QWidget(parent),
      left_view_(new QGraphicsView),
      right_view_(new QGraphicsView),
      left_time_taken_(new QLabel),
      right_time_taken_(new QLabel),
      master_layout_(new QVBoxLayout),
      viewer_layout_(new QHBoxLayout),
      time_taken_layout_(new QHBoxLayout)
{
    setWindowFlag(Qt::Window);

    master_layout_->addLayout(viewer_layout_);
    master_layout_->addLayout(time_taken_layout_);

    viewer_layout_->addWidget(left_view_);
    viewer_layout_->addWidget(right_view_);

    time_taken_layout_->addWidget(left_time_taken_);
    time_taken_layout_->addWidget(right_time_taken_);

    setLayout(master_layout_);
}

void
SideBySideViewer::on_compare_ready(const QImage &left_image, const QImage &right_image, const int &left_t, const int &right_t)
{
    QPixmap image1 = QPixmap::fromImage(left_image);
    QPixmap image2 = QPixmap::fromImage(right_image);

    left_scene_.addPixmap(image1);
    right_scene_.addPixmap(image2);

    left_view_->setScene(&left_scene_);
    right_view_->setScene(&right_scene_);

    left_time_taken_->setText("Time taken: " + QString::number(left_t) + " my");
    right_time_taken_->setText("Time taken: " + QString::number(right_t) + " my");

    show();
}
