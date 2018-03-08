#include "accountsecurity.h"
#include "ui_accountsecurity.h"
#include "server.h"

#include <QDebug>
#include <QMessageBox>
#include <QPair>

AccountSecurity::AccountSecurity(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::AccountSecurity)
{
    ui->setupUi(this);
}

AccountSecurity::~AccountSecurity()
{
    delete ui;
}
void AccountSecurity::do_work()
{
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message, 3);
        qDebug() << "TESTING Recoveer Username: " << message_list[0];
        if (message_list[0] == "RUSR")
        {
            RUSR(QString(message_list[1]));
        }
        if (message_list[0] == "RPWD")
        {
            RPWD(message_list[1].toInt(), QString(message_list[2]));
        }
    }
}

void AccountSecurity::display_recovery_page(int index)
{
    ui->account_recovery->setCurrentIndex(index);
}

void AccountSecurity::on_recover_user_btn_clicked()
{
    QString response_msg;
    QString email = ui->recover_user_email->text();
    server::send(server::RECOVER_USERNAME+email);
}
void AccountSecurity::RUSR(QString email_sent)
{
    QMessageBox message_box;
    message_box.information(0, "Recover Username", email_sent);
}
void AccountSecurity::clear_text(){
    //ui->recover_pass_email->clear();
    //ui->recover_pass_username->clear();
    ui->recover_user_email->clear();
}

void AccountSecurity::on_recover_pass_btn_clicked()
{
    ui->account_recovery->setCurrentIndex(2);
}

void AccountSecurity::on_next_btn_clicked()
{
    if(ui->username_lineEdit->text().isEmpty()){
        qDebug() << "Username field is blank";
    }
    else{
        server::send(server::RECOVER_PASSWORD+ui->username_lineEdit->text());
        ui->account_recovery->setCurrentIndex(2);
    }
    ui->random_securityQ_label->setText("random question");

}

void AccountSecurity::on_request_code_btn_clicked()
{
    QString securityQ_answer = ui->securityQ_answer->text();
    if(securityQ_answer.isEmpty()){
        qDebug() << "answer field is blank";
    }
    else{
        server::send("CHKA"+random_question_index+" "+random_question_answer);
        ui->account_recovery->setCurrentIndex(3);
    }
}

void AccountSecurity::on_submit_code_btn_clicked()
{
    QString code = ui->recover_code_lineEdit->text();
    if(code.isEmpty()){
        qDebug() << "answer field is blank";
    }
    else{
        //server::send("CHKC"+username + " " + code);
        ui->account_recovery->setCurrentIndex(4);
    }
}

void AccountSecurity::on_reset_password_btn_clicked()
{
    QString new_password = ui->new_password_lineEdit1->text();
    QString confirm_password = ui->new_password_lineEdit2->text();

    if(new_password.isEmpty() || confirm_password.isEmpty()){
        qDebug() << "answer field is blank";
    }
    else{
        server::send("UPWD"+random_question_index+" "+random_question_answer);
        ui->account_recovery->setCurrentIndex(5);
    }
}
void AccountSecurity::RPWD(int index, QString question)
{
    random_question_index = index;
    ui->random_securityQ_label->setText(question);
}
