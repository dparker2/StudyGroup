#include "grouplistitem.h"
#include "ui_grouplistitem.h"

#include "server.h"

GroupListItem::GroupListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupListItem)
{
    ui->setupUi(this);
}

GroupListItem::~GroupListItem()
{
    delete ui;
}

void GroupListItem::set_group(QString group)
{
    ui->name->setText(group);
}

void GroupListItem::set_pop(QString num)
{
    ui->users->setText(num);
}

void GroupListItem::set_favorite(bool fav)
{
    ui->fav_button->setChecked(fav);
}

void GroupListItem::on_join_button_released()
{
    emit join_pressed(ui->name->text());
}

void GroupListItem::on_fav_button_clicked(bool checked)
{
    ui->fav_button->setChecked(false);
    QString _;
    if (checked) {
        if(server::request_response(server::HOMEPAGE_NEW_FAVORITE + ui->name->text(), _)) {
            ui->fav_button->setChecked(true);
        }
    } else {
        if(server::request_response(server::HOMEPAGE_REMOVE_FAVORITE + ui->name->text(), _)) {
            ui->fav_button->setChecked(false);
        }
    }
}
