#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <qcombobox.h>
#include "recoverusername.h"
#include "resetpassword.h"
#include "createaccount.h"

namespace Ui {
class StartPage;
}

class StartPage : public SGWidget
{
    Q_OBJECT

public:
    explicit StartPage(QString name = "startpage", QWidget *parent = 0);
    ~StartPage();

    void display_login();

signals:
    void logged_in(unsigned);


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

    //void set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid);

    //void on_singup_button_clicked();

    void on_pushButton_recover_user_clicked();

    //void on_lineEdit_password2_textEdited();

    //void on_lineEdit_password2_cursorPositionChanged();

    void do_work();


    void on_tabWidget_currentChanged(int index);


    void on_pushButton_reset_password_clicked();

    void hide_recover_buttons();
    void show_recover_buttons();

    void on_pushButton_recover_back_clicked();

    void display_recover_widget(QWidget* recover_widget);


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::StartPage *ui;
    RecoverUsername *recover_username;
    ResetPassword *reset_password;
    QString username;
    bool customQ_flag;
    QList<QLineEdit*> custom_questions;
    QList<QString> questions;
    QList<QComboBox*> question_comboBox;
    CreateAccount* sign_up;

};

#endif // STARTPAGE_H
