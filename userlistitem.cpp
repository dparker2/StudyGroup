#include "userlistitem.h"
#include "ui_userlistitem.h"

UserListItem::UserListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListItem)
{
    ui->setupUi(this);
}

UserListItem::~UserListItem()
{
    delete ui;
}

void UserListItem::show_add_button()
{
    ui->button->show();
    ui->button->setCurrentWidget(ui->add_button);
}

void UserListItem::show_remove_button()
{
    ui->button->show();
    ui->button->setCurrentWidget(ui->remove_button);
}

void UserListItem::hide_button()
{
    ui->button->hide();
}

void UserListItem::show_online_status()
{
    ui->online_status->show();
}

void UserListItem::hide_online_status()
{
    ui->online_status->hide();
}

void UserListItem::set_online_status(int status)
{
    if(status == 0) {
        ui->online_status->setCurrentWidget(ui->offline);
    }
    else if(status == 1) {
        ui->online_status->setCurrentWidget(ui->online);
    }
}

void UserListItem::set_name(QString name)
{
    ui->name->setText(name);
}
