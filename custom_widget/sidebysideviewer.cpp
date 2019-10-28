#include "sidebysideviewer.h"

/*!
 * \brief SideBySideViewer::SideBySideViewer
 * Creates the basic ui for the Side by Side Viewer Window
 * \param parent
 */
SideBySideViewer::SideBySideViewer(QWidget *parent)
    : QWidget(parent),
      left_view_(new QGraphicsView),
      right_view_(new QGraphicsView),
      left_time_taken_(new QLabel),
      right_time_taken_(new QLabel),
      left_description_(new QLabel("Mehod 1 (Finds triangles by approximating a polygon)")),
      right_description_(new QLabel("Method 2 (Finds triangles, by calculating shape-factor)")),
      master_layout_(new QVBoxLayout),
      viewer_layout_(new QHBoxLayout),
      time_taken_layout_(new QHBoxLayout),
      description_layout_(new QHBoxLayout)
{
    setWindowFlag(Qt::Window);
    setWindowTitle("Side by side comparison");


    description_layout_->addWidget(left_description_);
    description_layout_->addWidget(right_description_);

    viewer_layout_->addWidget(left_view_);
    viewer_layout_->addWidget(right_view_);

    time_taken_layout_->addWidget(left_time_taken_);
    time_taken_layout_->addWidget(right_time_taken_);

    master_layout_->addLayout(description_layout_);
    master_layout_->addLayout(viewer_layout_);
    master_layout_->addLayout(time_taken_layout_);

    setLayout(master_layout_);
}

/*!
 * \brief SideBySideViewer::on_compare_ready
 * Displays the comparison of both algorithms
 *  if the compare_ready signal is recveived
 * \param left_image
 * Image for the left viewer
 * \param right_image
 * Image for the right viewer
 * \param left_t
 * Time taken left algorithm
 * \param right_t
 * Time taken right algorithm
 */
void
SideBySideViewer::on_compare_ready(const QImage &left_image, const QImage &right_image, int left_t, int right_t)
{
    QPixmap image1 = QPixmap::fromImage(left_image);
    QPixmap image2 = QPixmap::fromImage(right_image);

    left_scene_.clear();
    right_scene_.clear();

    left_scene_.addPixmap(image1);
    right_scene_.addPixmap(image2);

    left_view_->setScene(&left_scene_);
    right_view_->setScene(&right_scene_);

    left_time_taken_->setText("Time taken: " + QString::number(left_t) + " my");
    right_time_taken_->setText("Time taken: " + QString::number(right_t) + " my");

    left_view_->fitInView(left_scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
    right_view_->fitInView(right_scene_.itemsBoundingRect(), Qt::KeepAspectRatio);

    show();
}

/*!
 * \brief SideBySideViewer::resizeEvent
 * Resizes both images to fill the whole graphicsview
 * after resizing
 * \param event
 */
void SideBySideViewer::resizeEvent(QResizeEvent *event)
{
    left_view_->fitInView(left_scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
    right_view_->fitInView(right_scene_.itemsBoundingRect(), Qt::KeepAspectRatio);
    event->accept();
}
