#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    connect(ui->loginButton, SIGNAL(released()), this, SLOT(login()));
}

void LoginPage::login()
{
    ui->testText->setText(ui->lineEdit->text());
}

LoginPage::~LoginPage()
{
    delete ui;
}
