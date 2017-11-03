#ifndef MY_WHITEBOARD_H
#define MY_WHITEBOARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class my_whiteboard : public QWidget
{
    Q_OBJECT
public:
    explicit my_whiteboard(QWidget* parent = nullptr);
    void draw_line(const QPoint& point1, const QPoint& point2, bool from_here = true);

signals:
    void line_drawn(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QPoint prev_mouse_pos;
    QImage image;
    bool drawing;
    bool moving;
};

#endif // MY_WHITEBOARD_H
