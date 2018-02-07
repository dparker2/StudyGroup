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
    ui->back_text->setText(text);
}
