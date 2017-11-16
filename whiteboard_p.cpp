#include "whiteboard_p.h"
#include <QDebug>
#include <QBuffer>

my_whiteboard::my_whiteboard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    drawing = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void my_whiteboard::draw_line(const QPoint& point1, const QPoint& point2, bool from_here)
{
    // emit a signal if this function was called from our own mouse events.
    // This is captured by the server and sent to all other clients for drawing.
    if(from_here) {
        emit line_drawn(point1, point2);
    }
    // Create a painter for the image, set the pen settings, and draw onto the image.
    // The update function forces a paintEvent, passing the rectangle around the draw
    // allows the paint event to not have to copy the entire image over.
    QPainter painter(&image);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (point1 == point2) {
        painter.drawPoint(point1);
    } else {
        painter.drawLine(point1, point2);
    }
    update(QRect(point1, point2).normalized().adjusted(-7, -7, 7, 7));
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
        // Store the location of the click, initalize a line draw and also draw a point
        prev_mouse_pos = event->pos();
        drawing = true;
    }
}

void my_whiteboard::mouseMoveEvent(QMouseEvent *event)
{
    // If currently set to draw, draw a line from the previous position to here
    if(drawing)
    {
        draw_line(prev_mouse_pos, event->pos());
        prev_mouse_pos = event->pos();
    }
}

void my_whiteboard::mouseReleaseEvent(QMouseEvent *event)
{
    // If left buton is released and we previously were drawing
    if(event->button() == Qt::LeftButton && drawing)
    {
        // Draw the final line to this point, and reset drawing to false.
        draw_line(prev_mouse_pos, event->pos());
        drawing = false;
    }
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
