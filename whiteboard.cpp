#include "whiteboard.h"
#include "whiteboard_p.h"
#include <QDebug>
#include <QCursor>

Whiteboard::Whiteboard(QWidget *parent, QWidget* save_button) : QScrollArea(parent)
{
    this->setStyleSheet("background-color: #ffffff");
    this->setFrameShadow(QFrame::Plain);
    this->setCursor(QCursor(Qt::CrossCursor));
    drawing_board = new my_whiteboard;
    drawing_board->resize(2000, 1000);
    this->save_button = save_button;
    saved = true;

    // Connections to send the drawing messages up the chain to the server object
    connect(dynamic_cast<my_whiteboard*>(drawing_board), SIGNAL(line_drawn(QPoint,QPoint,QColor,int)), this, SIGNAL(line_drawn(QPoint,QPoint,QColor,int)));

    this->setWidget(drawing_board);
}

QByteArray* Whiteboard::whiteboard_ba()
{
    return dynamic_cast<my_whiteboard*>(drawing_board)->get_whiteboard();
}

void Whiteboard::draw_line(const QPoint &point1, const QPoint &point2, const QColor& pen_color, const int& pen_size)
{
    save_button->setEnabled(true);
    dynamic_cast<my_whiteboard*>(drawing_board)->draw_line(point1, point2, pen_color, pen_size);
}

void Whiteboard::get_whiteboard(QString ip)
{
    qDebug() << "whiteboard.cpp emitting send_whiteboard";
    emit send_whiteboard(ip, dynamic_cast<my_whiteboard*>(drawing_board)->get_whiteboard());
}

void Whiteboard::update_whiteboard(QByteArray *wb_data)
{
    qDebug() << "hello from whiteboard.cpp update_whiteboard";
    dynamic_cast<my_whiteboard*>(drawing_board)->update_whiteboard(wb_data);
}

QColor Whiteboard::get_pen_color()
{
    return dynamic_cast<my_whiteboard*>(drawing_board)->get_pen_color();
}

int Whiteboard::get_pen_size()
{
    return dynamic_cast<my_whiteboard*>(drawing_board)->get_pen_size();
}

void Whiteboard::set_pen_color(QColor color_arg)
{
    dynamic_cast<my_whiteboard*>(drawing_board)->set_pen_color(color_arg);
}

void Whiteboard::set_pen_size(int size_arg)
{
    dynamic_cast<my_whiteboard*>(drawing_board)->set_pen_size(size_arg);
}
