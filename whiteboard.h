#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QScrollArea>

class Whiteboard : public QScrollArea
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr, QWidget* save_button = nullptr);
    QByteArray* whiteboard_ba();
    QColor get_pen_color();
    int get_pen_size();
    void set_pen_color(QColor color_arg);
    void set_pen_size(int size_arg);

signals:
    void line_drawn(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);
    void send_whiteboard(QString&, QByteArray*);

public slots:
    void draw_line(const QPoint& point1, const QPoint& point2, const QColor& pen_color, const int& pen_size);
    void get_whiteboard(QString ip);
    void update_whiteboard(QByteArray* wb_data);

protected:

private:
    QWidget* drawing_board;
    QWidget* save_button;
    bool saved;
};

#endif // WHITEBOARD_H
