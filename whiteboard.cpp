#include "whiteboard.h"
#include "whiteboard_p.h"
#include <QDebug>

Whiteboard::Whiteboard(QWidget *parent) : QScrollArea(parent)
{
    this->setStyleSheet("background-color: #ababab");
    drawing_board = new my_whiteboard;
    drawing_board->resize(2000, 1000);
    this->setWidget(drawing_board);
}
