#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QWidget>
#include <QScrollArea>

#include "whiteboard.h"

namespace Ui {
class Whiteboard;
}

class Whiteboard_Wrapper : public QWidget
{
    Q_OBJECT
public:
    explicit Whiteboard_Wrapper(QString name, QWidget *parent = nullptr, QWidget* save_button = nullptr);
    ~Whiteboard_Wrapper();
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
    Ui::Whiteboard *ui;
    //QWidget* drawing_board;
    QWidget* save_button;
    bool saved;
};

#endif // WHITEBOARD_H
