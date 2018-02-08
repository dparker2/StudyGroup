#include "whiteboard.h"
#include <QDebug>
#include <QBuffer>

Whiteboard::Whiteboard(QString name, QWidget *parent) : SGWidget(name, parent)
{
    this->setCursor(QCursor(Qt::CrossCursor));
    image = QImage(2000, 1000, QImage::Format_ARGB32);
    image.fill(Qt::white);
    setAttribute(Qt::WA_StaticContents);
    update_timer.setInterval(30);
    update_timer.start();
    connect(&update_timer, SIGNAL(timeout()), this, SLOT(process_paints()));
    mouse_pos_queue = new QQueue<QPair<QPair<QPoint, QPoint>, QPair<QColor, int>>>;
    pen_color = QColor(0, 0, 0);
    pen_size = 5;
    image_changed = false;
    drawing = false;
    erasing = false;
    ruler_drawing = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    group_id = name.section(' ', 0, 0);
}

void Whiteboard::process_paints()
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

void Whiteboard::do_work()
{
    while(!_work_queue.isEmpty()) {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message, 2);
        if (message_list[0] == "WBLN") {
            WBLN(message_list[1]);
        }
        else if(message_list[0] == "NUWB") {
            NUWB(message_list[1]);
        }
        else if(message_list[0] == "WBUP") {
            WBUP(message_list[1]);
        }
    }
}

void Whiteboard::WBLN(QByteArray& line_info)
{
    QList<QByteArray> line_list = split(line_info, 6);
    QColor pen_color = QColor(QString(line_list[0]));
    int pen_size = line_list[1].toInt();
    QPoint point1(line_list[2].toInt(), line_list[3].toInt());
    QPoint point2(line_list[4].toInt(), line_list[5].toInt());
    draw_line(point1, point2, pen_color, pen_size);
}

void Whiteboard::NUWB(QByteArray& user_ip)
{
    server::send(server::WHITEBOARD_UPDATE + user_ip + ' ' + *get_whiteboard());
}

void Whiteboard::WBUP(QByteArray& whiteboard_ba)
{
    image = QImage::fromData(whiteboard_ba, "PNG"); // reads bytes into an image in PNG format
    update(image.rect());
    qDebug() << "Whiteboard is updated!";
}

void Whiteboard::draw_and_send(QPoint new_position, QColor color)
{
    color = (color == nullptr) ? pen_color : color;
    QPair<QPoint, QPoint> point_pair(prev_mouse_pos, new_position);
    QPair<QColor, int> pen_pair(color, pen_size);
    mouse_pos_queue->enqueue(qMakePair(point_pair, pen_pair));
    server::send(server::WHITEBOARD_DRAW +
                 group_id + ' ' +
                 color.name() + ' ' +
                 QString::number(pen_size) + ' ' +
                 QString::number(prev_mouse_pos.x()) + ' ' +
                 QString::number(prev_mouse_pos.y()) + ' ' +
                 QString::number(new_position.x()) + ' ' +
                 QString::number(new_position.y()));
}

void Whiteboard::draw_line(const QPoint& point1, const QPoint& point2, const QColor& pen_color_arg, const int& pen_size_arg)
{
    mouse_pos_queue->enqueue(qMakePair(qMakePair(point1, point2), qMakePair(pen_color_arg, pen_size_arg)));
}

QByteArray* Whiteboard::get_whiteboard()
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

void Whiteboard::update_whiteboard(QByteArray* wb_data)
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

void Whiteboard::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "clicky clicky";
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
            draw_and_send(event->pos());
            drawing = true;
        }
    }
    if(event->button() == Qt::RightButton)
    {
        prev_mouse_pos = event->pos();
        draw_and_send(event->pos(), QColor("#fff"));
        erasing = true;
    }
}

void Whiteboard::mouseMoveEvent(QMouseEvent *event)
{
    // If currently set to draw, draw a line from the previous position to here
    if(drawing)
    {
        draw_and_send(event->pos());
        prev_mouse_pos = event->pos();
    }
    if(erasing)
    {
        draw_and_send(event->pos(), QColor("#fff"));
        prev_mouse_pos = event->pos();
    }
}

void Whiteboard::mouseReleaseEvent(QMouseEvent *event)
{
    // If left buton is released and we previously were drawing
    if(event->button() == Qt::LeftButton && drawing)
    {
        drawing = false;
    }
    if(event->button() == Qt::LeftButton && ruler_drawing)
    {
        draw_and_send(event->pos());
        ruler_drawing = false;
    }
    if(event->button() == Qt::RightButton && erasing)
    {
        erasing = false;
    }
    update_timer.start();
}

// Overridden paintEvent for the widget
void Whiteboard::paintEvent(QPaintEvent *event)
{
    // This is called whenever update() is called. See the draw functions.
    // Make a painter object for the widget, and copy the part of the QImage that
    // was drawn on to the widget.
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// SHOULD NEVER BE CALLED
void Whiteboard::resizeEvent(QResizeEvent *event)
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
