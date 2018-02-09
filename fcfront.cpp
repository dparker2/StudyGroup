#include "fcfront.h"
#include "ui_fcfront.h"
#include <QDebug>

FCFront::FCFront(QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFront)
{
    ui->setupUi(this);
    edit_front(text, false);
}

FCFront::~FCFront()
{
    delete ui;
}

// Sets front label and text box with flashcard text
void FCFront::edit_front(QString text, bool display_edit)
{
    ui->front_text->setPlainText(text);
    ui->front_label->setText(text);

    if(display_edit){
        display_edit_front();
    }
    else{
        display_front();
    }
}
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
