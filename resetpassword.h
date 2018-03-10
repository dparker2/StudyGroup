#ifndef RESETPASSWORD_H
#define RESETPASSWORD_H

#include "sgwidget.h"
#include <QWidget>
#include <QLabel>

namespace Ui {
class ResetPassword;
}

class ResetPassword : public SGWidget
{
    Q_OBJECT

public:
    explicit ResetPassword(QString name = "reset password", QWidget *parent = 0);
    ~ResetPassword();

private slots:
    void do_work();
    void clear_text();
    void set_invalid_icon(QLabel *mark);
    void RPWD(int index, QString random_question);
    void on_next_btn_clicked();
    void on_submit_answer_btn_clicked();
    void on_verify_code_btn_clicked();
    void on_reset_password_btn_clicked();

    void on_username_lineEdit_textEdited(const QString &arg1);

private:
    Ui::ResetPassword *ui;
    QString security_question;
    int question_index;
    QString username;
};

#endif // RESETPASSWORD_H
