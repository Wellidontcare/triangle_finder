#include "stepwindow.h"

StepWindow::StepWindow(QWidget *parent) :
    QWidget(parent),
    step_tab_widget_(new QTabWidget(this)),
    master_layout_(new QVBoxLayout)

{
    setWindowFlag(Qt::Window);
    setWindowTitle("Step Viewer");
    setWindowIcon(QIcon(":/resources/icon.png"));
    setLayout(master_layout_);
}

void
StepWindow::update_steps(const std::vector<QImage>& images)
{
    int tab_count = 1;
    step_tab_widget_->clear();

    for(const auto& image : images){
        auto widget = new QWidget;
        auto layout = new QVBoxLayout;
        auto graphics_view = new QGraphicsView;
        QPixmap pixmap = QPixmap::fromImage(image);
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addPixmap(pixmap);
        graphics_view->setScene(scene);
        layout->addWidget(graphics_view);
        widget->setLayout(layout);
        step_tab_widget_->addTab(widget, "Step " + QString::number(tab_count));
        tab_count++;
        graphics_view->fitInView(graphics_view->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    master_layout_->addWidget(step_tab_widget_);
    show();
}
