#include "startpage.h"
#include "ui_startpage.h"
#include "server.h"
#include "joingrouppage.h"
#include <QMessageBox>

StartPage::StartPage(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::do_work()
{
    while(!_work_queue.isEmpty()) {
        qDebug() << _work_queue.dequeue();
    }
}

void StartPage::on_signin_button_clicked()
{
    QString login_code = "LOGN";
    QString email;  // Return parameter from the server response

    server::test("startpage", "this is from the signin slot");
    QString full_string = login_code + ui->lineEdit_username->text() + " " + ui->lineEdit_password->text();

    if(server::request_response(full_string, email))
    {
        // Now logged in!
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");
        // Set username and password
        //user_info->setUsername(username);                     // FIX THESE WHEN USER CLASS IS DONE
        //user_info->setPassword(password);
        // Update settings page
        //ui->settings_email->setText(email);                   // FIX THESE WHEN SETTINGS CLASS IS DONE
        //ui->settings_username->setText(user_info->getUsername());

        emit logged_in(0); // Change main page
    }
}

void StartPage::on_singup_button_clicked()
{
    QString create_code = "CACC";
    QString full_string = create_code + ui->lineEdit_email->text() +
            " " + ui->lineEdit_username_signup->text() +
            " " + ui->lineEdit_password2->text();
    QString response;
    if(server::request_response(full_string, response))
    {
        QMessageBox success_box;
        success_box.setText(response);
        success_box.exec();
        ui->lineEdit_email->setText("");
        ui->lineEdit_username_signup->setText("");
        ui->lineEdit_password1->setText("");
        ui->lineEdit_password2->setText("");
    }
}

/*****************************************************
 * ACCOUNT SIGNUP
 */

void StartPage::on_lineEdit_email_textChanged(const QString &email)
{
    if (email == "") {
        ui->label_email_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, error_msg, valid);
    }
}

void StartPage::on_lineEdit_username_signup_textChanged(const QString &username)
{
    if (username == "") {
        ui->label_username_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, error_msg, valid);
    }
}

void StartPage::on_lineEdit_password1_textChanged(const QString &password1)
{
    if (password1 == "") {
        ui->label_password1_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, error_msg, valid);
    }
}

void StartPage::on_lineEdit_password2_textChanged(const QString &password2)
{
    if (password2 == "") {
        ui->label_password2_check->hide();
    } else {
        QString error_msg;
        bool valid = true; //user_info->usernameValidation(username,  error_msg);
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, error_msg, valid);
    }
}

void StartPage::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid){
    QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    this_label->setPixmap(mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this_label->show();
}

/*****************************************************************
 * ACCOUNT RECOVERY
 */

void StartPage::on_pushButton_recover_pass_clicked()
{
    /*QString username = ui->lineEdit_recover_pass_1->text();
    QString email = ui->lineEdit_recover_pass_2->text();

    QString pass;

    if (my_serv->recover_pass(username, email, pass)){
        QMessageBox password_box;
        password_box.setText ("Email Sent!");
        password_box.exec();
    }
    else {
        QMessageBox error_box;
        error_box.critical (0, "Error", "An error has occured! ");
        error_box.setFixedSize(500,200);
    }*/

}

void StartPage::on_pushButton_recover_user_clicked()
{
    QString email = ui->lineEdit_recover_user->text();
    QString user;

    /*if (my_serv->recover_user(email, user)){
        // QString username = user;
        QMessageBox username_box;
        username_box.setText("Your username is: ");
        username_box.setInformativeText(user); //placeholder
        username_box.exec();
    }
    else {
        QMessageBox error_box;
        error_box.critical (0, "Error", "An error has occured! ");
        error_box.setFixedSize(500,200);
    }*/

}
