#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_account_check.h"
#include "server.h"
#include <QPixmap>
#include <QString>
#include <QDebug>
#include <QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit

    QPixmap logo2(":/resources/img/GSLogoName2.png");    // StudyGroup2 logo
    ui->inner_label_logo->setPixmap(logo2.scaled(500,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_username_error->hide();
    ui->label_email_check->hide();

    my_serv = new server();
    my_serv->connect_server();
    user_info = new UserAccount();

    // UI Connections
    connect(ui->exit_settings_button, SIGNAL(released()), this, SLOT(exit_settings()));

    // TEST STUFF
    std::vector<QString> testusers1;
    testusers1.push_back("TestName1"); testusers1.push_back("XxXNoScopeTest"); testusers1.push_back("LelNameTest");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_signin_button_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(my_serv->login(username, password)) {
        // Now logged in!
        user_info->setUsername(username);
        user_info->setPassword(password);
        ui->stackedWidget_window->setCurrentWidget(ui->main_page); // Change main page
    }
}

void MainWindow::on_singup_button_clicked()
{
    user_info->printReadyState();

    bool ready_to_send = true;
    int size = 3;

    for(int i = 0; i < size; i++){
        //qDebug() << user_info->get_info_complete(i);
        if(user_info->get_info_complete(i) == false){
            ready_to_send = false;
            break;
        }
    }
    QString password = user_info->getPassword();
    if(ready_to_send){
        QString email = user_info->getEmail();
        QString username = user_info->getUsername();


        my_serv->create_account(email, username, password);

        qDebug() << "Ready To Send";
    }
    else{
        qDebug() << "ready? "<< ready_to_send; // test
        // error message sign up form not complete?
    }
}
/*
 * Sign Up Check - Username
 * Checks the username entered by the user and sets the green check/X icon
 * accordingly. If username is valid it is set to the UserAccount email
 */
void MainWindow::on_lineEdit_username_signup_editingFinished()
{
    QString username = ui->lineEdit_username_signup->text();
    QString error_msg;
    bool valid = user_info->usernameValidation(username,  error_msg);   // returns if username is valid or not
                                                                        // with error msg if not valid
    //qDebug() << valid;   testing
    if(username.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_username_signup->setStyleSheet("color:black; background-color:white");
    }
    if(valid){                         // sets valid username to UserAccounts username member
        user_info->setUsername(username);
        user_info->set_info_complete(1,1);
        set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, error_msg, valid);
    }
    // sets correct icon
    // green: valid, X otherwise

}
/*
 * Hides the icons when the user edits the line
 */
void MainWindow::on_lineEdit_username_signup_textEdited()
{
    ui->label_username_check->hide();
    //ui->label_username_error->hide();
    user_info->set_info_complete(1,0);
    on_lineEdit_username_signup_cursorPositionChanged();
}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void MainWindow::on_lineEdit_username_signup_cursorPositionChanged()
{
    ui->lineEdit_username_signup->setStyleSheet("color:black; background-color:white");
}
/*
 * Sign Up Check - Email
 * Checks the email entered by the user and sets the green check/X icon
 * accordingly. If email is valid it is set to the UserAccount email
 */
void MainWindow::on_lineEdit_email_editingFinished()
{

    QString email = ui->lineEdit_email->text();
    QString error_msg;
    bool valid = user_info->emailValidation(email, error_msg);      // returns if email is valid or not
                                                                    // with error msg if not valid
    if(email.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_email->setStyleSheet("color:black; background-color:white");
    }
    else if(valid){                      // sets valid email to UserAccounts email member
        user_info->setEmail(email);
        user_info->set_info_complete(0,1);
        // sets correct icon
        // green: valid, X otherwise
        set_valid_icons(ui->label_email_check, ui->lineEdit_email, error_msg, valid);
    }
}
/*
 * Hides the icons when the user edits the line
 */
void MainWindow::on_lineEdit_email_textEdited()
{
    user_info->set_info_complete(0,0);
    ui->label_email_check->hide();               // hides the red X when user begins editing text
    on_lineEdit_email_cursorPositionChanged();   // resets color when editing text

}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void MainWindow::on_lineEdit_email_cursorPositionChanged()
{
    ui->lineEdit_email->setStyleSheet("color: black; background-color:white");
}
/*
 * Sign Up Check - Password
 * Checks the password entered by the user and sets the green check/X icon
 * accordingly. If password is valid it is set to the UserAccount password
 */
void MainWindow::on_lineEdit_password1_editingFinished()
{
    QString password = ui->lineEdit_password1->text();
    QString error_msg;
    bool valid = user_info->passwordValidtion(password,  error_msg);       // returns if password is valid or not
                                                                            // with error msg if not valid
    if(password.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_password1->setStyleSheet("color:black; background-color:white");
    }
    else if(valid){                        // sets valid password to UserAccounts password member
        user_info->setPassword(password);
        // sets correct icon
        // green: valid, X otherwise
        set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, error_msg, valid);
    }

}
/*
 * Hides the icons when the user edits the line
 */
