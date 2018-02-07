#include "fcback.h"
#include "ui_fcback.h"

FCBack::FCBack(QString, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCBack)
{
    ui->setupUi(this);
}

FCBack::~FCBack()
{
    delete ui;
}

void FCBack::edit_back(QString text){
    ui->back_text->setPlainText(text);
    ui->back_label->setText(text);
}
// Displays back label
void FCBack::display_back(){
    ui->back_widget->setCurrentIndex(0);
}
// Displays the editing side of the back card
void FCBack::display_edit_back(){
    ui->back_widget->setCurrentIndex(1);
}

