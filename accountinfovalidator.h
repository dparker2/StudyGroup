#ifndef ACCOUNTINFOVALIDATOR_H
#define ACCOUNTINFOVALIDATOR_H

#include <QLineEdit>
#include <QLabel>

class AccountInfoValidator
{
public:
    AccountInfoValidator();
    void initialize_validation_variables(bool);

    bool get_username_status();
    bool get_email_status();
    bool get_password1_status();
    bool get_password2_status();
    QString get_error_msg();

    bool validate_email(QString email, bool check_empty);
    bool validate_username(QString username, bool check_empty);
    bool validate_password(QLineEdit* current_line, QLineEdit* other_line, bool pwd, bool check_empty);
    bool info_valid_status();

private:
    QString error_msg;
    bool valid_username;
    bool valid_email;
    bool valid_password1;
    bool valid_password2;
};

#endif // ACCOUNTINFOVALIDATOR_H
