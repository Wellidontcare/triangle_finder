#include "dropenabledgraphicsview.h"

DropEnabledGraphicsView::DropEnabledGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/*!
 * \brief DropEnabledGraphicsView::on_scene_changed
 * Updates the current view, if the scene_changed signal is received
 * \param scn
 */
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

/*!
 * \brief DropEnabledGraphicsView::dropEvent
 * Emits the dropped file path as a successfull_drop_image_file_event signal
 * Warns the user and returns if the dropped data doesn't contain a valid path
 * \param event
 */
void
DropEnabledGraphicsView::dropEvent(QDropEvent *event)
{   
    if(event->mimeData()->hasUrls()){
        QString drop_text = event->mimeData()->urls()[0].toString();
        //handle drop from file explorer
        if(valid_image_file(drop_text)){
            QString file_path = drop_text.remove(0, FILE_PREFIX);
            current_pixmap_.load(file_path);
            emit drag_and_drop(file_path);
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


/*!
 * \brief DropEnabledGraphicsView::valid_image_file
 * Helper function to check if the file_path is correct
 * \param file_path
 * \return
 */
bool
DropEnabledGraphicsView::valid_image_file(const QString &file_path) const
{
    return file_path.startsWith("file:///")
            && (file_path.endsWith("jpg")
                || file_path.endsWith("png")
                || file_path.endsWith("bmp"));
}

/*!
 * \brief DropEnabledGraphicsView::resizeEvent
 * Fits the contents of the scene to the graphics view
 * after resizing the window
 * \param event
 */
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
