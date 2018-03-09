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

void RecoverUsername::on_recover_username_btn_clicked()
{
    if(ui->email_lineEdit->text().isEmpty()){
        set_invalid_icon();
        return;
    }
    QString succs_msg;
    if(server::request_response(server::RECOVER_USERNAME + ui->email_lineEdit->text(), succs_msg))
    {
        qDebug() << "Recover username for:  " << ui->email_lineEdit->text();
        QMessageBox succs_box;
        succs_box.information(0, "Email Sent", succs_msg);
    }
    else{
        set_invalid_icon();
    }
}

void RecoverUsername::clear_text()
{
    ui->email_lineEdit->clear();
    ui->invalid_icon_label->clear();
}
void RecoverUsername::set_invalid_icon()
{
    QPixmap x_mark = QPixmap(":/resources/img/x_mark.png");
    ui->invalid_icon_label->setPixmap(x_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->invalid_icon_label->show();
}

void RecoverUsername::on_email_lineEdit_textEdited(const QString &arg1)
{
    ui->invalid_icon_label->clear();
}
