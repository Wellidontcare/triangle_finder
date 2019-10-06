#include "dropenabledgraphicsview.h"

DropEnabledGraphicsView::DropEnabledGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void DropEnabledGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    setWhatsThis("drop image");

    event->acceptProposedAction();
}

void DropEnabledGraphicsView::dropEvent(QDropEvent *event)
{
    bool valid_drop = false;
    if(event->mimeData()->hasImage()){
        qDebug() << "Has image";
        event->acceptProposedAction();
        current_pixmap_ = QPixmap(qvariant_cast<QPixmap>(event->mimeData()->imageData()));
        emit successfull_drop_image_data_event(event->mimeData()->imageData());
        valid_drop = true;
    }
    if(event->mimeData()->hasUrls()){
        //workaround for linux
        //better build a mimedata parser
        QString text = event->mimeData()->urls()[0].toString();
        QString file_path;
        qDebug() << text;
        //handle drop from file explorer
        if(text.startsWith("file:///")){
            file_path = text.remove(0, 7);
            if(text.endsWith("jpg") || text.endsWith("png") || text.endsWith("bmp")){
                current_pixmap_.load(file_path);
                emit successfull_drop_image_file_event(file_path);
                valid_drop = true;
            }
        }
    }
    if(!valid_drop){
        QMessageBox msgbox("Not an image", "Do you promise to only drop image files into the viewer? (*.jpg, *.png, *.bmp)", QMessageBox::Icon::Warning, 0, 0, 0);
        msgbox.addButton("I promise!", QMessageBox::YesRole);
        msgbox.exec();
        return;
    }
}

void DropEnabledGraphicsView::resizeEvent(QResizeEvent *event)
{
    if(scene()){
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
    event->accept();
}

void DropEnabledGraphicsView::update_scene_action(QGraphicsScene *scn)
{
    setScene(scn);
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void DropEnabledGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropEnabledGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
