#include "stepwindow.h"

StepWindow::StepWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlag(Qt::Window);
    setWindowTitle("Step Viewer");
    setWindowIcon(QIcon(":/resources/icon.png"));
}

void StepWindow::update_steps(const std::vector<QImage>& images)
{
    delete layout();
    int tab_count = 1;
    auto step_tab_widget = new QTabWidget;
    for(auto image : images){
        auto widget = new QWidget;
        auto layout = new QVBoxLayout;
        auto graphics_view = new QGraphicsView;
        QPixmap pixmap = QPixmap::fromImage(image);
        QGraphicsScene* scene = new QGraphicsScene;
        //qDebug() << pixmap.width() << " " << pixmap.height();
        scene->addPixmap(pixmap);
        graphics_view->setScene(scene);
        layout->addWidget(graphics_view);
        widget->setLayout(layout);
        step_tab_widget->addTab(widget, "Step " + QString::number(tab_count));
        tab_count++;
        graphics_view->fitInView(graphics_view->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    auto layout = new QVBoxLayout;
    layout->addWidget(step_tab_widget);
    setLayout(layout);
    show();
}
