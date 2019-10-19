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
    QGraphicsScene current_scene_;
    QPixmap current_pixmap_;
    Q_OBJECT
public:
    DropEnabledGraphicsView(QWidget* parent);

public slots:
    void on_scene_changed(QGraphicsScene* scn);

signals:
    void successfull_drop_image_data_event(const QVariant& data);
    void drag_and_drop(const QString& path);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    bool valid_image_file(const QString& file_path) const;
};

#endif // DROPENABLEDGRAPHICSVIEW_H