void MainWindow::on_lineEdit_password1_textEdited()
{
    user_info->set_info_complete(2,0);
    ui->label_password1_check->hide();               // hides the red X when user begins editing text
    on_lineEdit_password1_cursorPositionChanged();   // resets color when editing text
}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void MainWindow::on_lineEdit_password1_cursorPositionChanged()
{
    ui->lineEdit_email->setStyleSheet("color: black; background-color:white");
}
/*
 * Sign Up Check - Password (Second Input)
 * Checks the password entered by the user and sets the green check/X icon
 * accordingly. If password is valid it is set to the UserAccount password
 */
void MainWindow::on_lineEdit_password2_editingFinished()
{
    QString password = ui->lineEdit_password2->text();
    if(password.isEmpty()){
        ui->lineEdit_password2->setStyleSheet("color: black; background-color: white");
    }
    else if(password == user_info->getPassword()){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, "", 1);
        user_info->set_info_complete(2, 1);    // since it matches first entered password
                                               // info_complete[2] is set to true (ready to send)
    }
    else{
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, "", 0);
        user_info->set_info_complete(2,0);
    }
}
void MainWindow::on_lineEdit_password2_textEdited()
{

    ui->label_password2_check->hide();               // hides the red X when user begins editing text
    user_info->set_info_complete(2,0);
    on_lineEdit_password2_cursorPositionChanged();   // resets color when editing text
}
void MainWindow::on_lineEdit_password2_cursorPositionChanged()
{
    ui->lineEdit_password2->setStyleSheet("color: black; background-color:white");
}
/*
 * Sets the stylesheet colors for invalid input
 */
void MainWindow::invalid_label_stylesheet(QLabel* this_label, QLineEdit *this_line, QString error_msg){
    this_label->show();                           // becomes visible (hidden in constructor)
    this_label->setStyleSheet("color:white");
    this_label->setText(error_msg);
    this_line->setStyleSheet("color:white; background-color: rgb(20, 230, 180)");
}
/*
 * Sets green check/X accordingly with given valid input. Outputs error message if needed
 */
void MainWindow::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid){
    if(valid){
        QPixmap check_mark(":/resources/img/check_mark.png");
        this_label->setPixmap(check_mark.scaled(31,31,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        this_line->setStyleSheet("color: black; background-color:white");
        this_label->show();
     }
     else{
        invalid_label_stylesheet(this_label, this_line, error_msg);
        QPixmap x_mark(":/resources/img/x_mark.png");
        this_label->setPixmap(x_mark.scaled(20,20,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        this_label->show();
     }
}

void MainWindow::on_settings_button_released()
{
    exit_settings_to = ui->stackedWidget_inner->currentWidget(); // Save previous page to exit to after
    ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_Settings); // Change active page to settings
}

void MainWindow::exit_settings()
{
    ui->stackedWidget_inner->setCurrentWidget(exit_settings_to); // Go back to previously active page
}

void MainWindow::on_join_button_released()
{
    ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
}

void MainWindow::on_create_button_released()
{
    ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_CreateGroup);
}

void MainWindow::on_create_group_button_released()
{
    QString group_name = ui->create_group_lineEdit->text();
    QString group_id = group_name + "_0000";
    if(my_serv->create_group(group_name, group_id))
    {
        group_widget = new GroupWidget();
        ui->stackedWidget_inner->addWidget(group_widget);
        ui->stackedWidget_inner->setCurrentWidget(group_widget);
        QString name = user_info->getUsername();
        group_widget->user_joined(name);
        QString chat = "12:43am - User1: Hello!";
        group_widget->new_chat(chat);

        connect(my_serv, SIGNAL(user_joined(QString)), group_widget, SLOT(user_joined(QString)));
        connect(my_serv, SIGNAL(users_changed()), group_widget, SLOT(users_changed()));
    }

}

void MainWindow::on_join_group_button_released()
{
    QString group_id = ui->join_group_lineEdit->text();
    if(my_serv->join_group(group_id))
    {
        group_widget = new GroupWidget();
        ui->stackedWidget_inner->addWidget(group_widget);
        ui->stackedWidget_inner->setCurrentWidget(group_widget);
        QString name = user_info->getUsername();
        group_widget->user_joined(name);
        QString chat = "12:43am - User1: Hello!";
        group_widget->new_chat(chat);

        connect(my_serv, SIGNAL(user_joined(QString)), group_widget, SLOT(user_joined(QString)));
        connect(my_serv, SIGNAL(users_changed()), group_widget, SLOT(users_changed()));
    }
}
