#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

#include "custom_widget/dropenabledgraphicsview.h"
#include "custom_widget/stepwindow.h"
#include "custom_widget/sidebysideviewer.h"
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
    //member data
    Ui::MainWindow* ui = nullptr;
    TriangleFinderAdapter triangle_finder_{};
    StepWindow* step_window_ = nullptr;
    SideBySideViewer* side_by_side_viewer_ = nullptr;

};
#endif // MAINWINDOW_H
