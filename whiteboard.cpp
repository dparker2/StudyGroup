#include "whiteboard.h"
#include "whiteboard_p.h"
#include <QDebug>

Whiteboard::Whiteboard(QWidget *parent, QWidget* save_button) : QScrollArea(parent)
{
    this->setStyleSheet("background-color: #ababab");
    drawing_board = new my_whiteboard;
    drawing_board->resize(2000, 1000);
    this->save_button = save_button;
    saved = true;

    // Connections to send the drawing messages up the chain to the server object
    connect((my_whiteboard*)drawing_board, SIGNAL(line_drawn(QPoint,QPoint)), this, SIGNAL(line_drawn(QPoint,QPoint)));

    this->setWidget(drawing_board);
}

void Whiteboard::draw_line(QPoint &point1, QPoint &point2)
{
    if (saved) {
        saved = false;

    }
    dynamic_cast<my_whiteboard*>(drawing_board)->draw_line(point1, point2, false);
}

void Whiteboard::get_whiteboard(QString ip)
{
    qDebug() << "whiteboard.cpp emitting send_whiteboard";
    emit send_whiteboard(ip, dynamic_cast<my_whiteboard*>(drawing_board)->get_whiteboard());
}

void Whiteboard::update_whiteboard(QByteArray *wb_data)
{
    dynamic_cast<my_whiteboard*>(drawing_board)->update_whiteboard(wb_data);
}
