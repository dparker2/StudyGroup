#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_account_check.h"
#include "server.h"
#include <QPixmap>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    group_widget = nullptr;
    ui->setupUi(this);

    QPixmap logo(":/resources/img/GSLogoName1.png");    // StudyGroup logo
    ui->label_logo->setPixmap(logo.scaled(250,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));     // Resize to fit

    QPixmap gear(":/resources/img/gear.png");
    QIcon settingBtn(gear);
    ui->settings_button->setIcon(settingBtn);
    ui->settings_button->setIconSize(QSize(31,31));

    // check/X icons are hidden initially
    ui->label_username_check->hide();
    ui->label_password1_check->hide();
    ui->label_password2_check->hide();
    ui->label_email_check->hide();

    ui->back_to_group_button->hide();
    ui->leave_button->hide();

    my_serv = new server();
    my_serv->connect_server();
    connect(my_serv, SIGNAL(disconnected()), this, SLOT(on_logout_button_released())); // Logs out user if server connection is lost
    user_info = new UserAccount();

    // Make the login page always the first one
    ui->stackedWidget_window->setCurrentWidget(ui->login_page);
    ui->tabWidget->setCurrentWidget(ui->tab_sign_in);

    // Read the config file
    QFile config(".sg"); // Initialize config file
    if(config.open(QIODevice::ReadWrite)) { // Open config file
        char timezone_data;
        int read_size = config.read(&timezone_data, 1); // Read one byte (timezone)
        qDebug() << read_size << timezone_data;
        if(read_size != 1) {
            qDebug() << "error reading config file";
        }
        if((timezone_data == server::timestamp_local) || (timezone_data == server::timestamp_utc)) {
            ui->settings_timestamps->setCurrentIndex(timezone_data - 1);
            my_serv->setTimestamps(timezone_data);
        } else {
            config.seek(config.pos() - 1); // Move I/O pos 1 backwards
            char default_time = server::timestamp_local;
            config.write(&default_time, 1); // Reset to deault (local) if odd value
            qDebug() << "wrote" << default_time << "to config";
            my_serv->setTimestamps(server::timestamp_local);
        }

        char user_data_size;
        qint64 user_read_size = config.read(&user_data_size, 2);
        qDebug() << (qint64)user_data_size;
        if((user_read_size == 2) && (user_data_size != -1))
        {
            QByteArray user_data(config.read((qint64)user_data_size));
            if(user_data.length() > 0)
            {
                ui->lineEdit_username->setText(QString(user_data));
            }
            char pass_data_size;
            qint64 pass_read_size = config.read(&pass_data_size, 2);
            qDebug() << (qint64)pass_data_size;
            if(pass_read_size == 2)
            {
                QByteArray pass_data(config.read((qint64)pass_data_size));
                if(pass_data.length() > 0)
                {
                    ui->lineEdit_password->setText(QString(pass_data));
                    ui->settings_remember_login->setChecked(true);
                } else {
                    ui->lineEdit_username->setText("");
                }
            }
        }
        config.close();
    }
}

MainWindow::~MainWindow()
{
    on_logout_button_released();

    delete user_info;
    delete my_serv;
    delete ui;
}

void MainWindow::on_signin_button_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString email;

    if(my_serv->login(username, password, email))
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
        ui->lineEdit_username_signup->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    if(valid){                         // sets valid username to UserAccounts username member
        user_info->setUsername(username);
        user_info->set_info_complete(1,1);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_username_check, ui->lineEdit_username_signup, error_msg, valid);
}
/*
 * Hides the icons when the user edits the line
 */
void MainWindow::on_lineEdit_username_signup_textEdited()
{
    ui->label_username_check->hide();
    user_info->set_info_complete(1,0);
    on_lineEdit_username_signup_cursorPositionChanged();
}
/*
 * Resets defaults to line edit style sheet anytime the user changes cursor to edit
 */
void MainWindow::on_lineEdit_username_signup_cursorPositionChanged()
{
    ui->lineEdit_username_signup->setStyleSheet("color: white; background-color: #545454; border-style: none;");
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
        ui->lineEdit_email->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    else if(valid){                      // sets valid email to UserAccounts email member
        user_info->setEmail(email);
        user_info->set_info_complete(0,1);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_email_check, ui->lineEdit_email, error_msg, valid);
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
    ui->lineEdit_email->setStyleSheet("color: white; background-color: #545454; border-style: none;");
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
        ui->lineEdit_password1->setStyleSheet("color: white; background-color: #545454; border-style: none;");
    }
    else if(valid){                        // sets valid password to UserAccounts password member
        user_info->setPassword(password);
    }
    // sets correct icon
    // green: valid, X otherwise
    set_valid_icons(ui->label_password1_check, ui->lineEdit_password1, error_msg, valid);

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
    ui->lineEdit_password1->setStyleSheet("color: white; background-color: #545454; border-style: none;");
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
    ui->lineEdit_password2->setStyleSheet("color: white; background-color: #545454; border-style: none;");
}
/*
 * Sets green check/X accordingly with given valid input. Outputs error message if needed
 */
