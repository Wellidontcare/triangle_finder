#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "custom_widget/dropenabledgraphicsview.h"
#include "triangle_finder_adapter/trianglefinderadapter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class TriangleFinderAdapter;

private:
    Ui::MainWindow *ui;
    TriangleFinderAdapter* triangle_finder_;
};
#endif // MAINWINDOW_H
