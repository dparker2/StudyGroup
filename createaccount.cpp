#include "createaccount.h"
#include "ui_createaccount.h"
#include "server.h"
#include <QRegExp>
#include <QMessageBox>

const int USERNAME_MIN = 8;
const int USERNAME_MAX = 20;

const int PASSWORD_MIN = 8;
const int PASSWORD_MAX = 16;

CreateAccount::CreateAccount(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::CreateAccount)
{
    ui->setupUi(this);
    user_info_valid = false;
    security_questions = new SecurityQuestions();
    ui->questions_layout->addWidget(security_questions);

    server::send(server::SECURITY_QUESTIONS+"msalomon"); // should send at initial start of program

    //ui->register_btn->setEnabled(false);
}

CreateAccount::~CreateAccount()
{
    delete ui;
}
void CreateAccount::do_work()
{
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QString code = message.left(4);
        message.remove(0,4);
        QList<QByteArray> message_list = split(message,3);
        if (code == "REQQ")
        {
            qDebug() << "MESSAGE LIST:  " << message_list;
            security_questions->REQQ(message_list);
        }
    }
}
/*****************************************************
 * ACCOUNT SIGNUP
 */

void CreateAccount::on_register_btn_clicked()
{
    QString username = ui->lineEdit_username_signup->text();
    QString full_string = server::CREATE_ACCOUNT + ui->lineEdit_email->text() +
            " " + username +
            " " + ui->lineEdit_password2->text();
    QString response, questions;
    qDebug() << "Info Valid.." << user_info_valid;
    if(user_info_valid && server::request_response(full_string, response))
    {
        qDebug() << "sending account";
        QMessageBox success_box;
        success_box.setText(response);
        success_box.exec();
        ui->lineEdit_email->setText("");
        ui->lineEdit_username_signup->setText("");
        ui->lineEdit_password1->setText("");
        ui->lineEdit_password2->setText("");
    }
}
void CreateAccount::on_lineEdit_email_textChanged(const QString &email)
{
    ui->label_email_check->clear();
    user_info_valid = validate_email(email);
    if(user_info_valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, user_info_valid);
    }
}
void CreateAccount::on_lineEdit_email_editingFinished()
{
    QString email = ui->lineEdit_email->text();
    if(!email.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, user_info_valid, "Email not valid");
    }
}
void CreateAccount::on_lineEdit_username_signup_textChanged(const QString &username)
{
    ui->label_username_check->clear();
    user_info_valid = validate_username(username, error_msg);
    if(user_info_valid){
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid);
    }
}

void CreateAccount::on_lineEdit_username_signup_editingFinished()
{
    QString username = ui->lineEdit_username_signup->text();
    if(!username.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid, error_msg);
    }
}

void CreateAccount::on_lineEdit_password1_textChanged(const QString &password1)
{
    ui->label_password1_check->clear();
    user_info_valid = validate_password(ui->lineEdit_password1, ui->lineEdit_password2, error_msg);
    if(!password1.isEmpty() && user_info_valid){
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, user_info_valid);
    }
}

void CreateAccount::on_lineEdit_password1_editingFinished()
{
    QString password = ui->lineEdit_password1->text();
    if(!password.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, user_info_valid, error_msg);
    }
}

void CreateAccount::on_lineEdit_password2_textChanged(const QString &password2)
{
    ui->label_password2_check->clear();
    user_info_valid = validate_password(ui->lineEdit_password2, ui->lineEdit_password1, error_msg);
    if(!password2.isEmpty() && user_info_valid){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, user_info_valid);
        ui->lineEdit_password2->setFocus();
    }
}

void CreateAccount::on_lineEdit_password2_editingFinished()
{
    QString password = ui->lineEdit_password2->text();
    if(!password.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, user_info_valid, error_msg);
    }
}

/*
 * User Info Validation
*****/

void CreateAccount::set_valid_icons(QLabel* this_label, QLineEdit* this_line, bool valid, QString error_msg)
{
    if(!error_msg.isEmpty())
    {
        // Qt bug fix so it wont display prompt twice for textchanged/editing finished signals
        if(this_line->text().toDouble()<1000)
        {
            this_line->blockSignals(true);
            QMessageBox error_box;
            error_box.information(0, "Error Message", error_msg);
            this_line->blockSignals(false);
        }
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

bool CreateAccount::validate_username(QString username, QString &error_msg)
{
    QRegExp username_regex("([\\d]+|[\\w]+)+");
    username_regex.setCaseSensitivity(Qt::CaseSensitive);
    username_regex.setPatternSyntax(QRegExp::RegExp);
    bool password_valid = username_regex.exactMatch(username);

    if(username.size() > USERNAME_MIN && username.size() < USERNAME_MAX && password_valid){
        return true;
    }
    if(!password_valid){
        error_msg = "Username can contain only alphanumeric characters and unserscores";
    }
    else if(username.size() < USERNAME_MIN)
    {
        QString min = QString::number(USERNAME_MIN);
        error_msg = "Username must be more than " + min + "  characters";
    }
    else if(username.size() > USERNAME_MAX)
    {
        QString max = QString::number(USERNAME_MIN);
        error_msg = "Username cannot exceed " + max + " characters";
    }
    return false;
}

bool CreateAccount::validate_password(QLineEdit* current_line, QLineEdit* other_line, QString &error_msg)
{
    QString current_password = current_line->text();
    QString other_password = other_line->text();

    if(!other_password.isEmpty() && current_password != other_password)
    {
        error_msg = "Passwords do not match";
        qDebug() << error_msg;
        return false;
    }
    if(current_password.size() < PASSWORD_MIN)
    {
        QString min = QString::number(PASSWORD_MIN);
        error_msg = "Password cannot be less than " + min + " characters";
        qDebug() << error_msg;
        return false;
    }
    else if(current_password.size() > PASSWORD_MAX)
    {
        QString max = QString::number(PASSWORD_MAX);
        error_msg = "Password cannot exceed " + max + " characters";
        qDebug() << error_msg;
        return false;
    }
    return true;
}