void MainWindow::set_valid_icons(QLabel* this_label, QLineEdit* this_line, QString error_msg, bool valid){
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

void MainWindow::on_settings_button_released()
{
    QWidget* curr_page = ui->stackedWidget_inner->currentWidget();
     // Save previous page to exit to after
    if(curr_page->objectName() == "stackedPage_Settings"){
        exit_settings();
    }
    else{
        exit_settings_to = ui->stackedWidget_inner->currentWidget();
        ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_Settings); // Change active page to settings
    }
}

void MainWindow::exit_settings()
{
    ui->stackedWidget_inner->setCurrentWidget(exit_settings_to); // Go back to previously active page
}

void MainWindow::set_settings_btn_icon(int icon){
    if(icon){   // if true sets icon to an X
        QPixmap exit(":/resources/img/exit.png");
        QIcon settingBtn(exit);
        ui->settings_button->setIcon(settingBtn);
        ui->settings_button->setIconSize(QSize(25,25));
        ui->settings_button->setChecked(true);
        //ui->settings_button->setStyleSheet("background-color: rgb(163,163,163);");
    }
    else{      // sets button to an gear
        QPixmap gear(":/resources/img/gear.png");
        QIcon settingBtn(gear);
        ui->settings_button->setIcon(settingBtn);
        ui->settings_button->setIconSize(QSize(31,31));
        ui->settings_button->setChecked(false);
        //ui->settings_button->setStyleSheet("background-color: rgb(39,125,176);");
    }
}

void MainWindow::on_pushButton_recover_pass_clicked()
{
    QString username = ui->lineEdit_recover_pass_1->text();
    QString email = ui->lineEdit_recover_pass_2->text();

    QString pass;

    if (my_serv->recover_pass(username, email, pass)){
        QMessageBox password_box;
        password_box.setText ("Your password is: ");
        password_box.setInformativeText(pass); // placeholder
        password_box.exec();
    }
    else {
        QMessageBox error_box;
        error_box.critical (0, "Error", "An error has occured! ");
        error_box.setFixedSize(500,200);
    }

}

void MainWindow::on_pushButton_recover_user_clicked()
{
    QString email = ui->lineEdit_recover_user->text();
    QString user;

    if (my_serv->recover_user(email, user)){
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
    }

}

void MainWindow::on_join_button_released()
{
    ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
    ui->join_button->setChecked(true);
}

void MainWindow::on_create_button_released()
{
    ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_CreateGroup);
    ui->create_button->setChecked(true);
}

void MainWindow::on_create_group_button_released()
{
    QString group_name = ui->create_group_lineEdit->text();
    QString group_id;
    _initialize_group();
    if(my_serv->create_group(group_name, group_id))
    {
        ui->create_group_lineEdit->setText("");
        _activate_group(group_id);
    }
    else {
        group_widget->deleteLater();
    }
}

void MainWindow::on_join_group_button_released()
{
    QString group_id = ui->join_group_lineEdit->text();
    _initialize_group();
    if(my_serv->join_group(group_id))
    {
        ui->join_group_lineEdit->setText("");
        _activate_group(group_id);
    }
    else {
        group_widget->deleteLater();
    }
}

void MainWindow::on_back_to_group_button_released()
{
    if(group_widget != nullptr) // Sanity check, check if even in a group
    {
        ui->stackedWidget_inner->setCurrentWidget(group_widget);
        ui->back_to_group_button->setChecked(true);
    }
}

void MainWindow::on_leave_button_released()
{
    if(group_widget != nullptr) // Check if even in a group
    {
        QString group_id = group_widget->get_groupID();
        if(my_serv->leave_group(group_id))
        {
            ui->stackedWidget_inner->removeWidget(group_widget);
            ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
            ui->back_to_group_button->setVisible(false);
            ui->leave_button->setVisible(false);
            group_widget->whiteboard_ptr()->deleteLater();
            group_widget->deleteLater();
            group_widget = nullptr;
        }
    }
}

/**************
 *
 * PRIVATE
 *
 */

void MainWindow::_initialize_group()
{
    group_widget = new GroupWidget();

    // ALL THE CONNECTIONS!!!
    connect(my_serv, SIGNAL(user_joined(QString)), group_widget, SLOT(user_joined(QString)));
    connect(my_serv, SIGNAL(users_changed()), group_widget, SLOT(users_changed()));
    connect(my_serv, SIGNAL(new_chat(QString,QString,QString)), group_widget, SLOT(new_chat(QString,QString,QString)));

    connect(my_serv, SIGNAL(whiteboard_draw_line(QPoint,QPoint,QColor,int)), group_widget, SIGNAL(whiteboard_draw_line(QPoint,QPoint,QColor,int)));
    connect(my_serv, SIGNAL(get_whiteboard(QString)), group_widget->whiteboard_ptr(), SLOT(get_whiteboard(QString)));
    connect(group_widget->whiteboard_ptr(), SIGNAL(send_whiteboard(QString&,QByteArray*)), my_serv, SLOT(send_whiteboard(QString&,QByteArray*)));
    connect(my_serv, SIGNAL(update_whiteboard(QByteArray*)), group_widget->whiteboard_ptr(), SLOT(update_whiteboard(QByteArray*)));

    connect(group_widget, SIGNAL(send_chat(QString&,QString&)), my_serv, SLOT(send_chat(QString&,QString&)));
    connect(group_widget, SIGNAL(line_drawn(QString,QPoint,QPoint,QColor,int)), my_serv, SLOT(send_whiteboard_line(QString,QPoint,QPoint,QColor,int)));
    connect(group_widget, SIGNAL(save_whiteboard(QString&,QByteArray*)), my_serv, SLOT(save_whiteboard(QString&,QByteArray*)));
}

