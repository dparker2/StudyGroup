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

private slots:
    void do_work();
    void RUSR(QString email_sent);
    void on_recover_user_btn_clicked();



    void on_recover_pass_btn_clicked();

private:
    Ui::AccountSecurity *ui;
};

#endif // ACCOUNTSECURITY_H
