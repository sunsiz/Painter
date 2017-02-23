#include "painter.h"
#include "ui_painter.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>

Painter::Painter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Painter)
{
    ui->setupUi(this);
    line1=QLine(10,10,90,90);
    rect1=QRect(10,20,100,100);
    shape=Polygon;
    antialiased=false;
    transformed=false;
    drawing=false;
    modefied=false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    pen.setWidth(3);
    setMouseTracking(true);
    drawingMode="Any";
    //    setAttribute(Qt::WA_StaticContents);
}

Painter::~Painter()
{
    delete ui;
}

void Painter::drawLine()
{
    QPainter painter(this);
    painter.drawLine(line1);
}

void Painter::drawRectangle(QPoint endPoint)
{
    QPainter painter(&image);
    painter.setPen(pen);
    painter.drawRoundRect(QRect(lastone,endPoint));
    update();
    modefied=true;
}

void Painter::setShape(Shape shape)
{
    this->shape=shape;
    update();
}

void Painter::setPen(const QPen &pen)
{
    this->pen=pen;
    update();
}

void Painter::setBrush(const QBrush &brush)
{
    this->brush=brush;
    update();
}

void Painter::setAntialiased(bool antialiased)
{
    this->antialiased=antialiased;
    update();
}

void Painter::setTransformed(bool transformed)
{
    this->transformed=transformed;
    update();
}

void Painter::paintEvent(QPaintEvent *event)
{
    //    static const QPoint points[4] = {
    //        QPoint(10,80),
    //        QPoint(20,100),
    //        QPoint(10,30),
    //        QPoint(90,70)
    //    };

    //    QRect rect(30,20,180,60);

    //    QPainterPath path;
    //    path.moveTo(20,80);
    //    path.lineTo(20,30);


    //    QPainter painter(this);
    //    painter.setPen(pen);
    //    painter.setBrush(brush);
    //    painter.drawPoints(points,4);
    //    painter.setPen(palette().dark().color());
    //    painter.setBrush(Qt::NoBrush);
    //    //painter.drawRect(QRect(0,0,width()-1,height()-1));
    //    painter.drawRect(rect);
    //    QPainter painter(this);
    //        painter.setPen(Qt::blue);
    //        painter.setFont(QFont("Arial", 30));
    //        painter.drawText(rect(), Qt::AlignCenter, "Qt");
    //        QLineF line(10.0, 80.0, 90.0, 20.0);

    ////        QPainter(this);
    //        painter.drawLine(line);
    //        QFont font = painter.font();
    //        font.setPixelSize(48);
    //        painter.setFont(font);

    //        const QRect rectangle = QRect(90, 90, 150, 50);
    //        QRect boundingRect;
    //        painter.drawText(rectangle, 0, tr("Hello"), &boundingRect);

    //        QPen pen = painter.pen();
    //        pen.setStyle(Qt::DotLine);
    //        painter.setPen(pen);
    //        painter.drawRect(boundingRect.adjusted(0, 0, -pen.width(), -pen.width()));

    //        pen.setStyle(Qt::DashLine);
    //        painter.setPen(pen);
    //        painter.drawRect(rectangle.adjusted(0, 0, -pen.width(), -pen.width()));
    QPainter painter(this);
    QRect dirtyRect=event->rect();
    painter.drawImage(dirtyRect,image,dirtyRect);
}

void Painter::resizeEvent(QResizeEvent *event)
{
    if(width()>image.width() || height()>image.height())
    {
        int newWidth=qMax(width()+128,image.width());
        int newHeight=qMax(height()+128,image.height());
        resizeImage(&image,QSize(newWidth,newHeight));
        update();
    }
    ui->groupBox->setGeometry(width()-122,0,120,240);
    QWidget::resizeEvent(event);
}

void Painter::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size()==newSize)return;
    QImage newImage(newSize,QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0),*image);
    *image=newImage;
}

