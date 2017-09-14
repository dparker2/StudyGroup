#include "mainWindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_signInButton_clicked()
{
    if(ui->lineEdit_username->text() != "studygroup" || ui->lineEdit_password->text() != "csci150"){
        ui->label->setText("Username or Password incorrect");

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
