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

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);

    // Account Security
    recover_username = new RecoverUsername("recover username");
    reset_password = new ResetPassword("reset password");
    //customQ_flag = false;


    //sign_up = new CreateAccount("create account");
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
void StartPage::on_pushButton_recover_user_clicked()
{
    ui->recover_account->addWidget(recover_username);
    ui->recover_account->setCurrentWidget(recover_username);
    ui->tab_recover_account->setContentsMargins(0, 0, -20, 0);
}
void StartPage::on_reset_password_btn_clicked()
{
    ui->recover_account->addWidget(reset_password);
    ui->recover_account->setCurrentWidget(reset_password);
    ui->tab_recover_account->setContentsMargins(0, 0, -20, 0);
}

void StartPage::on_tabWidget_tabBarClicked(int index)
{
    /*
     * NOT SURE ABOUT THIS FEATURE
     * Asks the user whether they want to skip security questions if they click away from
     * security question tab
    if(ui->tabWidget->currentIndex() == 1)
    {
        QMessageBox message_box;
        int user_reply = message_box.warning(0, "Skip Security Questions", "Are you sure you want to skip security questions?", "I want to be safe", "Yes, risk it");
        if(user_reply){
            ui->tabWidget->setCurrentIndex(0);
        }
    }
    */


}

void StartPage::on_tabWidget_currentChanged(int index)
{
    if(index == 0){
        ui->tabWidget->setMaximumWidth(406);
        ui->tabWidget->setMaximumHeight(372);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
    else if(index == 1){
       // ui->recover_account->removeWidget();
        sign_up = new CreateAccount("create account");
        ui->sign_up->addWidget(sign_up);
        ui->sign_up->setCurrentWidget(sign_up);
        ui->tabWidget->setMaximumHeight(500);
        ui->tabWidget->setMaximumWidth(900);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
    else if(index == 2){
        recover_username->clear_text();
        ui->recover_account->removeWidget(recover_username);
        ui->recover_account->setCurrentWidget(ui->recover_acct_page);
        qDebug() << "resetting margins *** ";
        ui->recover_account->setContentsMargins(0,0,0,0);
        ui->tabWidget->setMaximumWidth(406);
        ui->tabWidget->setMaximumHeight(372);
        ui->tabWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    }
}
