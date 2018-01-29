#include "header.h"
#include "ui_header.h"

Header::Header(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Header)
{
    ui->setupUi(this);

    QPixmap gear(":/resources/img/gear.png");
    QIcon settingBtn(gear);
    ui->settings_button->setIcon(settingBtn);
    ui->settings_button->setIconSize(QSize(31,31));

    ui->back_to_group_button->hide();
    ui->leave_button->hide();
}

Header::~Header()
{
    delete ui;
}

void Header::on_settings_button_released()
{
    /*QWidget* curr_page = ui->stackedWidget_inner->currentWidget();
     // Save previous page to exit to after
    if(curr_page->objectName() == "stackedPage_Settings"){
        exit_settings();
    }
    else{
        exit_settings_to = ui->stackedWidget_inner->currentWidget();
        ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_Settings); // Change active page to settings
    }*/
}

void Header::set_settings_btn_icon(int icon){
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

void Header::on_join_button_released()
{
    //ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_JoinGroup);
    ui->join_button->setChecked(true);
}

void Header::on_create_button_released()
{
    //ui->stackedWidget_inner->setCurrentWidget(ui->stackedPage_CreateGroup);
    ui->create_button->setChecked(true);
}

void Header::on_back_to_group_button_released()
{
    /*if(group_widget != nullptr) // Sanity check, check if even in a group
    {
        ui->stackedWidget_inner->setCurrentWidget(group_widget);
        ui->back_to_group_button->setChecked(true);
    }*/
}

void Header::on_leave_button_released()
{
    /*if(group_widget != nullptr) // Check if even in a group
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
    }*/
}
