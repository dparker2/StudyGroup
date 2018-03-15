#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "sgwidget.h"

namespace Ui {
class CreateAccount;
}

class CreateAccount : public SGWidget
{
    Q_OBJECT

public:
    explicit CreateAccount(QString name, QWidget *parent = 0);
    ~CreateAccount();

private slots:
    void do_work();
    void on_lineEdit_username_signup_textChanged(const QString &arg1);
    void set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid);

    void on_lineEdit_email_textChanged(const QString &arg1);

    void on_lineEdit_password1_textChanged(const QString &arg1);

    void on_lineEdit_password2_textChanged(const QString &arg1);

    // Validate Info Functions
    bool validate_email(QString);

    void on_lineEdit_email_editingFinished();

    void on_lineEdit_password2_editingFinished();

private:
    Ui::CreateAccount *ui;
    QString username;
};

#endif // CREATEACCOUNT_H
