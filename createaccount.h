#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "sgwidget.h"
#include "securityquestions.h"
#include "accountinfovalidator.h"

namespace Ui {
class CreateAccount;
}

class CreateAccount : public SGWidget, public AccountInfoValidator
{
    Q_OBJECT

public:
    explicit CreateAccount(QString name = "create account", QWidget *parent = 0);
    ~CreateAccount();

private slots:
    void do_work();

    void on_lineEdit_email_textChanged(const QString &email);
    void on_lineEdit_username_signup_textChanged(const QString &username);
    void on_lineEdit_password1_textChanged(const QString &password1);
    void on_lineEdit_password2_textChanged(const QString &password2);


    void on_lineEdit_email_editingFinished();
    void on_lineEdit_username_signup_editingFinished();
    void on_lineEdit_password1_editingFinished();
    void on_lineEdit_password2_editingFinished();

    // Validate Info Functions
    /*
    bool validate_email(QString email);
    bool validate_username(QString username, QString &error_msg);
    bool validate_password(QLineEdit* current_line, QLineEdit* other_line, QString &error_msg);
    */

    void set_valid_icons(QLabel* this_label, QLineEdit* this_line, bool valid, QString error_msg = "");
    //void initialize_validation_variables(bool);
    //bool info_valid_status();

    void on_register_btn_clicked();

private:
    Ui::CreateAccount *ui;
    SecurityQuestions *security_questions;

    // Might move this stuff to a validation class
    /*
    QString error_msg;
    bool valid_username;
    bool valid_email;
    bool valid_password;
    */
};

#endif // CREATEACCOUNT_H