void Painter::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(pen);
    painter.setRenderHint(painter.Antialiasing,true);
    //    painter.setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap,
    //                        Qt::RoundJoin));
    //    pen.setWidth(30);
    //    pen.setColor(Qt::black);
    //    painter.setPen(pen);
    int rad = (1 / 2) + 2;
    update(QRect(lastone, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    painter.drawLine(lastone,endPoint);
    //    pen.setWidth(7);
    //    pen.setColor(Qt::red);
    //    painter.setPen(pen);
    //    painter.drawPoint(lastone);
    //    painter.drawPoint(endPoint);

    lastone=endPoint;
    modefied=true;
}

void Painter::drawPointTo(const QPoint endPoint)
{
    QPainter painter(&image);
    //    pen.setWidth(9);
    painter.setPen(pen);
    painter.setRenderHint(painter.Antialiasing,true);
    painter.drawPoint(endPoint);
    update();
    modefied=true;
}

void Painter::drawTails(const QPoint currentPoint)
{
    QPainter painter(&image);
    painter.setPen(pen);
    painter.setRenderHint(painter.Antialiasing,true);
    painter.drawPoint(currentPoint);
    QVector<QPoint> points;
    for(int i=0; i<10; i++)
    {
        points.append(QPoint(currentPoint.x()+qrand()%10,currentPoint.y()+qrand()%10));
        points<<QPoint(currentPoint.x()-qrand()%10,currentPoint.y()-qrand()%10);
    }
    painter.drawPoints(points);
    update();
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastone=event->pos();
        drawing=true;
    }
    //    QPoint point=event->pos();
    //    this->repaint();
    //    QPainter painter(this);
    //    pen.setWidth(30);
    //    painter.setPen(pen);
    //    brush.setColor("green");
    //    painter.setBrush(brush);
    //    painter.drawPoint(point);
    //    QWidget* point=new QWidget(this);
    //    if(currentZone)
    //    {point->setStyleSheet("background-color:red; color:red;");}
    //    else
    //    {point->setStyleSheet("background-color:blue; color:blue;");}
    //    point->resize(5,5);
    //    point->move(event->pos());
    //    point->show();
    //    point->setEnabled(0);
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && drawing)
    {
        if(drawingMode=="Any")
            drawLineTo(event->pos());
    }
    if(ui->brushCheckBox->isChecked())
    {
        drawTails(event->pos());
//        drawTails(QCursor::pos());
    }
    statusBar()->showMessage(QString("(x,y) coordinates: (%1,%2)").arg(event->x()).arg(event->y()));
}

void Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && drawing)
    {
        if(drawingMode=="Line")
            drawLineTo(event->pos());
        if(drawingMode=="Point")
            drawPointTo(event->pos());
        if(drawingMode=="Rectangle")
            drawRectangle(event->pos());
        drawing=false;
    }
}

void Painter::mouseGrabber(QMouseEvent *event)
{
    if(event->button()==Qt::NoButton)
    {
        drawTails(event->pos());
        drawTails(QCursor::pos());
    }
}

void Painter::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()=="Line")
        drawingMode="Line";
    if(item->text()=="Point")
        drawingMode="Point";
    if(item->text()=="Rectangle")
        drawingMode="Rectangle";
    if(item->text()=="Any")
        drawingMode="Any";
}

//void Painter::drawingModeChanged()
//{
//    switch(drawingMode)
//    case "Line":
//    this->setShape(Painter::Shape(0));
//        break;
//    case "Point":
//        this->setShape(Painter::Shape(1));
//}

bool Painter::openImage(QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName))
        return false;
    QSize newSize=loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage,newSize);
    image=loadedImage;
    modefied=false;
    update();
    return true;
}

