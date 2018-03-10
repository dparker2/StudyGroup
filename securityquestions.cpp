#include "securityquestions.h"
#include "ui_securityquestions.h"

SecurityQuestions::SecurityQuestions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityQuestions)
{
    ui->setupUi(this);
}

SecurityQuestions::~SecurityQuestions()
{
    delete ui;
}
