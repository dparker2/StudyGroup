#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QScrollArea>
#include <QPainter>

class my_whiteboard;

class Whiteboard : public QScrollArea
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr);

signals:

public slots:

protected:

private:
    my_whiteboard* drawing_board;
};

#endif // WHITEBOARD_H
