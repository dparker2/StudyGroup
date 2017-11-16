#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QScrollArea>

class Whiteboard : public QScrollArea
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr, QWidget* save_button = nullptr);
    QByteArray* whiteboard_ba();

signals:
    void line_drawn(const QPoint&, const QPoint&);
    void send_whiteboard(QString&, QByteArray*);

public slots:
    void draw_line(QPoint& point1, QPoint& point2);
    void get_whiteboard(QString ip);
    void update_whiteboard(QByteArray* wb_data);

protected:

private:
    QWidget* drawing_board;
    QWidget* save_button;
    bool saved;
};

#endif // WHITEBOARD_H
