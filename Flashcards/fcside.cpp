#include "fcside.h"
#include "ui_fcside.h"

FCSide::FCSide(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCSide)
{
    ui->setupUi(this);
}

FCSide::~FCSide()
{
    delete ui;
}

void FCSide::set_text(QString& text)
{
    ui->label->setText(text);
}

QString FCSide::get_text()
{
    return ui->label->text();
}
