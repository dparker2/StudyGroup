#include "accountsecurity.h"
#include "ui_accountsecurity.h"
#include "server.h"

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
        QList<QByteArray> message_list = split(message, 2);
        if (message_list[0] == "RECQ")
        {
            //RECQ(QString(message_list[1]));
        }
    }
}

void AccountSecurity::display_recovery_page(int index){
    ui->account_recovery->setCurrentIndex(index);
}
void AccountSecurity::RECQ(QString email_sent)
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
    QString email = ui->recover_pass_email->text();
    server::send(server::RECOVER_USERNAME+email);
}
