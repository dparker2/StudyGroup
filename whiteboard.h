#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QScrollArea>

class Whiteboard : public QScrollArea
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr);

signals:
    void line_drawn(const QPoint&, const QPoint&);

public slots:
    void draw_line(const QPoint& point1, const QPoint& point2);

protected:

private:
    QWidget* drawing_board;
};

#endif // WHITEBOARD_H
