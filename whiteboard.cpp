#include "whiteboard.h"
#include <QMouseEvent>
#include <QDebug>

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

my_whiteboard::my_whiteboard(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    drawing = false;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void my_whiteboard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        prev_mouse_pos = event->pos();
        drawing = true;
    }
}

void my_whiteboard::mouseMoveEvent(QMouseEvent *event)
{
    if(drawing)
    {
        draw_line(event->pos());
    }
}

void my_whiteboard::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && drawing)
    {
        draw_line(event->pos());
        drawing = false;
    }
}

void my_whiteboard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void my_whiteboard::resizeEvent(QResizeEvent *event)
{
    QImage old_image = image;
    image = QImage(this->width(), this->height(), QImage::Format_RGB32);
    image.fill(QColor(255, 255, 255));
    QPainter copy_image_painter(&image);
    copy_image_painter.drawImage(QPoint(0,0), old_image);
    QWidget::resizeEvent(event);
}

void my_whiteboard::draw_line(const QPoint& final_mouse_pos)
{
    QPainter painter(&image);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(prev_mouse_pos, final_mouse_pos);
    update(QRect(prev_mouse_pos, final_mouse_pos).normalized().adjusted(-7, -7, 7, 7));
    prev_mouse_pos = final_mouse_pos;
}

/*******************
 *
 *
 *
 *  WHITEBOARD CLASS
 *
 *
 *
 */
Whiteboard::Whiteboard(QWidget *parent) : QScrollArea(parent)
{
    this->setStyleSheet("background-color: #ababab");
    drawing_board = new my_whiteboard;
    drawing_board->resize(2000, 1000);
    this->setWidget(drawing_board);
}
