#include "dropenabledgraphicsview.h"

DropEnabledGraphicsView::DropEnabledGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
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
        event->acceptProposedAction();
        current_pixmap_ = QPixmap(qvariant_cast<QPixmap>(event->mimeData()->imageData()));
        emit successfull_drop_image_data(event->mimeData()->imageData());
        valid_drop = true;
    }
    if(event->mimeData()->hasText()){
        QString text = event->mimeData()->text();
        QString file_path;
        //handle drop from file explorer
        if(text.startsWith("file:///")){
            file_path = text.remove(0, 8);
            if(text.endsWith("jpg") || text.endsWith("png") || text.endsWith("bmp")){
                current_pixmap_.load(file_path);
                emit successfull_drop_image_file(file_path);
                valid_drop = true;
            }
        }
    }
    if(!valid_drop){
        QMessageBox::warning(this,
                             tr("Not an image"),
                             tr("Do you promise to only drop image files into the viewer? (*.jpg, *.png, *.bmp"),
                             QMessageBox::Yes);
        return;
    }

current_scene_.clear();
current_scene_.addPixmap(current_pixmap_);
setScene(&current_scene_);
fitInView(current_scene_.itemsBoundingRect(),Qt::KeepAspectRatio);
}

void DropEnabledGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropEnabledGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
