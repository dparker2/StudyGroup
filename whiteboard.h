#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QWidget>
#include <QPainter>

class Whiteboard : public QWidget
{
    Q_OBJECT
public:
    explicit Whiteboard(QWidget *parent = nullptr);
    void test_call();

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void draw_line(const QPoint& final_mouse_pos);
    void move_image(const QPoint& final_mouse_pos);
    QPoint prev_mouse_pos;
    QImage image;
    bool drawing;
    bool moving;
};

#endif // WHITEBOARD_H
