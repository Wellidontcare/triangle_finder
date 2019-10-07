#ifndef STEPWINDOW_H
#define STEPWINDOW_H

#include <QWidget>

namespace Ui {
class StepWindow;
}

class StepWindow : public QWidget
{
    Q_OBJECT

public:
    //constructors
    explicit StepWindow(QWidget *parent = nullptr);
    ~StepWindow() override;

    //methods
public slots:
    void set_step_images_action(const std::vector<QPixmap>& pixmaps);

signals:



private:
    Ui::StepWindow *ui;
};

#endif // STEPWINDOW_H