bool Painter::openImages(QStringList &fileNames)
{
    QImage loadedImage;
    int columns=4;
    int rows=fileNames.count()/columns+1;
    int columnWidth=width()/columns;
    int rowHeight=qMin(height()/rows,200);
    int counter=0;
    int rowCounter=0;
    QPainter painter(&image);
    painter.setPen(pen);
    QSize newImageSize=QSize(columnWidth,rowHeight);
    foreach (QString filename, fileNames)
    {
        if(!loadedImage.load(filename))
            return false;
//        QSize newSize=loadedImage.size().expandedTo(newImageSize);
//        resizeImage(&loadedImage,newImageSize);/*newSize*/
        loadedImage.scaled(newImageSize,Qt::KeepAspectRatio,Qt::FastTransformation);
        painter.drawImage(QRect(counter*columnWidth,rowCounter*rowHeight,columnWidth,rowHeight),loadedImage);
//        image+=loadedImage;
        modefied=false;
        update();
        counter++;
        if(counter==columns)
        {
            counter=0;
            rowCounter+=1;
        }
    }
    return true;
}

bool Painter::saveImage(QString &fileName, const char *fileFormat)
{
    QImage visibleImage;
    resizeImage(&visibleImage,size());

    if(visibleImage.save(fileName,fileFormat))
    {
        modefied=false;
        return true;
    }
    else {
        return false;
    }
}

void Painter::on_horizontalSlider_valueChanged(int value)
{
    pen.setWidth(value);
}

void Painter::on_toolButton_clicked()
{
    QColor newColor=QColorDialog::getColor(pen.color());
    if(newColor.isValid())
        pen.setColor(newColor);
}

void Painter::on_actionOpen_triggered()
{
    if(!modefied)
    {
        QString fileName=QFileDialog::getOpenFileName(this,tr("Open an image"),QDir::homePath());
        if(!fileName.isEmpty())
        {
            Painter::openImage(fileName);
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The image has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            saveFile("png");
            break;
        case QMessageBox::Discard:
            modefied=false;
            on_actionOpen_triggered();
            break;
        case QMessageBox::Cancel:
            return;
            break;
        default:
            // should never be reached
            break;
        }
    }
}

void Painter::on_actionExit_triggered()
{
    if(modefied)
    {
        QMessageBox msgBox;
        msgBox.setText("The image has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            saveFile("png");
            break;
        case QMessageBox::Discard:
            QApplication::exit();
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
    }
    else
    {
        QApplication::exit();
    }
}

void Painter::on_actionSave_triggered()
{
    QAction *action=qobject_cast<QAction *>(sender());
    QByteArray fileFormat=action->data().toByteArray();
    saveFile(fileFormat);
}

bool Painter::saveFile(const QByteArray &fileFormat)
{
    QString initialPath=QDir::currentPath()+"/untitled."+fileFormat;

    QString fileName=QFileDialog::getSaveFileName(this,tr("Save image to file"),
                                                  initialPath,
                                                  tr("%1 Files (*.%2);;All Files (*)")
                                                  .arg(QString::fromLatin1(fileFormat).toUpper())
                                                  .arg(QString::fromLatin1(fileFormat)));
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return Painter::saveImage(fileName,fileFormat.constData());
    }
}

void Painter::clearImage()
{
    image.fill(qRgb(255,255,255));
    update();
    modefied=true;
}

void Painter::on_actionNew_triggered()
{
    if(!modefied)
    {
        clearImage();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(tr("the image has been modefied."));
        msgBox.setInformativeText("do you want to save changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret=msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:
            if(saveFile("png"))modefied=false;
            break;
        case QMessageBox::Discard:
            clearImage();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}

void Painter::on_actionChooseColor_triggered()
{
    on_toolButton_clicked();
}

void Painter::on_actionSave_As_triggered()
{
    if(saveFile("png"))modefied=false;
}

void Painter::on_actionPenSize_triggered()
{

}

void Painter::on_actionStitchPhotos_triggered()
{
    QStringList fileNames=QFileDialog::getOpenFileNames(this,"Please select the files you want to stitch together.",QDir::homePath());
    if(!fileNames.isEmpty())
    {
        openImages(fileNames);
    }
}

void Painter::on_actionHideSettings_triggered()
{
    if(ui->actionHideSettings->text()=="Hide Settings")
    {
        ui->groupBox->setVisible(false);
        ui->actionHideSettings->setText(tr("Show Settings"));
    }
    else {
        ui->groupBox->setVisible(true);
        ui->actionHideSettings->setText(tr("Hide Settings"));
    }
}
