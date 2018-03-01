#include "startpage.h"
#include "ui_startpage.h"
#include "server.h"
#include "joingrouppage.h"
#include <QMessageBox>
#include <QListView>

StartPage::StartPage(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit
    QPixmap pencil(":/resources/img/edit_pencil.png");

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);

    // Account Security
    recover = new AccountSecurity("recover");
    customQ_flag = false;

}

StartPage::~StartPage()
{
    delete ui;
}
void StartPage::do_work(){
    qDebug() << "IN DO WORK " << _work_queue.isEmpty() ;
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        qDebug() << "MESSAGE IN DO WORK: " << message;
        QList<QByteArray> message_list = split(message,4);
        if (message_list[0] == "REQQ")
        {
            set_questions(message_list);

        }
    }
}


void StartPage::on_signin_button_clicked()
{
    QString email;  // Return parameter from the server response

    server::test("startpage", "this is from the signin slot");
    QString full_string = server::LOGIN + ui->lineEdit_username->text() + " " + ui->lineEdit_password->text();

    if(server::request_response(full_string, email))
    {
        // Now logged in!
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");
        qDebug() << "Email:" << email;
        // Set username and password
        //user_info->setUsername(username);                     // FIX THESE WHEN USER CLASS IS DONE
        //user_info->setPassword(password);
        // Update settings page
        //ui->settings_email->setText(email);                   // FIX THESE WHEN SETTINGS CLASS IS DONE
        //ui->settings_username->setText(user_info->getUsername());

        emit logged_in(0); // Change main page
    }
}
/*
void StartPage::on_singup_button_clicked()
{
    ui->sign_up->setCurrentIndex(1);
    recover->display_recovery_page(1);
    ui->recover_account->addWidget(recover);
    ui->recover_account->setCurrentWidget(recover);
    ui->tab_recover_account->layout()->setContentsMargins(0,0,0,0);
}
*/

/*****************************************************
 * ACCOUNT SIGNUP
 */

/**
 * Comeback here when user info starts to get verified
 * to handle the empty string case and invalid info
 */
void StartPage::on_register_btn_clicked()
{
    username = ui->lineEdit_username_signup->text();
    QString full_string = server::CREATE_ACCOUNT + ui->lineEdit_email->text() +
            " " + username +
            " " + ui->lineEdit_password2->text();
    QString response, questions;
    if(server::request_response(full_string, response))
    {
        qDebug() << "sending account";
        QMessageBox success_box;
        success_box.setText(response);
        success_box.exec();
        ui->lineEdit_email->setText("");
        ui->lineEdit_username_signup->setText("");
        ui->lineEdit_password1->setText("");
        ui->lineEdit_password2->setText("");

        // Request default questions from server
        ui->sign_up->setCurrentIndex(1);
        if(ui->comboBox_q2->count() == 0){
            qDebug() << "SENDING SECURITY QUESTIONS REQUEST";
            server::send(server::SECURITY_QUESTIONS+username);
        }
        return;
    }
    ui->sign_up->setCurrentIndex(0);
}
/*
 * Send security question and answers to server
*****/
void StartPage::on_save_question_btn_clicked()
{
    // Replaces all  spaces with underscores before sending to server
    QString a1 = ui->lineEdit_answer1->text().replace(' ', '_');
    QString a2 = ui->lineEdit_answer2->text().replace(' ', '_');
    QString a3 = ui->lineEdit_answer3->text().replace(' ', '_');


    // If there exists a custom question send all questions to server
    if(customQ_flag){
        //server::send(server::SECURITYQ_CUSTOM+username + ' ' + question[0] + ' ' + question[1]  + ' ' + question[2]);
    }
    server::send(server::SECURITY_ANSWERS + a1 + ' ' + a2 + ' ' + a3);  // Answers get sent regardless
}
void StartPage::set_questions(QList<QByteArray> message)
{
    QComboBox* t;
    // Loops through message and sets all three questions to each combobox
    for(int i = 1; i < message.size(); i++)
    {
        for(int j = 1; j < message.size(); j++)
        {
            t = parentWidget()->findChild<QComboBox*>("comboBox_q" + QString::number(i));
            t->addItem(QString(message[j]).replace('_', ' '));
        }
        t->addItem("Create custom question");
        custom_questions.append(nullptr);   // appends null for custom question QlineEdits
    }

}
/*
 *  When user chooses custom question the custom flag is set to true
 *  and the current combobox is hidden and replaced with a QLineEdit
********/
void StartPage::on_comboBox_q1_currentIndexChanged(int index)
{
    customQ_flag = true;
    QLineEdit* q = new QLineEdit();
    q->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
    if(index)
    {
        ui->comboBox_q1->hide();
        ui->layout_q1->addWidget(q);
    }
}

void StartPage::on_comboBox_q2_currentIndexChanged(int index)
{
    customQ_flag = true;
    QLineEdit* q = new QLineEdit();
    q->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
    if(index)
    {
        ui->comboBox_q2->hide();
        ui->layout_q2->addWidget(q);
    }
}
void StartPage::on_comboBox_q3_currentIndexChanged(int index)
{
    customQ_flag = true;
    QLineEdit* q = new QLineEdit();
    q->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
    if(index)
    {
        ui->comboBox_q3->hide();
        ui->layout_q3->addWidget(q);
    }
}

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

void StartPage::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid)
{
    QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    this_label->setPixmap(mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this_label->show();
}

/*****************************************************************
 * ACCOUNT RECOVERY
 */
void StartPage::on_pushButton_recover_user_clicked()
{
    recover->display_recovery_page(0);
    ui->recover_account->addWidget(recover);
    ui->recover_account->setCurrentWidget(recover);
}

void StartPage::on_pushButton_recover_pass_clicked()
{
    recover->display_recovery_page(1);
    ui->recover_account->addWidget(recover);
    ui->recover_account->setCurrentWidget(recover);
}

void StartPage::on_tabWidget_tabBarClicked(int index)
{
    /*
     * NOT SURE ABOUT THIS FEATURE
     * Asks the user whether they want to skip security questions if they click away from
     * security question tab
    if(ui->tabWidget->currentIndex() == 1)
    {
        QMessageBox message_box;
        int user_reply = message_box.warning(0, "Skip Security Questions", "Are you sure you want to skip security questions?", "I want to be safe", "Yes, risk it");
        if(user_reply){
            ui->tabWidget->setCurrentIndex(0);
        }
    }
    */
    recover->clear_text();
    ui->recover_account->removeWidget(recover);
    ui->recover_account->setCurrentWidget(ui->recover_acct_page);
}



