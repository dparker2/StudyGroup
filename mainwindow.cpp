#include "mainwindow.h"
#include "ui_mainwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    my_serv = new server();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_signInButton_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username != "studygroup" || password != "csci150"){
        ui->label->setText("Username or Password incorrect");
    } else {
        my_serv->verifyUserInfo(username, password);
    }
}

void LoginWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index){
        ui->tabWidget->widget(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            ui->tabWidget->widget(index)->resize(ui->tabWidget->widget(index)->minimumSizeHint());
            ui->tabWidget->widget(index)->adjustSize();
    }
}
