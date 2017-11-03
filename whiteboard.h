#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QScrollArea>

class Whiteboard : public QScrollArea
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr);

signals:

public slots:

protected:

private:
    QWidget* drawing_board;
};

#endif // WHITEBOARD_H
