#include "whiteboard_p.h"
#include <QDebug>
#include <QBuffer>

my_whiteboard::my_whiteboard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    update_timer.setInterval(30);
    update_timer.start();
    connect(&update_timer, SIGNAL(timeout()), this, SLOT(process_paints()));
    mouse_pos_queue = new QQueue<QPair<QPair<QPoint, QPoint>, QPair<QColor, int>>>;
    image_changed = false;
    drawing = false;
    erasing = false;
    ruler_drawing = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void my_whiteboard::process_paints()
{
    QPainter painter(&image);
    while(!mouse_pos_queue->isEmpty())
    {
        QPair<QPair<QPoint, QPoint>, QPair<QColor, int>> paint_info = mouse_pos_queue->dequeue();
        QPoint point1 = paint_info.first.first;
        QPoint point2 = paint_info.first.second;
        QColor pen_color = paint_info.second.first;
        int pen_size = paint_info.second.second;
        // Create a painter for the image, set the pen settings, and draw onto the image.
        // The update function forces a paintEvent, passing the rectangle around the draw
        // allows the paint event to not have to copy the entire image over.
        painter.setPen(QPen(QBrush(pen_color), pen_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        if (point1 == point2) {
            painter.drawPoint(point1);
        } else {
            painter.drawLine(point1, point2);
        }
        update(QRect(point1, point2).normalized().adjusted(-7, -7, 7, 7));
    }
}

void my_whiteboard::draw_line(const QPoint& point1, const QPoint& point2, const QColor& pen_color_arg, const int& pen_size_arg)
{
    mouse_pos_queue->enqueue(qMakePair(qMakePair(point1, point2), qMakePair(pen_color_arg, pen_size_arg)));
}

QByteArray* my_whiteboard::get_whiteboard()
{
    qDebug() << "Getting whiteboard;";
    // Here because server has requested a copy of the whiteboard, in the form of a string
    QByteArray* image_bytes = new QByteArray();
    QBuffer buffer(image_bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); // writes image into image_bytes in PNG format
    qDebug() << image_bytes->size();
    qDebug() << "Whiteboard & " << image_bytes;
    return image_bytes;
}

void my_whiteboard::update_whiteboard(QByteArray* wb_data)
{
    image = QImage::fromData(*wb_data, "PNG"); // reads bytes into an image in PNG format
    update(image.rect());
    qDebug() << "Whiteboard is updated!";
}

/*****
 *
 * PROTECTED
 *
 */

void my_whiteboard::mousePressEvent(QMouseEvent *event)
{
    // Capture left mouse button click
    if(event->button() == Qt::LeftButton)
    {
        prev_mouse_pos = event->pos();
        // Store the location of the click, initalize a line draw and also draw a point
        if(event->modifiers() == Qt::CTRL)
        {
            ruler_drawing = true;
        }
        else {
            QPair<QPoint, QPoint> point_pair(prev_mouse_pos, event->pos());
            QPair<QColor, int> pen_pair(pen_color, pen_size);
            mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
            emit line_drawn(prev_mouse_pos, event->pos(), pen_color, pen_size);
            drawing = true;
        }
    }
    if(event->button() == Qt::RightButton)
    {
        prev_mouse_pos = event->pos();
        QPair<QPoint, QPoint> point_pair(prev_mouse_pos, event->pos());
        QPair<QColor, int> pen_pair(QColor("#fff"), pen_size);
        mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
        emit line_drawn(prev_mouse_pos, event->pos(), pen_color, pen_size);
        erasing = true;
    }
}

void my_whiteboard::mouseMoveEvent(QMouseEvent *event)
{
    // If currently set to draw, draw a line from the previous position to here
    if(drawing)
    {
        QPair<QPoint, QPoint> point_pair(prev_mouse_pos, event->pos());
        QPair<QColor, int> pen_pair(pen_color, pen_size);
        mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
        emit line_drawn(prev_mouse_pos, event->pos(), pen_color, pen_size);
        prev_mouse_pos = event->pos();
    }
    if(erasing)
    {
        QPair<QPoint, QPoint> point_pair(prev_mouse_pos, event->pos());
        QPair<QColor, int> pen_pair(QColor("#fff"), pen_size);
        mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
        emit line_drawn(prev_mouse_pos, event->pos(), pen_color, pen_size);
        prev_mouse_pos = event->pos();
    }
}

void my_whiteboard::mouseReleaseEvent(QMouseEvent *event)
{
    // If left buton is released and we previously were drawing
    if(event->button() == Qt::LeftButton && drawing)
    {
        drawing = false;
    }
    if(event->button() == Qt::LeftButton && ruler_drawing)
    {
        QPair<QPoint, QPoint> point_pair(prev_mouse_pos, event->pos());
        QPair<QColor, int> pen_pair(pen_color, pen_size);
        mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
        emit line_drawn(prev_mouse_pos, event->pos(), pen_color, pen_size);
        ruler_drawing = false;
    }
    if(event->button() == Qt::RightButton && erasing)
    {
        erasing = false;
    }
    update_timer.start();
}

// Overridden paintEvent for the widget
void my_whiteboard::paintEvent(QPaintEvent *event)
{
    // This is called whenever update() is called. See the draw functions.
    // Make a painter object for the widget, and copy the part of the QImage that
    // was drawn on to the widget.
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// SHOULD NEVER BE CALLED
void my_whiteboard::resizeEvent(QResizeEvent *event)
{
    // CURRENTLY NOT USED BECAUSE OF THE QSCROLLAREA IMPLEMENTATION
    // This allows resizing of the widget while maintaining the drawings.
    // Because we maintain a fixed QScrollArea to store this widget, it
    // should never be called though.
    QImage old_image = image;
    image = QImage(this->width(), this->height(), QImage::Format_RGB32);
    image.fill(QColor(255, 255, 255));
    QPainter copy_image_painter(&image);
    copy_image_painter.drawImage(QPoint(0,0), old_image);
    QWidget::resizeEvent(event);
}

/******
 *
 * PRIVATE
 *
 */
