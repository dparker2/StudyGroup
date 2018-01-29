#include "startpage.h"
#include "ui_startpage.h"

StartPage::StartPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();

    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::on_signin_button_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString email;

    /*if(my_serv->login(username, password, email))
    {
        // Now logged in!
        ui->lineEdit_username->setText("");
        ui->lineEdit_password->setText("");
        // Set username and password
        user_info->setUsername(username);
        user_info->setPassword(password);
        // Update settings page
        ui->settings_email->setText(email);
        ui->settings_username->setText(user_info->getUsername());

        ui->stackedWidget_window->setCurrentWidget(ui->main_page); // Change main page
        ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
    }*/
}

void StartPage::on_singup_button_clicked()
{
    /*user_info->printReadyState();

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


        if(my_serv->create_account(email, username, password))
        {
            ui->lineEdit_email->setText("");
            ui->lineEdit_username_signup->setText("");
            ui->lineEdit_password1->setText("");
            ui->lineEdit_password2->setText("");
        }

        qDebug() << "Ready To Send";
    }
    else{
        qDebug() << "ready? "<< ready_to_send; // test
        // error message sign up form not complete?
    }*/
}

void StartPage::on_lineEdit_username_signup_editingFinished()
{
    QString username = ui->lineEdit_username_signup->text();
    QString error_msg;
    bool valid = true; //user_info->usernameValidation(username,  error_msg);   // returns if username is valid or not
                                                                        // with error msg if not valid
    //qDebug() << valid;   testing
    if(username.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_username_signup->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    if(valid){                         // sets valid username to UserAccounts username member
        //user_info->setUsername(username);
        //user_info->set_info_complete(1,1);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, error_msg, valid);
}
/*
 * Hides the icons when the user edits the line
 */
void StartPage::on_lineEdit_username_signup_textEdited()
{
    ui->label_username_check->hide();
    //user_info->set_info_complete(1,0);
    on_lineEdit_username_signup_cursorPositionChanged();
}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void StartPage::on_lineEdit_username_signup_cursorPositionChanged()
{
    ui->lineEdit_username_signup->setStyleSheet("color: white; background-color: #545454; border-style: none;");
}
/*
 * Sign Up Check - Email
 * Checks the email entered by the user and sets the green check/X icon
 * accordingly. If email is valid it is set to the UserAccount email
 */
void StartPage::on_lineEdit_email_editingFinished()
{

    QString email = ui->lineEdit_email->text();
    QString error_msg;
    bool valid = true; //user_info->emailValidation(email, error_msg);      // returns if email is valid or not
                                                                    // with error msg if not valid
    if(email.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_email->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    else if(valid){                      // sets valid email to UserAccounts email member
        //user_info->setEmail(email);
        //user_info->set_info_complete(0,1);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_email_check, ui->lineEdit_email, error_msg, valid);
}
/*
 * Hides the icons when the user edits the line
 */
void StartPage::on_lineEdit_email_textEdited()
{
    //user_info->set_info_complete(0,0);
    ui->label_email_check->hide();               // hides the red X when user begins editing text
    on_lineEdit_email_cursorPositionChanged();   // resets color when editing text

}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void StartPage::on_lineEdit_email_cursorPositionChanged()
{
    ui->lineEdit_email->setStyleSheet("color: white; background-color: #545454; border-style: none;");
}
/*
 * Sign Up Check - Password
 * Checks the password entered by the user and sets the green check/X icon
 * accordingly. If password is valid it is set to the UserAccount password
 */
void StartPage::on_lineEdit_password1_editingFinished()
{
    QString password = ui->lineEdit_password1->text();
    QString error_msg;
    bool valid = true; //user_info->passwordValidtion(password,  error_msg);       // returns if password is valid or not
                                                                            // with error msg if not valid
    if(password.isEmpty()){            // resets the stylesheet of the lineEdit when it is clear
        ui->lineEdit_password1->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    else if(valid){                        // sets valid password to UserAccounts password member
        //user_info->setPassword(password);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, error_msg, valid);

}
/*
 * Hides the icons when the user edits the line
 */
void StartPage::on_lineEdit_password1_textEdited()
{
    //user_info->set_info_complete(2,0);
    ui->label_password1_check->hide();               // hides the red X when user begins editing text
    on_lineEdit_password1_cursorPositionChanged();   // resets color when editing text
}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void StartPage::on_lineEdit_password1_cursorPositionChanged()
{
    ui->lineEdit_password1->setStyleSheet("color: white; background-color: #545454; border-style: none;");
}
/*
 * Sign Up Check - Password (Second Input)
 * Checks the password entered by the user and sets the green check/X icon
 * accordingly. If password is valid it is set to the UserAccount password
 */
void StartPage::on_lineEdit_password2_editingFinished()
{
    /*QString password = ui->lineEdit_password2->text();
    if(password.isEmpty()){
        ui->lineEdit_password2->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    else if(password == user_info->getPassword()){
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, "", 1);
        user_info->set_info_complete(2, 1);    // since it matches first entered password
                                               // info_complete[2] is set to true (ready to send)
    }
    else{
        set_valid_icons(ui->label_password2_check, ui->lineEdit_password2, "", 0);
        user_info->set_info_complete(2,0);
    }*/
}
void StartPage::on_lineEdit_password2_textEdited()
{

    ui->label_password2_check->hide();               // hides the red X when user begins editing text
    //user_info->set_info_complete(2,0);
    on_lineEdit_password2_cursorPositionChanged();   // resets color when editing text
}
void StartPage::on_lineEdit_password2_cursorPositionChanged()
{
    ui->lineEdit_password2->setStyleSheet("color: white; background-color: #545454; border-style: none;");
}
/*
 * Sets green check/X accordingly with given valid input. Outputs error message if needed
 */
void StartPage::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid){
    if(valid){
        QPixmap check_mark(":/resources/img/check_mark.png");
        this_label->setPixmap(check_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this_line->setStyleSheet("color: white; background-color: #545454; border-style: none;");
        this_label->show();
     }
     else{
        this_line->setStyleSheet("color: white; background-color: rgb(230, 80, 80); border-style: none;");
        QPixmap x_mark(":/resources/img/x_mark.png");
        this_label->setPixmap(x_mark.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this_label->show();
     }
}

void StartPage::on_pushButton_recover_pass_clicked()
{
    /*QString username = ui->lineEdit_recover_pass_1->text();
    QString email = ui->lineEdit_recover_pass_2->text();

    QString pass;

    if (my_serv->recover_pass(username, email, pass)){
        QMessageBox password_box;
        password_box.setText ("Email Sent!");
        password_box.exec();
    }
    else {
        QMessageBox error_box;
        error_box.critical (0, "Error", "An error has occured! ");
        error_box.setFixedSize(500,200);
    }*/

}

void StartPage::on_pushButton_recover_user_clicked()
{
    QString email = ui->lineEdit_recover_user->text();
    QString user;

    /*if (my_serv->recover_user(email, user)){
        // QString username = user;
        QMessageBox username_box;
        username_box.setText("Your username is: ");
        username_box.setInformativeText(user); //placeholder
        username_box.exec();
    }
    else {
        QMessageBox error_box;
        error_box.critical (0, "Error", "An error has occured! ");
        error_box.setFixedSize(500,200);
    }*/

}
