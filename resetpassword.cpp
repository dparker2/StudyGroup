#include "resetpassword.h"
#include "ui_resetpassword.h"
#include "server.h"
#include <QComboBox>

ResetPassword::ResetPassword(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::ResetPassword)
{
    ui->setupUi(this);
}

ResetPassword::~ResetPassword()
{
    delete ui;
}

void ResetPassword::do_work(){
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message,3);
        if (message_list[0] == "RPWD")
        {
            RPWD(message_list[1].toInt(), QString(message_list[2]));
        }
    }
}

void ResetPassword::RPWD(int index, QString random_question){
    question_index = index;
    security_question = random_question;
}

void ResetPassword::on_next_btn_clicked()
{
    username = ui->username_lineEdit->text();
    if(username.isEmpty()){
        //add ui invalid icon
        return;
    }
    QString email_message;
    if(server::request_response(server::RECOVER_PASSWORD + username, email_message)){
        QComboBox succs_box;
        succs_box.information(0, "Check Email", email_message);
        ui->reset_password->setCurrentIndex(1);
    }
}

void ResetPassword::on_submit_answer_btn_clicked()
{
    if(!ui->answer_lineEdit->text().isEmpty()){
        server::send(server::CHECK_SECURITY_ANSWER + username + " " + question_index + " " + ui->answer_lineEdit->text());
        ui->reset_password->setCurrentIndex(2);
    }
}

void ResetPassword::on_verify_code_btn_clicked()
{
    if(!ui->email_code_lineEdit->text().isEmpty()){
        server::send(server::CHECK_SECURITY_CODE + username + " " + ui->email_code_lineEdit->text());
        ui->reset_password->setCurrentIndex(3);
    }
}

void ResetPassword::on_confirm_pass_lineEdit_clicked()
{
    bool empty_field = (ui->new_pass_lineEdit->text().isEmpty() || ui->confirm_pass_lineEdit->text().isEmpty());
    bool passwords_match = (ui->new_pass_lineEdit->text() == ui->confirm_pass_lineEdit->text());

    if(!empty_field && passwords_match){
        server::send(server::UPDATE_PASSWORD + username + " " + ui->new_pass_lineEdit->text());
    }
}
