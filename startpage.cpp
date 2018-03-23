#include "startpage.h"
#include "ui_startpage.h"
#include "server.h"
#include "joingrouppage.h"
#include <QMessageBox>
#include <QListView>
#include <QStandardItemModel>

StartPage::StartPage(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit
    //QPixmap pencil(":/resources/img/edit_pencil.png");

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();
    ui->pushButton_recover_back->hide();

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);

    // Account Security
    recover_username = new RecoverUsername("recover username");
    reset_password = new ResetPassword("reset password");

    //sign_up = new CreateAccount("create account");

    connect(reset_password, SIGNAL(display_login()), this, SLOT(display_login()));
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::do_work()
{

}

void StartPage::on_signin_button_clicked()
{
    QString email;  // Return parameter from the server response

    server::test("startpage", "this is from the signin slot");
    QString full_string = server::LOGIN + ui->lineEdit_username->text() + " " + ui->lineEdit_password->text();

    if(server::request_response(full_string, email))
    {
        // Now logged in!
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");
        qDebug() << "Email:" << email;
        // Set username and password
        //user_info->setUsername(username);                     // FIX THESE WHEN USER CLASS IS DONE
        //user_info->setPassword(password);
        // Update settings page
        //ui->settings_email->setText(email);                   // FIX THESE WHEN SETTINGS CLASS IS DONE
        //ui->settings_username->setText(user_info->getUsername());

        emit logged_in(0); // Change main page
    }
}

/*****************************************************************
 * ACCOUNT RECOVERY
 */
void StartPage::hide_recover_buttons()
{
    ui->pushButton_recover_user->hide();
    ui->pushButton_reset_password->hide();
}
void StartPage::show_recover_buttons()
{
    ui->pushButton_recover_user->show();
    ui->pushButton_reset_password->show();
}
void StartPage::display_recover_widget(QWidget* recover_widget)
{
    ui->recover_account->addWidget(recover_widget);
    ui->recover_account->setCurrentWidget(recover_widget);
}
void StartPage::on_pushButton_recover_user_clicked()
{
    hide_recover_buttons();
    ui->pushButton_recover_back->show();
    display_recover_widget(recover_username);
    ui->tab_recover_account->setContentsMargins(25, 0, 0, 0);
}

void StartPage::on_pushButton_reset_password_clicked()
{
    hide_recover_buttons();
    reset_password->clear_info();
    ui->pushButton_recover_back->show();
    display_recover_widget(reset_password);
    //ui->tab_recover_account->setContentsMargins(25, 0, 0, 0);
}
void StartPage::on_pushButton_recover_back_clicked()
{
    show_recover_buttons();
    ui->pushButton_recover_back->hide();
    ui->recover_account->removeWidget(ui->recover_account->currentWidget());
    ui->tab_recover_account->setContentsMargins(0, 0, 0, 0);
}


void StartPage::on_tabWidget_currentChanged(int index)
{
    if(index == 0){
        ui->tabWidget->setMaximumWidth(406);
        ui->tabWidget->setMaximumHeight(372);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
    else if(index == 1){
        sign_up = new CreateAccount("create account");
        ui->sign_up->addWidget(sign_up);
        ui->sign_up->setCurrentWidget(sign_up);
        ui->tabWidget->setMaximumHeight(500);
        ui->tabWidget->setMaximumWidth(900);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
    else if(index == 2){
        recover_username->clear_text();
        ui->recover_account->setContentsMargins(0,0,0,0);
        ui->tabWidget->setMaximumWidth(406);
        ui->tabWidget->setMaximumHeight(372);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
}

void StartPage::display_login(){
    ui->tabWidget->setCurrentIndex(0);
}



void StartPage::on_pushButton_clicked()
{
    qDebug() << "PRESS BUTTON";
    on_pushButton_recover_user_clicked();
    ui->tabWidget->setCurrentIndex(2);
}

void StartPage::on_pushButton_2_clicked()
{
    on_pushButton_reset_password_clicked();
    ui->tabWidget->setCurrentIndex(2);
}
