#include "accountinfovalidator.h"
#include <QDebug>
const int USERNAME_MIN = 8;
const int USERNAME_MAX = 20;

const int PASSWORD_MIN = 8;
const int PASSWORD_MAX = 16;

AccountInfoValidator::AccountInfoValidator()
{
    initialize_validation_variables(false);
}

bool AccountInfoValidator::get_username_status(){
    return valid_username;
}
bool AccountInfoValidator::get_email_status(){
    return valid_email;
}
bool AccountInfoValidator::get_password1_status(){
    return valid_password1;
}
bool AccountInfoValidator::get_password2_status(){
    return valid_password2;
}
bool AccountInfoValidator::info_valid_status()
{
    return valid_email && valid_username && valid_password1 && valid_password2;
}
QString AccountInfoValidator::get_error_msg(){
    return error_msg;
}

void AccountInfoValidator::initialize_validation_variables(bool set_false){
    valid_email = set_false;
    valid_username = set_false;
    valid_password1 = set_false;
    valid_password2 = set_false;
}

bool AccountInfoValidator::validate_email(QString email)
{
    QRegExp email_regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    email_regex.setCaseSensitivity(Qt::CaseInsensitive);
    email_regex.setPatternSyntax(QRegExp::RegExp);

    return valid_email = email_regex.exactMatch(email);
}

bool AccountInfoValidator::validate_username(QString username)
{
    QRegExp username_regex("([\\d]+|[\\w]+)+");
    username_regex.setCaseSensitivity(Qt::CaseSensitive);
    username_regex.setPatternSyntax(QRegExp::RegExp);

    bool regex_valid = username_regex.exactMatch(username);
    bool size_valid = username.size() > USERNAME_MIN && username.size() < USERNAME_MAX;

    if(size_valid && regex_valid){
        return valid_username = true;
    }
    if(!regex_valid){
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
    return valid_username = false;
}

bool AccountInfoValidator::validate_password(QLineEdit* current_line, QLineEdit* other_line, bool pwd)
{
    QString current_password = current_line->text();
    QString other_password = other_line->text();
    bool valid;

    qDebug() << "OTHER PASS: " << other_password;
    qDebug() << "CURR PASS: " << current_password;

    if(!current_password.isEmpty() && !other_password.isEmpty() && current_password == other_password){
        valid_password1 = valid_password2 = true;
        return true;
    }
    else if(!current_password.isEmpty() && current_password.size() < PASSWORD_MIN)
    {
        QString min = QString::number(PASSWORD_MIN);
        error_msg = "Password cannot be less than " + min + " characters";
        valid = false;
    }
    else if(!current_password.isEmpty() && current_password.size() > PASSWORD_MAX)
    {
        QString max = QString::number(PASSWORD_MAX);
        error_msg = "Password cannot exceed " + max + " characters";
        valid = false;
    }
    else if(!other_password.isEmpty()){
        error_msg = "Passwords do not match";
        valid = false;
    }
    qDebug() << "TEST: " << valid;
    if(pwd){
        return valid_password1 = valid;
    }
    return valid_password2 = valid;
}






