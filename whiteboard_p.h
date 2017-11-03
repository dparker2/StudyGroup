#ifndef MY_WHITEBOARD_H
#define MY_WHITEBOARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class my_whiteboard : public QWidget
{
public:
    explicit my_whiteboard(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void draw_line(const QPoint& final_mouse_pos);
    QPoint prev_mouse_pos;
    QImage image;
    bool drawing;
    bool moving;
};

#endif // MY_WHITEBOARD_H
