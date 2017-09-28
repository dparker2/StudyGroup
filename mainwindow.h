#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include "server.h"
#include "user_account_check.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_signin_button_clicked();

    void on_lineEdit_email_editingFinished();
    void on_lineEdit_email_cursorPositionChanged();
    void on_lineEdit_email_textEdited();

    void on_lineEdit_username_signup_editingFinished();
    void on_lineEdit_username_signup_cursorPositionChanged();
    void on_lineEdit_username_signup_textEdited();

    void on_lineEdit_password1_editingFinished();
    void on_lineEdit_password1_cursorPositionChanged();
    void on_lineEdit_password1_textEdited();

    void on_lineEdit_password2_editingFinished();

    void invalid_label_stylesheet(QLabel* this_label, QLineEdit *this_line, QString error_msg);
    void set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid);



    void on_singup_button_clicked();

private:
    Ui::MainWindow *ui;
    server *my_serv;
    UserAccount *user_info;
};

#endif // MAINWINDOW_H
