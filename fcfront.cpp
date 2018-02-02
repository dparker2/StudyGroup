#include "fcfront.h"
#include "ui_fcfront.h"

FCFront::FCFront(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFront)
{
    ui->setupUi(this);
}

FCFront::~FCFront()
{
    delete ui;
}
