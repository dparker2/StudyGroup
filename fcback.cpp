#include "fcback.h"
#include "ui_fcback.h"

FCBack::FCBack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCBack)
{
    ui->setupUi(this);
}

FCBack::~FCBack()
{
    delete ui;
}
