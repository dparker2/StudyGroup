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

    void clear_info();

private slots:
    void do_work();    
    void set_invalid_icon(QLabel *mark);
    void RPWD(int index, QString random_question);

    void on_username_lineEdit_textEdited(const QString &arg1);



    void on_pushButton_reset_password_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_submit_answer_clicked();

    void on_pushButton_verify_code_clicked();

private:
    Ui::ResetPassword *ui;
    QString security_question;
    int question_index;
    QString username;
};

#endif // RESETPASSWORD_H
