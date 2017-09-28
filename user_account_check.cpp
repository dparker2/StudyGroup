#include "user_account_check.h"
//#include "mainwindow.h"
#include <QRegularExpression>

UserAccount::UserAccount(QWidget *parent) : QMainWindow(parent)
{
    acct_email = "0 ";
    acct_username = "1 ";
    acct_password = "2 ";
    info_complete[3] = 0;    // defaulted to false
}
bool UserAccount::get_info_complete(int i){
    return info_complete[i];
}
void UserAccount::setUsername(QString usernameStr){
    acct_username.push_back(usernameStr);
}
void UserAccount::setEmail(QString emailStr){
    acct_email.push_back(emailStr);
}
void UserAccount::setPassword(QString passwordStr){
    acct_password.push_back(passwordStr);
}
/*
 * Verifys if info is ready to send. If all true: ready to send
 * otherwise sign up button will not send info to server until all info
 * is entered and correct.
 * index 0: email, index 1: username, index 2: password
 */
void UserAccount::set_info_complete(int i, bool set){
    info_complete[i] = set;
}
QString UserAccount::getUsername(){
    return acct_username;
}
QString UserAccount::getEmail(){
    return acct_email;
}
QString UserAccount::getPassword(){
    return acct_password;
}
bool UserAccount::usernameValidation(QString usernameStr, QString &error_msg){

    if(usernameStr.isEmpty()){
        return false;
    }

    // checks for whitespace within the username string
    for(int i = 0; i < usernameStr.size(); i++){
        if(usernameStr[i] == " "){
            error_msg = "Username cannot contain a space";
            return false;
        }
    }

    // Checks for invalid username characters
    QRegularExpression username_regex("[!@#$%^&*()-+={}\\\|.]");
    QRegularExpressionMatch match = username_regex.match(usernameStr);
    bool hasMatch = match.hasMatch();     // this will be true if username matches with special characters

    if(hasMatch){
        error_msg = "Username can only contain underscore";

    }
    return !hasMatch;
}
bool UserAccount::emailValidation(QString emailStr, QString &error_msg){

    // Regex matches valid email
    QRegularExpression email_regex("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    QRegularExpressionMatch match = email_regex.match(emailStr);
    bool hasMatch = match.hasMatch();

    return match.hasMatch();
 }
bool UserAccount::passwordValidtion(QString passwordStr, QString &error_msg){

    // checks for whitespace within the password string
    for(int i = 0; i < passwordStr.size(); i++){
        if(passwordStr[i] == " "){
            error_msg = "password cannot contain a space";
            return false;
        }
    }
    return true;
}
