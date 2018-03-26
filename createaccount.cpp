#include "createaccount.h"
#include "ui_createaccount.h"
#include "server.h"
#include <QRegExp>
#include <QMessageBox>



CreateAccount::CreateAccount(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::CreateAccount)
{
    ui->setupUi(this);
    //initialize_validation_variables(false);
    security_questions = new SecurityQuestions();
    ui->questions_layout->addWidget(security_questions);

    server::send(server::SECURITY_QUESTIONS); // should send at initial start of program

    //ui->register_btn->setEnabled(false);
}

CreateAccount::~CreateAccount()
{
    delete ui;
}
void CreateAccount::do_work()
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
            security_questions->REQQ(message_list);
        }
    }
}
/*****************************************************
 * ACCOUNT SIGNUP
 */

void CreateAccount::on_register_btn_clicked()
{
    QString username = ui->lineEdit_username_signup->text();
    QString full_string = server::CREATE_ACCOUNT + ui->lineEdit_email->text() +
            " " + username +
            " " + ui->lineEdit_password2->text();
    QString response, questions;
    qDebug() << "Info Valid.." << info_valid_status();

    QString security_question_msg;
    bool ready = security_questions->ready_to_send(security_question_msg);

    if(info_valid_status() && server::request_response(full_string, response))
    {
        qDebug() << "sending account";
        QMessageBox success_box;
        success_box.setText(response);
        success_box.exec();
        ui->lineEdit_email->setText("");
        ui->lineEdit_username_signup->setText("");
        ui->lineEdit_password1->setText("");
        ui->lineEdit_password2->setText("");
    }
}
void CreateAccount::on_lineEdit_email_textChanged(const QString &email)
{
    ui->label_email_check->clear();
    bool valid = validate_email(email);
    if(valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, valid);
    }
}
void CreateAccount::on_lineEdit_email_editingFinished()
{
    QString email = ui->lineEdit_email->text();
    bool valid = get_email_status();
    if(!email.isEmpty() && !valid){
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, valid, "Email not valid");
    }
}
void CreateAccount::on_lineEdit_username_signup_textChanged(const QString &username)
{
    ui->label_username_check->clear();
    bool valid = validate_username(username);
    if(valid){
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, valid);
    }
}

void CreateAccount::on_lineEdit_username_signup_editingFinished()
{
    QString username = ui->lineEdit_username_signup->text();
    bool valid = get_username_status();
    if(!username.isEmpty() && !valid){
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, valid, get_error_msg());
    }
}

void CreateAccount::on_lineEdit_password1_textChanged(const QString &password1)
{
    ui->label_password1_check->clear();
    bool valid = validate_password(ui->lineEdit_password1, ui->lineEdit_password2, true);
    if(!password1.isEmpty() && valid){
        if(ui->lineEdit_password1->text() == ui->lineEdit_password2->text()){
            set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, valid);
        }
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, valid);
    }
}

void CreateAccount::on_lineEdit_password1_editingFinished()
{
    QString password1 = ui->lineEdit_password1->text();
    bool valid1 = get_password1_status();
    if(!password1.isEmpty() && !valid1){
        qDebug() << "HEY LOOK AT ME";
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, valid1, get_error_msg());
    }

}

void CreateAccount::on_lineEdit_password2_textChanged(const QString &password2)
{
    ui->label_password2_check->clear();
    bool valid = validate_password(ui->lineEdit_password2, ui->lineEdit_password1, false);
    if(!password2.isEmpty() && valid){
        if(ui->lineEdit_password1->text() == ui->lineEdit_password2->text()){
            set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, valid);
        }
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, valid);
    }
}

void CreateAccount::on_lineEdit_password2_editingFinished()
{
    QString password2 = ui->lineEdit_password2->text();
    bool valid2 = get_password2_status();
    if(!password2.isEmpty() && !valid2){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, valid2, get_error_msg());
    }
}

void CreateAccount::set_valid_icons(QLabel* this_label, QLineEdit* this_line, bool valid, QString error_msg)
{
    if(!error_msg.isEmpty())
    {
        // Qt bug fix so it wont display prompt twice for textchanged/editing finished signals
        if(this_line->text().toDouble()<1000)
        {
            this_line->blockSignals(true);
            QMessageBox error_box;
            error_box.information(0, "Error Message", error_msg);
            this_line->blockSignals(false);
        }
    }
    QPixmap mark = valid ? QPixmap(":/resources/img/check_mark.png") : QPixmap(":/resources/img/x_mark.png");
    this_label->setPixmap(mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this_label->show();
}



