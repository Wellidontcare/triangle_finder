#ifndef STEPWINDOW_H
#define STEPWINDOW_H

#include <QGraphicsView>
//#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QTabWidget>
#include <QWidget>

namespace Ui {
class StepWindow;
}

class StepWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StepWindow(QWidget *parent = nullptr);
    ~StepWindow() = default;

public slots:
    void update_steps(const std::vector<QImage>& images);

};

#endif // STEPWINDOW_H