void MainWindow::_activate_group(QString &group_id)
{
    ui->stackedWidget_inner->addWidget(group_widget);
    ui->stackedWidget_inner->setCurrentWidget(group_widget);
    QString name = user_info->getUsername();
    group_widget->user_joined(name);
    group_widget->set_groupID(group_id);

    ui->back_to_group_button->setVisible(true);
    ui->back_to_group_button->setText(group_id);
    ui->leave_button->setVisible(true);
    ui->leave_button->setChecked(false);
}

void MainWindow::on_logout_button_released()
{
    // Sanity check: if we aren't even logged in yet (if login_page is active), don't do anything!
    if((ui->stackedWidget_window->currentWidget() != ui->login_page) && (my_serv->logout()))
    {
        if(group_widget != nullptr)
        {
            // Leave group if still in it
            QString id = group_widget->get_groupID();
            my_serv->leave_group(id);
            ui->stackedWidget_inner->removeWidget(group_widget);
            ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
            ui->back_to_group_button->setVisible(false);
            ui->leave_button->setVisible(false);
            group_widget->whiteboard_ptr()->deleteLater();
            group_widget->deleteLater();
            group_widget = nullptr;
        }
        // Clear username info
        delete user_info;
        user_info = new UserAccount();
        // Change widget
        ui->stackedWidget_window->setCurrentWidget(ui->login_page);
    }
}

void MainWindow::on_stackedWidget_inner_currentChanged(int)
{
    ui->join_button->setChecked(false);
    ui->create_button->setChecked(false);
    ui->back_to_group_button->setChecked(false);
    set_settings_btn_icon(0);
    if(ui->stackedWidget_inner->currentWidget() == ui->stackedPage_JoinGroup)
    {
        ui->join_button->setChecked(true);
    }
    else if(ui->stackedWidget_inner->currentWidget() == ui->stackedPage_CreateGroup)
    {
        ui->create_button->setChecked(true);
    }
    else if(ui->stackedWidget_inner->currentWidget() == ui->stackedPage_Settings)
    {
        set_settings_btn_icon(1);
        if(exit_settings_to == ui->stackedPage_JoinGroup)
            ui->join_button->setChecked(true);
        if(exit_settings_to == ui->stackedPage_CreateGroup)
            ui->create_button->setChecked(true);
        if(exit_settings_to == group_widget)
            ui->back_to_group_button->setChecked(true);
    }
    else if(ui->stackedWidget_inner->currentWidget() == group_widget)
    {
        ui->back_to_group_button->setChecked(true);
    }
}

void MainWindow::on_settings_timestamps_currentIndexChanged(int index)
{
    QFile config(".sg"); // Initialize config file
    char data;
    if(index == 0)
        data = server::timestamp_local;
    else if(index == 1)
        data = server::timestamp_utc;
    if(config.open(QIODevice::WriteOnly | QIODevice::Append)) {
        config.seek(0);
        config.write(&data, 1);
        config.close();
    }
}

void MainWindow::on_settings_remember_login_toggled(bool checked)
{
    QFile config(".sg"); // Initialize config file
    if((checked) && (ui->stackedWidget_window->currentWidget() == ui->main_page)) {
        qint64 user_data_length = user_info->getUsername().length();
        const char* user_data = user_info->getUsername().toStdString().c_str();
        qint64 pass_data_length = user_info->getPassword().length();
        const char* pass_data = user_info->getPassword().toStdString().c_str();
        if(config.open(QIODevice::WriteOnly | QIODevice::Append)) {
            config.seek(1);
            qDebug() << "WRITING";
            char ch_udl = user_data_length;
            char ch_psl = pass_data_length;
            config.write(&ch_udl, 2);
            config.write(user_data, user_data_length);
            config.write(&ch_psl, 2);
            config.write(pass_data, pass_data_length);
            qDebug() << "DONE WRITING";
            config.close();
        }
    }
    else if((!checked) && (ui->stackedWidget_window->currentWidget() == ui->main_page)) {
        if(config.open(QIODevice::WriteOnly | QIODevice::Append)) {
            config.seek(1);
            char nope = -1;
            config.write(&nope, 1);
        }
    }
}
