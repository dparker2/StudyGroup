#include "fcfront.h"
#include "ui_fcfront.h"
#include "server.h"
#include <QDebug>

FCFront::FCFront(QString group_name, QString text, int index, bool send_card, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFront)
{
    ui->setupUi(this);

    groupID = group_name;
    edit_front(text, index, send_card);
    ui->front_widget->setCurrentIndex(0);

    qDebug() << ui->front_label->sizeHint();

    current_index =  index;
}

FCFront::~FCFront()
{
    delete ui;
}

/*
 *  Sets front label and text box with flashcard text
 *  if send_card is set, card info is sent to server
 *  otherwise only the ui is updated
***/
void FCFront::edit_front(QString text, int index, bool send_card)
{
    qDebug() << "SENDING-> " + server::FLASHCARD_SET_FRONT + groupID + " " + QString::number(index) + " " + text;

    if(send_card)
    {
        server::send(server::FLASHCARD_SET_FRONT + groupID + " " + QString::number(index) + " " + text);
    }
    ui->front_text->setPlainText(text);    // Sets ui with text
    ui->front_label->setText(text);
}

// Displays front label
void FCFront::display_front()
{
    QString text = ui->front_text->toPlainText();
    ui->front_label->setText(text);
    ui->front_widget->setCurrentIndex(0);
}

/*
 *  Displays the editing side of the front card
***/
void FCFront::display_edit_front()
{
    QString text = ui->front_label->text();
    ui->front_text->setPlainText(text);
    ui->front_widget->setCurrentIndex(1);
}
void FCFront::save_front()
{
    QString text = ui->front_text->toPlainText();    // get the current edited text and pass it to edit
    edit_front(text, current_index, true);
    ui->front_widget->setCurrentIndex(0);
}
void FCFront::set_index(int index){
    current_index = index;
}

