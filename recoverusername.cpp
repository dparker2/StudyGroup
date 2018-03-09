#include "recoverusername.h"
#include "ui_recoverusername.h"
#include "server.h"
#include <QMessageBox>

RecoverUsername::RecoverUsername(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::RecoverUsername)
{
    ui->setupUi(this);
}

RecoverUsername::~RecoverUsername()
{
    delete ui;
}
void RecoverUsername::do_work(){
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message,2);
        if (message_list[0] == "RUSR")
        {
            RUSR(QString(message_list[1]));
        }
    }
}
void RecoverUsername::RUSR(QString email_msg){
    QMessageBox succs_box;
    succs_box.information(0, "Email Sent", email_msg);
}


void RecoverUsername::on_recover_username_btn_clicked()
{
    if(ui->email_lineEdit->text().isEmpty()){
        set_invalid_icon();
    }
    else{
        qDebug() << "Recover username for:  " << ui->email_lineEdit->text();
        server::send(server::RECOVER_USERNAME + ui->email_lineEdit->text());
    }
}

void RecoverUsername::clear_text(){
    ui->email_lineEdit->clear();
}
void RecoverUsername::set_invalid_icon(){
    QPixmap check_mark = QPixmap(":/resources/img/x_mark.png");

    //QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    //ui->invalid_icon_label->setPixmap(check_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //ui->invalid_icon_label->show();
}
