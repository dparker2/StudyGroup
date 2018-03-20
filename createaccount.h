#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "sgwidget.h"
#include "securityquestions.h"

namespace Ui {
class CreateAccount;
}

class CreateAccount : public SGWidget
{
    Q_OBJECT

public:
    explicit CreateAccount(QString name = "create account", QWidget *parent = 0);
    ~CreateAccount();

private slots:
    void do_work();
    void on_lineEdit_username_signup_textChanged(const QString &arg1);
    void set_valid_icons(QLabel* this_label, QLineEdit* this_line, bool valid, QString error_msg = "");

    void on_lineEdit_email_textChanged(const QString &arg1);

    void on_lineEdit_password1_textChanged(const QString &arg1);

    void on_lineEdit_password2_textChanged(const QString &arg1);

    // Validate Info Functions
    bool validate_email(QString email);
    bool validate_username(QString username, QString &error_msg);
    bool validate_password(QLineEdit* current_line, QLineEdit* other_line, QString &error_msg);



    void on_lineEdit_password1_editingFinished();

    void on_lineEdit_password2_editingFinished();

    void on_register_btn_clicked();

    void on_lineEdit_email_editingFinished();

    void on_lineEdit_username_signup_editingFinished();

private:
    Ui::CreateAccount *ui;
    SecurityQuestions *security_questions;
    bool user_info_valid;
    QString error_msg;
};

#endif // CREATEACCOUNT_H
