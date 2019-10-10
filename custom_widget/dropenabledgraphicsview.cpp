#include "dropenabledgraphicsview.h"

DropEnabledGraphicsView::DropEnabledGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//SLOTS
void DropEnabledGraphicsView::update_scene_action(QGraphicsScene *scn)
{
    setScene(scn);
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

//PROTECTED
//SIGNALS
void DropEnabledGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

//currently only drop from file explorer is supported
void DropEnabledGraphicsView::dropEvent(QDropEvent *event)
{   
    if(event->mimeData()->hasUrls()){
        QString text = event->mimeData()->urls()[0].toString();
        QString file_path;

        //handle drop from file explorer
        if(text.startsWith("file:///")){
            file_path = text.remove(0, FILE_PREFIX);
            if(text.endsWith("jpg") || text.endsWith("png") || text.endsWith("bmp")){
                current_pixmap_.load(file_path);
                emit successfull_drop_image_file_event(file_path);
            }
        }
        return;
    }
    else{
        QMessageBox msgbox("Not an image", "Do you promise to only drop image files into the viewer? (*.jpg, *.png, *.bmp)", QMessageBox::Icon::Warning, 0, 0, 0);
        msgbox.addButton("I promise!", QMessageBox::YesRole);
        msgbox.exec();
    }
}

void DropEnabledGraphicsView::resizeEvent(QResizeEvent *event)
{
    if(scene()) fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    event->accept();
}


void DropEnabledGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropEnabledGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
