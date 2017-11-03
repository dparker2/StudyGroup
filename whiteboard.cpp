#include "whiteboard.h"
#include "whiteboard_p.h"
#include <QDebug>

Whiteboard::Whiteboard(QWidget *parent) : QScrollArea(parent)
{
    this->setStyleSheet("background-color: #ababab");
    drawing_board = new my_whiteboard;
    drawing_board->resize(2000, 1000);

    // Connections to send the drawing messages up the chain to the server object
    connect((my_whiteboard*)drawing_board, SIGNAL(line_drawn(QPoint,QPoint)), this, SIGNAL(line_drawn(QPoint,QPoint)));

    this->setWidget(drawing_board);
}

void Whiteboard::draw_line(const QPoint &point1, const QPoint &point2)
{
    dynamic_cast<my_whiteboard*>(drawing_board)->draw_line(point1, point2, false);
}
