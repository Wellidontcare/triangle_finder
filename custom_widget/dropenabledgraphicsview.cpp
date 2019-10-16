#include "dropenabledgraphicsview.h"

DropEnabledGraphicsView::DropEnabledGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//SLOTS
void
DropEnabledGraphicsView::on_scene_changed(QGraphicsScene *scn)
{
    setScene(scn);
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void
DropEnabledGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

//currently only drop from file explorer is supported
void
DropEnabledGraphicsView::dropEvent(QDropEvent *event)
{   
    if(event->mimeData()->hasUrls()){
        QString drop_text = event->mimeData()->urls()[0].toString();
        //handle drop from file explorer
        if(valid_image_file(drop_text)){
            QString file_path = drop_text.remove(0, FILE_PREFIX);
            current_pixmap_.load(file_path);
            emit successfull_drop_image_file_event(file_path);
            return;
        }
    }
        QMessageBox msgbox("Not an image",
                           "Do you promise to only drop image "
                           "files into the viewer? (*.jpg, *.png, *.bmp)",
                           QMessageBox::Icon::Warning, 0, 0, 0);

        msgbox.addButton("I promise!", QMessageBox::YesRole);
        msgbox.exec();
}

bool
DropEnabledGraphicsView::valid_image_file(const QString &file_path) const
{
    return file_path.startsWith("file:///")
            && (file_path.endsWith("jpg")
                || file_path.endsWith("png")
                || file_path.endsWith("bmp"));
}

void
DropEnabledGraphicsView::resizeEvent(QResizeEvent *event)
{
    if(scene()) fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    event->accept();
}


void
DropEnabledGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void
DropEnabledGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
