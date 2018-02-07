#include "fcfront.h"
#include "ui_fcfront.h"

FCFront::FCFront(QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFront)
{
    ui->setupUi(this);
    edit_front(text);
}

FCFront::~FCFront()
{
    delete ui;
}

// Sets front label and text box with flashcard text
void FCFront::edit_front(QString text){
    ui->front_text->setPlainText(text);
    ui->front_label->setText(text);
}
// Displays front label
void FCFront::display_front(){
    ui->front_widget->setCurrentIndex(0);
}
// Displays the editing side of the front card
void FCFront::display_edit_front(){
    ui->front_widget->setCurrentIndex(1);
}
