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
    security_questions = new SecurityQuestions();
    ui->questions_layout->addWidget(security_questions);
    QString test;
    server::send(server::SECURITY_QUESTIONS+"msalomon"); // should send at initial start of program

    ui->register_btn->setEnabled(false);
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
    user_info_valid = validate_email(email);
    ui->label_email_check->clear();
    if(user_info_valid){
            set_valid_icons(ui->label_email_check, ui->lineEdit_email, user_info_valid);
    }
}
void CreateAccount::on_lineEdit_email_editingFinished()
{
    QString email = ui->lineEdit_email->text();
    user_info_valid = validate_email(email);

    if(!email.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, user_info_valid, "Email not valid");
        ui->lineEdit_email->setFocus();
    }
}
void CreateAccount::on_lineEdit_username_signup_textChanged(const QString &username)
{
    QString error_msg;
    user_info_valid = validate_username(username, error_msg);
    ui->label_username_check->clear();
    if(user_info_valid){
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid);
    }
}
void CreateAccount::on_lineEdit_username_signup_editingFinished()
{
    QString username = ui->lineEdit_username_signup->text();

    QString error_msg;
    user_info_valid = validate_username(username, error_msg);
    if(!username.isEmpty() && !user_info_valid){
        qDebug() << "editing finished...";
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid, error_msg);
        ui->lineEdit_username_signup->setFocus();
    }
}

void CreateAccount::on_lineEdit_password1_textChanged(const QString &password)
{
    QString error_msg;
    user_info_valid = validate_password(ui->lineEdit_password1, ui->lineEdit_password2, error_msg);
    ui->label_password1_check->clear();
    if(user_info_valid){
        qDebug() << "text changed...";
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, user_info_valid);
    }
    /*

    if (password1.isEmpty()) {
        ui->label_password1_check->clear();
    }
    else {
        QString password2 = ui->lineEdit_password2->text();

        if(!password2.isEmpty() && validate_password(password1))
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
    }*/
}

void CreateAccount::on_lineEdit_password2_textChanged(const QString &password2)
{
    QString error_msg;
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
            user_info_valid = validate_password(ui->lineEdit_password2, ui->lineEdit_password1, error_msg);
        }
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, user_info_valid);
    }
}

void CreateAccount::on_lineEdit_password1_editingFinished()
{
    QString error_msg;
    QString password = ui->lineEdit_password1->text();
    user_info_valid = validate_password(ui->lineEdit_password1, ui->lineEdit_password2, error_msg);

    if(!password.isEmpty() && !user_info_valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, user_info_valid, error_msg);
        ui->lineEdit_email->setFocus();
    }
    /*
    QString password1 = ui->lineEdit_password1->text();
    QString password2 = ui->lineEdit_password2->text();
    if(!password1.isEmpty() && !password2.isEmpty() && ui->lineEdit_password1->text().size() < PASSWORD_MIN)
    {
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, false, "Password must be at least 8 characters");
        //set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false);
    }
    else if(password1.size() > PASSWORD_MAX){
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, false, "Password cannot be more that 16 characters");
    }
    else if(!password2.isEmpty() && password1 != password2){
        qDebug() << "Pass1 match..";
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, false, "Passwords do not match");
    }
    */
}

void CreateAccount::on_lineEdit_password2_editingFinished()
{
    QString password1 = ui->lineEdit_password1->text();
    QString password2 = ui->lineEdit_password2->text();

    if(!password2.isEmpty() && !password2.isEmpty() && ui->lineEdit_password2->text().size() < PASSWORD_MIN)
    {
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false, "Password must be at least 8 characters");
        //set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, false);
    }
    else if(password2.size() > PASSWORD_MAX){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false, "Password cannot be more that 16 characters");
    }
    else if(!password1.isEmpty() && password1 != password2){
        qDebug() <<"Pass2 match..";
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false, "Passwords do not match");
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

    if(!current_password.isEmpty() && current_password.size() < PASSWORD_MIN){
        QString min = QString::number(PASSWORD_MIN);
        error_msg = "Password cannot be less than " + min + " characters";
        return false;
    }
    if(!current_password.isEmpty() && current_password.size() > PASSWORD_MAX){
        QString max = QString::number(PASSWORD_MAX);
        error_msg = "Password cannot exceed " + max + " characters";
        return false;
    }
    else if(!current_password.isEmpty() && !other_password.isEmpty() && current_password != other_password)
    {
        error_msg = "Passwords do not match";
        //set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, false);
        return false;
    }
    return true;


}



