#include "whiteboard.h"
#include <QMouseEvent>
#include <QDebug>

Whiteboard::Whiteboard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    drawing = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void Whiteboard::test_call()
{
    qDebug() << "success";
}

void Whiteboard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        prev_mouse_pos = event->pos();
        drawing = true;
    }
    if(event->button() == Qt::RightButton)
    {
        prev_mouse_pos = event->pos();
        moving = true;
    }
}

void Whiteboard::mouseMoveEvent(QMouseEvent *event)
{
    if(drawing)
    {
        draw_line(event->pos());
    }
    if(moving)
    {
        move_image(event->pos());
    }
}

void Whiteboard::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && drawing)
    {
        draw_line(event->pos());
        drawing = false;
    }
}

void Whiteboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void Whiteboard::resizeEvent(QResizeEvent *event)
{
    QImage old_image = image;
    image = QImage(this->width(), this->height(), QImage::Format_RGB32);
    image.fill(QColor(255, 255, 255));
    QPainter copy_image_painter(&image);
    copy_image_painter.drawImage(QPoint(0,0), old_image);
    QWidget::resizeEvent(event);
}

void Whiteboard::draw_line(const QPoint& final_mouse_pos)
{
    QPainter painter(&image);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(prev_mouse_pos, final_mouse_pos);
    update(QRect(prev_mouse_pos, final_mouse_pos).normalized().adjusted(-7, -7, 7, 7));
    prev_mouse_pos = final_mouse_pos;
}

void Whiteboard::move_image(const QPoint &final_mouse_pos)
{
    // TODO: Make a bigger image to always pull from and figure out how to move it around and stuff....
}
