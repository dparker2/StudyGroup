#include "whiteboard_wrapper.h"
#include "ui_whiteboard.h"
#include <QDebug>
#include <QCursor>

Whiteboard_Wrapper::Whiteboard_Wrapper(QString name, QWidget *parent, QWidget* save_button) :
    QWidget(parent),
    ui(new Ui::Whiteboard)
{
    ui->setupUi(this);

    ui->scrollArea->setStyleSheet("background-color: #ffffff");
    ui->scrollArea->setFrameShadow(QFrame::Plain);
    //ui->whiteboard->resize(2000, 1000);
    qDebug() << ui->scrollArea->widget();
    Whiteboard* wb = new Whiteboard(name);
    wb->setMinimumSize(2000, 1000);
    ui->scrollArea->setWidget(wb);
    qDebug() << ui->scrollArea->widget();
    //this->save_button = save_button;
    saved = true;

    // Connections to send the drawing messages up the chain to the server object
    //connect(dynamic_cast<Whiteboard*>(drawing_board), SIGNAL(line_drawn(QPoint,QPoint,QColor,int)), this, SIGNAL(line_drawn(QPoint,QPoint,QColor,int)));

    //this->setWidget(drawing_board);
}


Whiteboard_Wrapper::~Whiteboard_Wrapper()
{
    delete ui;
}

QByteArray* Whiteboard_Wrapper::whiteboard_ba()
{
    //return dynamic_cast<Whiteboard*>(drawing_board)->get_whiteboard();
}

void Whiteboard_Wrapper::draw_line(const QPoint &point1, const QPoint &point2, const QColor& pen_color, const int& pen_size)
{
    save_button->setEnabled(true);
    //dynamic_cast<Whiteboard*>(drawing_board)->draw_line(point1, point2, pen_color, pen_size);
}

void Whiteboard_Wrapper::get_whiteboard(QString ip)
{
    qDebug() << "whiteboard.cpp emitting send_whiteboard";
    //emit send_whiteboard(ip, dynamic_cast<Whiteboard*>(drawing_board)->get_whiteboard());
}

void Whiteboard_Wrapper::update_whiteboard(QByteArray *wb_data)
{
    qDebug() << "hello from whiteboard.cpp update_whiteboard";
    //dynamic_cast<Whiteboard*>(drawing_board)->update_whiteboard(wb_data);
}

QColor Whiteboard_Wrapper::get_pen_color()
{
    //return dynamic_cast<Whiteboard*>(drawing_board)->get_pen_color();
}

int Whiteboard_Wrapper::get_pen_size()
{
    //return dynamic_cast<Whiteboard*>(drawing_board)->get_pen_size();
}

void Whiteboard_Wrapper::set_pen_color(QColor color_arg)
{
    //dynamic_cast<Whiteboard*>(drawing_board)->set_pen_color(color_arg);
}

void Whiteboard_Wrapper::set_pen_size(int size_arg)
{
    //dynamic_cast<Whiteboard*>(drawing_board)->set_pen_size(size_arg);
}
