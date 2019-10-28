#ifndef SIDEBYSIDEVIEWER_H
#define SIDEBYSIDEVIEWER_H

#include <QDebug>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QResizeEvent>
#include <QWidget>

class SideBySideViewer : public QWidget
{
    Q_OBJECT
    QGraphicsView* left_view_ = nullptr;
    QGraphicsView* right_view_ = nullptr;
    QLabel* left_time_taken_ = nullptr;
    QLabel* right_time_taken_ = nullptr;
    QLabel* left_description_ = nullptr;
    QLabel* right_description_ = nullptr;
    QVBoxLayout* master_layout_ = nullptr;
    QHBoxLayout* viewer_layout_ = nullptr;
    QHBoxLayout* time_taken_layout_ = nullptr;
    QHBoxLayout* description_layout_ = nullptr;
    QGraphicsScene right_scene_{};
    QGraphicsScene left_scene_{};

public:
    explicit SideBySideViewer(QWidget *parent = nullptr);

public slots:
    void on_compare_ready(const QImage& left_image, const QImage& right_image,
                          int left_t, int right_t);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // SIDEBYSIDEVIEWER_H
