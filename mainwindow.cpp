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
    ui->stackedWidget_window->setCurrentIndex(0);

    server::initialize();
    left_buttons_list.push_back(ui->home_button);
    left_buttons_list.push_back(ui->create_button);
    left_buttons_list.push_back(ui->join_button);

    connect(ui->login_page, SIGNAL(logged_in(unsigned)), this, SLOT(changePage(unsigned)));

    connect(ui->join_page, SIGNAL(group_joined(QWidget*,QString)), this, SLOT(newPage(QWidget*,QString)));
    connect(ui->create_page, SIGNAL(group_joined(QWidget*,QString)), this, SLOT(newPage(QWidget*,QString)));

    connect(ui->home_button, &QPushButton::released, [=] { changePage(0); });
    connect(ui->create_button, &QPushButton::released, [=] { changePage(1); });
    connect(ui->join_button, &QPushButton::released, [=] { changePage(2); });

    connect(ui->settings, &SettingsPage::exit_settings, [=] { ui->stackedWidget_window->setCurrentWidget(ui->page_wrapper); });
    QPixmap gear = QPixmap(":/resources/img/gear.png").scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->settings_button->setIconSize(QSize(35, 35));
    ui->settings_button->setIcon(QIcon(gear));


    //connect(ui->header, SIGNAL(button_changed(unsigned)), this, SLOT(changePage(unsigned)));
    //connect(ui->header, SIGNAL(leave_group()), this, SLOT(removeCurrentPage()));

    //connect(my_serv, SIGNAL(disconnected()), this, SLOT(on_logout_button_released())); // Logs out user if server connection is lost
    user_info = new UserAccount();

    // Make the login page always the first one
    ui->stackedWidget_window->setCurrentWidget(ui->login_page);

    // Read the config file
    /******
     *
     * WILL REDO ALL OF THIS IN A LATER SPRINT... SO SETTINGS WILL NOT WORK UNTIL THEN....
     * Note: Use QSettings class....
     *
     *
    /**QFile config(".sg"); // Initialize config file
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
            QByteArray user_data;
            user_data = config.read((qint64)user_data_size);
            if(user_data.length() > 0)
            {
                ui->lineEdit_username->setText(QString(user_data));
            }
            char pass_data_size;
            qint64 pass_read_size = config.read(&pass_data_size, 2);
            qDebug() << (qint64)pass_data_size;
            if(pass_read_size == 2)
            {
                QByteArray pass_data;
                pass_data = config.read((qint64)pass_data_size);
                if(pass_data.length() > 0)
                {
                    ui->lineEdit_password->setText(QString(pass_data));
                    ui->settings_remember_login->setChecked(true);
                } else {
                    ui->lineEdit_username->setText("");
                }
            }
        }
    }**/
}

MainWindow::~MainWindow()
{
    //on_logout_button_released();

    delete ui;
    //group_widget->deleteLater();
}

void MainWindow::setStackedIndex(unsigned index)
{
    ui->stackedWidget_window->setCurrentIndex(index);
}

void MainWindow::changePage(unsigned index)
{
    show_leave_button(index > 2);
    ui->stackedWidget_window->setCurrentWidget(ui->page_wrapper);
    ui->page->setCurrentIndex(index);
    ui->homepage->set_active(index == 0);
    set_active_button(index);
}

void MainWindow::newPage(QWidget* new_widget, QString title)
{
    ui->stackedWidget_window->setCurrentWidget(ui->page_wrapper);
    int new_index = ui->page->addWidget(new_widget);
    show_leave_button(new_index);
    ui->page->setCurrentIndex(new_index);
    append_left_button(title);
    set_active_button(new_index);
}

void MainWindow::removeCurrentPage()
{
    QWidget* page = ui->page->currentWidget();
    ui->page->removeWidget(page);
    show_leave_button(ui->page->currentIndex() > 1);
    page->deleteLater();
}

/*
 * Sign Up Check - Username
 * Checks the username entered by the user and sets the green check/X icon
 * accordingly. If username is valid it is set to the UserAccount email
 */



