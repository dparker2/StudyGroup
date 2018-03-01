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
        QList<QByteArray> message_list = split(message, 2);
        qDebug() << "TESTING Recoveer Username: " << message_list[0];
        if (message_list[0] == "RUSR")
        {
            RUSR(QString(message_list[1]));
        }
    }
}

void AccountSecurity::display_recovery_page(int index)
{
    ui->account_recovery->setCurrentIndex(index);
}
// create security 3 questions when creating account
// sending questions and answers
// RECQUsername Q1 A1 Q2 A3

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
    ui->recover_pass_email->clear();
    //ui->recover_pass_username->clear();
    ui->recover_user_email->clear();
}

void AccountSecurity::on_recover_pass_btn_clicked()
{
    ui->account_recovery->setCurrentIndex(2);
}
