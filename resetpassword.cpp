#include "resetpassword.h"
#include "ui_resetpassword.h"
#include "server.h"
#include <QMessageBox>

ResetPassword::ResetPassword(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::ResetPassword)
{
    ui->setupUi(this);
    //ui->reset_password->setCurrentIndex(0);
}

ResetPassword::~ResetPassword()
{
    delete ui;
}

void ResetPassword::do_work(){
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QString code = message.left(4);
        message.remove(0, 4);
        QList<QByteArray> message_list = split(message,2);
        qDebug() << "in recover password";
        if (code == "RPWD")
        {
            RPWD(message_list[0].toInt(), QString(message_list[1]));
        }
    }
}
void ResetPassword::clear_text()
{
    ui->username_lineEdit->clear();
    ui->answer_lineEdit->clear();
    ui->confirm_pass_lineEdit->clear();
    ui->email_code_lineEdit->clear();
}
void ResetPassword::RPWD(int index, QString random_question)
{
    qDebug() << "Setting random question...";
    qDebug() << "Received index: " << index;
    question_index = index;
    qDebug() << "Question_index: " << question_index;
    security_question = random_question;
    ui->random_question_label->setText(random_question.replace('_', ' '));
    ui->reset_password->setCurrentIndex(1);
}

void ResetPassword::on_next_btn_clicked()
{
    qDebug() << "Sending username...";
    username = ui->username_lineEdit->text();
    if(username.isEmpty()){
        set_invalid_icon(ui->username_invalid_icon);
        return;
    }
    QString email_message;
    if(server::request_response(server::RECOVER_PASSWORD + username, email_message)){
        QMessageBox succs_box;
        succs_box.information(0, "Check Email", email_message);
        ui->reset_password->setCurrentIndex(1);
    }
    else{
        set_invalid_icon(ui->username_invalid_icon);
    }
}

void ResetPassword::on_submit_answer_btn_clicked()
{
    qDebug() << "Sending answer...";
    QString answer = ui->answer_lineEdit->text().replace(' ', '_');
    if(answer.isEmpty()){
        return;
    }
    QString answers_matched;
    qDebug() << "Sending Index: " << question_index;
    if(server::request_response(server::CHECK_SECURITY_ANSWER + username + " " + QString::number(question_index) + " " + answer, answers_matched)){
        qDebug() << "Answers match: " << answers_matched;
        ui->reset_password->setCurrentIndex(2);
    }
}

void ResetPassword::on_verify_code_btn_clicked()
{
    qDebug() << "Verifying security code...";
    QString code = ui->email_code_lineEdit->text();
    if(code.isEmpty()){
        return;
    }
    QString code_verified;
    if(server::request_response(server::CHECK_SECURITY_CODE + username + " " + ui->email_code_lineEdit->text(), code_verified)){
        qDebug() << "Code verified: " << code_verified;
        ui->reset_password->setCurrentIndex(3);
    }
}

void ResetPassword::on_reset_password_btn_clicked()
{
    qDebug() << "Resetting password...";
    bool empty_field = (ui->new_pass_lineEdit->text().isEmpty() || ui->confirm_pass_lineEdit->text().isEmpty());
    bool passwords_match = (ui->new_pass_lineEdit->text() == ui->confirm_pass_lineEdit->text());

    if(empty_field || !passwords_match){
        return;
    }
    QString password_reset;
    if(server::request_response(server::UPDATE_PASSWORD + username + " " + ui->new_pass_lineEdit->text(), password_reset)){
        QMessageBox succs_box;
        succs_box.information(0, "Check Email", password_reset);
        qDebug() << "Password reset: " << password_reset;
        clear_text();
        // back to login
     }
}

void ResetPassword::set_invalid_icon(QLabel *mark)
{
    QPixmap x_mark = QPixmap(":/resources/img/x_mark.png");
    mark->setPixmap(x_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    mark->show();
}

void ResetPassword::on_username_lineEdit_textEdited(const QString &arg1)
{
    ui->username_invalid_icon->clear();
}
