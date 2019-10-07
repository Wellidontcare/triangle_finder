#ifndef DROPENABLEDGRAPHICSVIEW_H
#define DROPENABLEDGRAPHICSVIEW_H

#ifdef WIN32
constexpr int FILE_PREFIX = 8;
#else
constexpr int FILE_PREFIX = 7;
#endif

#include <QDebug>

#include <QGraphicsView>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QMessageBox>

class DropEnabledGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    DropEnabledGraphicsView(QWidget* parent);

public slots:
    void update_scene_action(QGraphicsScene* scn);
    void dropEvent(QDropEvent *event) override;

signals:
    void successfull_drop_image_data_event(const QVariant& data);
    void successfull_drop_image_file_event(const QString& path);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;

private:
    QGraphicsScene current_scene_;
    QPixmap current_pixmap_;
};

#endif // DROPENABLEDGRAPHICSVIEW_H
