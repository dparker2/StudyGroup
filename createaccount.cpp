#include "createaccount.h"
#include "ui_createaccount.h"

CreateAccount::CreateAccount(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::CreateAccount)
{
    ui->setupUi(this);
}

CreateAccount::~CreateAccount()
{
    delete ui;
}
void CreateAccount::do_work(){

}
void CreateAccount::on_lineEdit_email_textChanged(const QString &arg1)
{
    QString email = ui->lineEdit_email->text();
    if (email.isEmpty()) {
        ui->label_email_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, error_msg, valid);
    }
}
void CreateAccount::on_lineEdit_username_signup_textChanged(const QString &arg1)
{
    username = ui->lineEdit_username_signup->text();
    if (username.isEmpty()) {
        ui->label_username_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, error_msg, valid);
    }
}

void CreateAccount::on_lineEdit_password1_textChanged(const QString &password1)
{
    if (password1 == "") {
        ui->label_password1_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, error_msg, valid);
    }
}

void CreateAccount::on_lineEdit_password2_textChanged(const QString &password2)
{
    if (password2 == "") {
        ui->label_password2_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, error_msg, valid);
    }
}

void CreateAccount::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid)
{
    QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    this_label->setPixmap(mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this_label->show();
}

