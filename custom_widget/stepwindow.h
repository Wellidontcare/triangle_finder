#ifndef STEPWINDOW_H
#define STEPWINDOW_H

#include <QGraphicsView>
//#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QTabWidget>
#include <QWidget>

class StepWindow : public QWidget
{
    Q_OBJECT
    QTabWidget* step_tab_widget_ = nullptr;
    QVBoxLayout* master_layout_ = nullptr;

public:
    explicit StepWindow(QWidget *parent = nullptr);
    StepWindow() = delete;
    ~StepWindow() = default;

public slots:
    void update_steps(const std::vector<QImage>& images);
};

#endif // STEPWINDOW_H
