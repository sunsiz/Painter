#ifndef PAINTER_H
#define PAINTER_H

#include <QMainWindow>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QMouseEvent>
#include <QListWidget>

namespace Ui {
class Painter;
}

class Painter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = 0);
    ~Painter();
    enum Shape { Line, Point, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                       Chord, Pie, Path, Text, Pixmap };
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void resizeImage(QImage *image, const QSize &newSize);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseGrabber(QMouseEvent *event);

private slots:
    void drawLine();
    void drawRectangle(QPoint endPoint);
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    void drawLineTo(const QPoint &endPoint);
    void drawPointTo(const QPoint endPoint);
    void drawTails(const QPoint currentPoint);
    bool openImage(QString &fileName);
    bool saveImage(QString &fileName, const char* fileFormat);
    bool saveFile(const QByteArray &fileFormat);
    void clearImage();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_horizontalSlider_valueChanged(int value);

    void on_toolButton_clicked();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionChooseColor_triggered();

    void on_actionSave_As_triggered();

private:
    Ui::Painter *ui;
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QLine line1;
    QRect rect1;
    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    bool drawing;
    bool modefied;
    QPixmap pixmap;
    QPoint lastone;
    QImage image;
    QString drawingMode;
};

#endif // PAINTER_H
