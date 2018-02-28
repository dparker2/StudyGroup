#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "accountsecurity.h"

namespace Ui {
class StartPage;
}

class StartPage : public SGWidget
{
    Q_OBJECT

public:
    explicit StartPage(QString name = "startpage", QWidget *parent = 0);
    ~StartPage();

signals:
    void logged_in(unsigned);

private slots:
    void expand_tabwidget();

private slots:
    void on_signin_button_clicked();

    /*void on_lineEdit_email_editingFinished();
    void on_lineEdit_email_cursorPositionChanged();
    void on_lineEdit_email_textEdited();

    void on_lineEdit_username_signup_editingFinished();
    void on_lineEdit_username_signup_cursorPositionChanged();
    void on_lineEdit_username_signup_textEdited();

    void on_lineEdit_password1_editingFinished();
    void on_lineEdit_password1_cursorPositionChanged();
    void on_lineEdit_password1_textEdited();

    void on_lineEdit_password2_editingFinished();*/

    void set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid);

    //void on_singup_button_clicked();

    void on_pushButton_recover_pass_clicked();

    void on_pushButton_recover_user_clicked();

    //void on_lineEdit_password2_textEdited();

    //void on_lineEdit_password2_cursorPositionChanged();

    void do_work();

    void on_lineEdit_email_textChanged(const QString &arg1);

    void on_lineEdit_username_signup_textChanged(const QString &arg1);

    void on_lineEdit_password1_textChanged(const QString &arg1);

    void on_lineEdit_password2_textChanged(const QString &arg1);

    void on_tabWidget_tabBarClicked(int index);

    void on_save_question_btn_clicked();

    void on_register_btn_clicked();

private:
    Ui::StartPage *ui;
    AccountSecurity *recover;
    QString username;
    bool customQ_flag;
};

#endif // STARTPAGE_H
