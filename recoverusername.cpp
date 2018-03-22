#include "recoverusername.h"
#include "ui_recoverusername.h"
#include "server.h"
#include <QMessageBox>
#include <QLabel>

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
            //RUSR(QString(message_list[1]));
        }
    }
}
void RecoverUsername::RUSR(QString email_msg)
{
    // don't think this is needed
}

void RecoverUsername::on_pushButton_recover_username_clicked()
{
    if(ui->lineEdit_email->text().isEmpty()){
        set_invalid_icon();
        return;
    }
    QString succs_msg;
    if(server::request_response(server::RECOVER_USERNAME + ui->lineEdit_email->text(), succs_msg))
    {
        qDebug() << "Recover username for:  " << ui->lineEdit_email->text();
        QMessageBox succs_box;
        succs_box.information(0, "Email Sent", succs_msg);
    }
    else{
        set_invalid_icon();
    }
}

void RecoverUsername::clear_text()
{
    ui->lineEdit_email->clear();
    ui->label_invalid->clear();
}
void RecoverUsername::set_invalid_icon()
{
    QPixmap x_mark = QPixmap(":/resources/img/x_mark.png");
    ui->label_invalid->setPixmap(x_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_invalid->show();
}

void RecoverUsername::on_email_lineEdit_textEdited(const QString &arg1)
{
    ui->label_invalid->clear();
}

