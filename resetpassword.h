#ifndef RESETPASSWORD_H
#define RESETPASSWORD_H

#include "sgwidget.h"
#include <QWidget>

namespace Ui {
class ResetPassword;
}

class ResetPassword : public SGWidget
{
    Q_OBJECT

public:
    explicit ResetPassword(QString name = "recover password", QWidget *parent = 0);
    ~ResetPassword();

private slots:
    void do_work();
    void RPWD(int index, QString random_question);

    void on_next_btn_clicked();

    void on_submit_answer_btn_clicked();

    void on_verify_code_btn_clicked();

    void on_confirm_pass_lineEdit_clicked();

private:
    Ui::ResetPassword *ui;
    QString security_question;
    QString question_index;
    QString username;
};

#endif // RESETPASSWORD_H
