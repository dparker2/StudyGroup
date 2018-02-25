#include "accountsecurity.h"
#include "ui_accountsecurity.h"
#include "server.h"

#include <QDebug>
#include <QMessageBox>

AccountSecurity::AccountSecurity(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::AccountSecurity)
{
    ui->setupUi(this);
    ui->comboBox->hide();
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->recover_pass_email_2->hide();
    ui->recover_pass_email_3->hide();
    ui->recover_pass_email_4->hide();
}

AccountSecurity::~AccountSecurity()
{
    delete ui;
}
void AccountSecurity::do_work()
{
    qDebug() << "IN DO WORK " << _work_queue.isEmpty() ;
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        qDebug() << "MESSAGE IN DO WORK: " << message;
        QList<QByteArray> message_list = split(message, 2);
        if (message_list[0] == "RUSR")
        {
            //RUSR(QString(message_list[1]));
        }
    }
}

void AccountSecurity::display_recovery_page(int index){
    ui->account_recovery->setCurrentIndex(index);
}
void AccountSecurity::RUSR(QString email_sent)
{

    if(email_sent == "SUCC"){
        //QMessageBox username_box;
        //username_box.information(0, "Username Recovery", "\nUsername was sent to email\n");
    }
    else{
        //QMessageBox error_box;
        //error_box.critical(0, "Username Recovery", "\nEmail does not exist\n");
        //error_box.setFixedSize(500,200);
    }
}

// create security 3 questions when creating account
// sending questions and answers
// RECQUsername Q1 A1 Q2 A3

void AccountSecurity::on_recover_user_btn_clicked()
{
    QString response_msg;
    QString email = ui->recover_user_email->text();
    server::request_response(server::RECOVER_USERNAME+email, response_msg);

    QMessageBox message_box;

    if (response_msg.startsWith('1')){
        message_box.information(0, "Recover Username", response_msg.remove(0,1));
    }

}
void AccountSecurity::clear_text(){
    ui->recover_pass_email->clear();
    ui->recover_pass_username->clear();
    ui->recover_user_email->clear();
}

void AccountSecurity::on_recover_pass_btn_clicked()
{
    ui->account_recovery->setCurrentIndex(2);
    ui->comboBox->show();
    ui->comboBox_2->show();
    ui->comboBox_3->show();
    ui->recover_pass_email_2->show();
    ui->recover_pass_email_3->show();
    ui->recover_pass_email_4->show();

    emit expand_tabwidget();
}
