#include "fcfront.h"
#include "ui_fcfront.h"
#include "server.h"
#include <QDebug>

FCFront::FCFront(QString groupID, QString text, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFront)
{
    ui->setupUi(this);
    edit_front(text, index);
    ui->front_widget->setCurrentIndex(0);
}

FCFront::~FCFront()
{
    delete ui;
}

// Sets front label and text box with flashcard text
void FCFront::edit_front(QString text, int index)
{
    ui->front_text->setPlainText(text);
    ui->front_label->setText(text);
    server::send(server::FLASHCARD_SET_FRONT + QString::number(index) + " " + text);

    /*
    if(display_edit){
        display_edit_front();
    }
    else{
        display_front();
    }
    */

}

// ** PROBABLY NOT NEEDED **

// Displays front label
void FCFront::display_front()
{
    QString text = ui->front_text->toPlainText();
    ui->front_label->setText(text);
    ui->front_widget->setCurrentIndex(0);
}

// Displays the editing side of the front card
void FCFront::display_edit_front()
{
    QString text = ui->front_label->text();
    qDebug()<< text;
    ui->front_text->setPlainText(text);
    ui->front_widget->setCurrentIndex(1);
}

void FCFront::on_edit_card_btn_clicked()
{
    ui->front_widget->setCurrentIndex(1);          // get the current edited text and pass it to edit
}

void FCFront::on_set_front_btn_clicked()
{
    QString text = ui->front_text->toPlainText();   // get the current edited text and pass it to edit
    edit_front(text, 0);                 // going to need to pass the index somehow **LOOK AT ME**
    ui->front_widget->setCurrentIndex(0);
}
