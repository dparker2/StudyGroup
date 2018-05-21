#include "whiteboard_wrapper.h"
#include "ui_whiteboard.h"
#include <QDebug>
#include <QCursor>

Whiteboard_Wrapper::Whiteboard_Wrapper(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Whiteboard)
{
    ui->setupUi(this);

    ui->scrollArea->setStyleSheet("background-color: #ffffff");
    ui->scrollArea->setFrameShadow(QFrame::Plain);
    whiteboard = new Whiteboard(name);
    connect(whiteboard, SIGNAL(line_drawn()), this, SLOT(whiteboard_changed()));
    whiteboard->set_pen_color(QColor("#000"));
    QString pen_size = ui->comboBox_pen_size->currentText();
    pen_size.chop(2);
    whiteboard->set_pen_size(pen_size.toInt());
    whiteboard->setMinimumSize(2000, 1000);
    ui->scrollArea->setWidget(whiteboard);
}


Whiteboard_Wrapper::~Whiteboard_Wrapper()
{
    delete ui;
}

void Whiteboard_Wrapper::on_comboBox_pen_color_currentTextChanged(const QString &pen_color)
{
    if(pen_color == "Black")
    {
        whiteboard->set_pen_color(QColor("#000"));
    }
    else if(pen_color == "Red")
    {
        whiteboard->set_pen_color(QColor("#f00"));
    }
    else if(pen_color == "Green")
    {
        whiteboard->set_pen_color(QColor("#0f0"));
    }
    else if(pen_color == "Blue")
    {
        whiteboard->set_pen_color(QColor("#00f"));
    }
}

void Whiteboard_Wrapper::on_save_whiteboard_button_released()
{
    qDebug() << "saving_whiteboard";
    QByteArray send_wb;
    send_wb += server::WHITEBOARD_SAVE + whiteboard->get_group_id() + ' ';
    send_wb += *whiteboard->get_whiteboard();
    qDebug() << send_wb;
    server::send(send_wb);
    ui->save_whiteboard_button->setEnabled(false);
}

void Whiteboard_Wrapper::on_comboBox_pen_size_currentTextChanged(const QString &pen_size)
{
    QString p_size = pen_size;
    p_size.chop(2);
    whiteboard->set_pen_size(p_size.toInt());
}

void Whiteboard_Wrapper::whiteboard_changed()
{
    ui->save_whiteboard_button->setEnabled(true);
}