/**************
 *
 * PRIVATE
 *
 */

void MainWindow::exit_settings()
{
    //ui->stackedWidget_inner->setCurrentWidget(exit_settings_to); // Go back to previously active page
}

void MainWindow::on_settings_button_released()
{
    ui->stackedWidget_window->setCurrentWidget(ui->settings);
}

void MainWindow::on_leave_button_released()
{
    unsigned index = ui->page->currentIndex();
    QString leave_code = "LGRP";
    QString group_id = left_buttons_list.at(index)->text();
    server::send(leave_code + group_id);
    removeCurrentPage();
    remove_button(index);
    set_active_button(ui->page->currentIndex());
}

void MainWindow::_initialize_group()
{
    /*
     * DONT THINK WE NEED ANYMORE...... JUST KEEPING FOR REFERENCE.
     * group_widget = new GroupWidget();

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

    connect(group_widget, SIGNAL(send_card(QString&, QString&, int&, int&)), my_serv, SLOT(send_card(QString&, QString&, int&, int&)));
    connect(my_serv, SIGNAL(new_flashcard(int,QString,bool)), group_widget, SLOT(incoming_card(int,QString,bool)));
*/}

void MainWindow::_activate_group(QString &group_id)
{
    /*
     * DONT THINK WE NEED ANYMORE
     * ui->stackedWidget_inner->addWidget(group_widget);
    ui->stackedWidget_inner->setCurrentWidget(group_widget);
    group_widget->set_groupID(group_id);

    ui->back_to_group_button->setVisible(true);
    ui->back_to_group_button->setText(group_id);
    ui->leave_button->setVisible(true);
    ui->leave_button->setChecked(false);**/
}

void MainWindow::set_active_button(unsigned index)
{
    for (unsigned i = 0; i < left_buttons_list.size(); i++) {
        left_buttons_list.at(i)->setChecked(i == index);
    }
}

void MainWindow::append_left_button(QString title)
{
    QPushButton* new_button = copy_button(ui->create_button);
    new_button->setText(title);
    left_buttons_list.push_back(new_button);
    ui->left_buttons->addWidget(new_button);
    connect(new_button, &QPushButton::released, [=] { changePage(left_buttons_list.indexOf(new_button)); });
}

QPushButton *MainWindow::copy_button(QPushButton* button)
{
    QPushButton* copied_button = new QPushButton();
    copied_button->setStyleSheet(button->styleSheet());
    copied_button->setFont(button->font());
    copied_button->setMinimumHeight(button->height());
    copied_button->setFocusPolicy(button->focusPolicy());
    copied_button->setCheckable(button->isCheckable());
    return copied_button;
}

void MainWindow::remove_button(unsigned index)
{
    QPushButton* button = left_buttons_list.takeAt(index);
    ui->left_buttons->removeWidget(button);
    button->deleteLater();
}

void MainWindow::show_leave_button(bool show)
{
    ui->leave_button->setVisible(show);
}

void MainWindow::on_logout_button_released()
{
    // Sanity check: if we aren't even logged in yet (if login_page is active), don't do anything!
    if((ui->stackedWidget_window->currentWidget() != ui->login_page))
    {
        // Clear username info -- REDO WHEN DONE
        //user_info->setEmail("");
        //user_info->setUsername("");
        //user_info->setPassword("");
        // Change widget
        ui->stackedWidget_window->setCurrentWidget(ui->login_page);
        server::send("LOGT");
    }
}

/*void MainWindow::on_stackedWidget_inner_currentChanged(int)
{
    /*ui->join_button->setChecked(false);
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
    }*/
//}

/***
 *
 * SETTINGS DONT WORK BRUH
 *
 * void MainWindow::on_settings_timestamps_currentIndexChanged(int index)
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
        }
    }
    else if((!checked) && (ui->stackedWidget_window->currentWidget() == ui->main_page)) {
        if(config.open(QIODevice::WriteOnly | QIODevice::Append)) {
            config.seek(1);
            char nope = -1;
            config.write(&nope, 1);
        }
    }
}*/
