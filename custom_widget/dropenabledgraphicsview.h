#ifndef DROPENABLEDGRAPHICSVIEW_H
#define DROPENABLEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QMimeData>

#ifdef WIN32
constexpr int FILE_PREFIX = 8;
#else
constexpr int FILE_PREFIX = 7;
#endif

class DropEnabledGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    DropEnabledGraphicsView(QWidget* parent);

public slots:
    void update_scene_action(QGraphicsScene* scn);

signals:
    void successfull_drop_image_data_event(const QVariant& data);
    void successfull_drop_image_file_event(const QString& path);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGraphicsScene current_scene_;
    QPixmap current_pixmap_;
};

#endif // DROPENABLEDGRAPHICSVIEW_H
