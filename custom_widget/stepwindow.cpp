#include "stepwindow.h"

/*!
 * \brief StepWindow::StepWindow
 * Basic ui setup for the Step Window that shows
 * the steps of the algorithms if the show steps
 * box is checked
 * \param parent
 */
StepWindow::StepWindow(QWidget *parent)
    : QWidget(parent),
      step_tab_widget_(new QTabWidget(this)),
      master_layout_(new QVBoxLayout)

{
    setWindowFlag(Qt::Window);
    setWindowTitle("Step Viewer");
    setWindowIcon(QIcon(":/resources/icon.png"));
    setLayout(master_layout_);
}

/*!
 * \brief StepWindow::update_steps
 * Displays the steps of the selected algorithm if the
 * steps_are_ready signal is received
 * \param images
 */
void
StepWindow::on_steps_are_ready(const std::vector<QImage>& images)
{
    int tab_count = 1;
    step_tab_widget_->clear();

    for(auto image : images){
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
