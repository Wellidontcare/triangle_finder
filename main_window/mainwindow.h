#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "custom_widget/dropenabledgraphicsview.h"
#include "custom_widget/stepwindow.h"
#include "triangle_finder_adapter/trianglefinderadapter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow* ui;
    TriangleFinderAdapter triangle_finder_;
    StepWindow* step_window_;

};
#endif // MAINWINDOW_H
