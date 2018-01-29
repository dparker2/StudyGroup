#include "creategrouppage.h"
#include "ui_creategrouppage.h"

CreateGroupPage::CreateGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroupPage)
{
    ui->setupUi(this);
}

CreateGroupPage::~CreateGroupPage()
{
    delete ui;
}

void CreateGroupPage::on_create_group_button_released()
{
    /*QString group_name = ui->create_group_lineEdit->text();
    QString group_id;
    _initialize_group();
    if(my_serv->create_group(group_name, group_id))
    {
        ui->create_group_lineEdit->setText("");
        _activate_group(group_id);
        QString name = user_info->getUsername();
        group_widget->user_joined(name);
    }
    else {
        group_widget->deleteLater();
    }*/
}
