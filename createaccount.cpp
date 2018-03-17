#include "createaccount.h"
#include "ui_createaccount.h"
#include <QRegExp>
#include <QMessageBox>

CreateAccount::CreateAccount(QString name, QWidget *parent) :
    SGWidget(name, parent), Recoverusername(name,parent),
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
void CreateAccount::on_lineEdit_email_textChanged(const QString &email)
{
    if (email.isEmpty()) {
        ui->label_email_check->clear();
    } else {
        QString error_msg;
        user_info_valid = validate_email(email); //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, true);
    }
}
void CreateAccount::on_lineEdit_username_signup_textChanged(const QString &username)
{
    if (username.isEmpty()) {
        ui->label_username_check->clear();
    }
    else {
        QString error_msg;
        user_info_valid = validate_username(username); //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid);
    }
}

void CreateAccount::on_lineEdit_password1_textChanged(const QString &password1)
{
    if (password1.isEmpty()) {
        ui->label_password1_check->clear();
    }
    else {
        QString password2 = ui->lineEdit_password2->text();

        if(!password2.isEmpty())
        {
            user_info_valid = (password1 == password2);
            if(user_info_valid)
            {
                set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, user_info_valid);
            }
        }
        else{
            user_info_valid = validate_password(password1);
        }
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, user_info_valid);
    }
}

void CreateAccount::on_lineEdit_password2_textChanged(const QString &password2)
{
    if (password2.isEmpty()) {
        ui->label_password2_check->clear();
    }
    else {
        QString password1 = ui->lineEdit_password1->text();
        if(!password1.isEmpty())
        {
            user_info_valid = (password1 == password2);
            if(user_info_valid)
            {
                set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, user_info_valid);
            }
        }
        else{
            user_info_valid = validate_password(password2);
        }
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, user_info_valid);
    }
}


/*
 * Validation Functions
*****/

void CreateAccount::set_valid_icons(QLabel* this_label, QLineEdit* this_line, bool valid, QString error_msg)
{
    if(!error_msg.isEmpty())
    {
        QMessageBox error_box;
        error_box.information(0, "Error Message", error_msg);
    }
    QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    this_label->setPixmap(mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this_label->show();
}

bool CreateAccount::validate_email(QString email)
{
    QRegExp email_regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    email_regex.setCaseSensitivity(Qt::CaseInsensitive);
    email_regex.setPatternSyntax(QRegExp::RegExp);

    return email_regex.exactMatch(email);
}

bool CreateAccount::validate_username(QString username)

{
    QRegExp username_regex("([\\d]+|[\\w]+)+");
    username_regex.setCaseSensitivity(Qt::CaseSensitive);
    username_regex.setPatternSyntax(QRegExp::RegExp);
    return username_regex.exactMatch(username);

}

bool CreateAccount::validate_password(QString password)
{
    // Leaving this here just in case we add a restrictions to passwords
    return true;
}

void CreateAccount::on_lineEdit_password1_editingFinished()
{
    password = ui->lineEdit_password1->text();
    if(!password.isEmpty() && ui->lineEdit_password1->text().size() < 8)
    {
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, false, "Password must be at least 8 characters");
    }
}

void CreateAccount::on_lineEdit_password2_editingFinished()
{
    password = ui->lineEdit_password2->text();
    if(!password.isEmpty() && ui->lineEdit_password2->text().size() < 8)
    {
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false, "Password must be at least 8 characters");
    }
}
