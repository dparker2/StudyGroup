#include "startpage.h"
#include "ui_startpage.h"
#include "server.h"
#include "joingrouppage.h"
#include <QMessageBox>
#include <QListView>
#include <QStandardItemModel>

StartPage::StartPage(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit
    //QPixmap pencil(":/resources/img/edit_pencil.png");

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);

    // Account Security
    recover_username = new RecoverUsername("recover username");
    //customQ_flag = false;


}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::do_work()
{
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QString code = message.left(4);
        message.remove(0,4);
        QList<QByteArray> message_list = split(message,3);
        if (code == "REQQ")
        {
            qDebug() << "MESSAGE LIST:  " << message_list;
            REQQ(message_list);
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
        if(questions.isEmpty()){
            server::send(server::SECURITY_QUESTIONS+username);
        }
        return;
    }
    ui->sign_up->setCurrentIndex(0);
}

/*
 * Load the security question comboboxes with questions sent
 * from the server
******/
void StartPage::REQQ(QList<QByteArray> message)
{
    QComboBox* comboBox;
    for(int i = 0; i < 3; i++){
        comboBox = parentWidget()->findChild<QComboBox*>("comboBox_q" + QString::number(i+1));
        question_comboBox.push_back(comboBox);
    }

    questions.push_back("Choose Security Question");
    for(int i = 0; i < message.size(); i++)
    {
        questions.push_back(QString(message[i]).replace('_', ' '));
        custom_questions.push_back(nullptr);
    }
     questions.push_back("Create Custom Question");

     qDebug() << "QUESTOINS : " << questions;
    for(int i = 0; i < question_comboBox.size(); i++)
    {
        for(int j = 0; j < questions.size(); j++)
        {
            question_comboBox[i]->addItem(questions[j]);
        }
    }
}

void StartPage::set_questions(QList<QByteArray> message)
{

}

/*
 * Send security question and answers to server
*****/
void StartPage::on_save_question_btn_clicked()
{
    // Replaces all  spaces with underscores before sending to server
    QString questions_to_send, answers_to_send;

    for(int i = 0; i < custom_questions.size(); i++){
        if(custom_questions[i] != nullptr)
        {
            questions[i+1] = custom_questions[i]->text();
        }
    }
    for(int i = 1; i < questions.size()-1; i++){
        questions_to_send += questions[i].replace(' ', '_') + " ";
    }

    answers_to_send = ui->lineEdit_answer1->text().replace(' ', '_') + " " + ui->lineEdit_answer2->text().replace(' ', '_') + " " + ui->lineEdit_answer3->text().replace(' ', '_');


    // If there exists a custom question send all questions to server
    if(customQ_flag)
    {
        server::send(server::SECURITYQ_CUSTOM+username + " " + questions_to_send);
    }
    server::send(server::SECURITY_ANSWERS+username + " " + answers_to_send);  // Answers get sent regardless
}

/*
 *  When user chooses custom question the custom flag is set to true
 *  and the current combobox is hidden and replaced with a QLineEdit
********/
void StartPage::on_comboBox_q1_activated(int index)
{
    //customQ_flag = false;
    if(index == question_comboBox[0]->count()-1)  // Custom question is picked
    {
        custom_questions[0] = new QLineEdit();
        custom_questions[0]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;  // Just in case I need the flag for something not sure yet
        ui->comboBox_q1->hide();
        ui->layout_q1->addWidget(custom_questions[0]);
    }
    else{
        update_question_list(index, ui->comboBox_q1);
    }
}

void StartPage::update_question_list(int current_index, QComboBox* current_comboBox)
{
    QList<QComboBox*> other_comboBox;           // pointer for combobox not currently selected
    QList<QString> other_selected_question;     // selected questions from the other comboboxes
    QList<int> other_selected_index;            // indexes of selected questions from other combo boxes

    for(int i = 0; i < question_comboBox.size(); i++)   // loops the number of comboboxes in UI (3)
    {
       if(question_comboBox[i] != current_comboBox)     // catches the other two comboboxes
       {
           other_comboBox.push_back(question_comboBox[i]);
       }
    }
    QString current_question = current_comboBox->currentText();   // the one currently selected

    for(int j = 0; j < other_comboBox.size(); j++){
        other_selected_question.push_back(other_comboBox[j]->currentText());   // pushes back the questions selected at two other comboboxes
        other_selected_index.push_back(other_comboBox[j]->currentIndex());
        other_comboBox[j]->clear();                 // clears all questions
        other_comboBox[j]->addItems(questions);     // adds them all back in, ready to remove the ones that are currently selected
    }

    int j = 1;
    for(int i = 0; i < other_comboBox.size(); i++)
    {
        // removes the currently selected question from the two other comboxes
        if(current_index != 0){
            other_comboBox[i]->removeItem(other_comboBox[i]->findText(current_question));
        }
        if(other_selected_index[j] != 0 && other_selected_index[j] != questions.size()-1){
            other_comboBox[i]->removeItem(other_comboBox[i]->findText(other_selected_question[j]));
        }
        j--;
    }
    // Finding the new index of selected questions and setting that index to be selected
    int new_index1 = other_comboBox[0]->findText(other_selected_question[0]);
    int new_index2 = other_comboBox[1]->findText(other_selected_question[1]);
    other_comboBox[0]->setCurrentIndex(new_index1);
    other_comboBox[1]->setCurrentIndex(new_index2);
}
void StartPage::on_comboBox_q2_activated(int index)
{
    if(index == question_comboBox[1]->count()-1)
    {
        custom_questions[1] = new QLineEdit();
        custom_questions[1]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;
        ui->comboBox_q2->hide();
        ui->layout_q2->addWidget(custom_questions[1]);
    }
    else{
        update_question_list(index, ui->comboBox_q2);
    }

}
void StartPage::on_comboBox_q3_activated(int index)
{
    if(index == question_comboBox[2]->count()-1)
    {
        custom_questions[2] = new QLineEdit();
        custom_questions[2]->setStyleSheet("background-color: #545454; color:white;border-style:none; height: 33px; width: 316px");
        customQ_flag = true;
        ui->comboBox_q3->hide();
        ui->layout_q3->addWidget(custom_questions[2]);
    }
    else
    {
        update_question_list(index, ui->comboBox_q3);
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
    ui->recover_account->addWidget(recover_username);
    ui->recover_account->setCurrentWidget(recover_username);
}

void StartPage::on_pushButton_recover_pass_clicked()
{
    //recover->display_recovery_page(1);
    //ui->recover_account->addWidget(recover);
    //ui->recover_account->setCurrentWidget(recover);
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
    recover_username->clear_text();
    ui->recover_account->removeWidget(recover_username);
    ui->recover_account->setCurrentWidget(ui->recover_acct_page);
}






