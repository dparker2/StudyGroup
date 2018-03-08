#ifndef ACCOUNTSECURITY_H
#define ACCOUNTSECURITY_H

#include <QWidget>
#include "sgwidget.h"

namespace Ui {
class AccountSecurity;
}

class AccountSecurity : public SGWidget
{
    Q_OBJECT

public:
    explicit AccountSecurity(QString name = "recover", QWidget *parent = 0);
    ~AccountSecurity();
    void display_recovery_page(int index);
    void clear_text();

signals:

private slots:
    void do_work();
    void RUSR(QString email_sent);
    void RPWD(int index, QString question);
    void on_recover_user_btn_clicked();
    void on_recover_pass_btn_clicked();

    void on_next_btn_clicked();

    void on_request_code_btn_clicked();

    void on_submit_code_btn_clicked();

    void on_reset_password_btn_clicked();

private:
    Ui::AccountSecurity *ui;
    QString random_question_answer;
    QString random_question_index;
};

#endif // ACCOUNTSECURITY_H
