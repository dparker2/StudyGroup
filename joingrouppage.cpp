#include "joingrouppage.h"
#include "ui_joingrouppage.h"

JoinGroupPage::JoinGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoinGroupPage)
{
    ui->setupUi(this);
}

JoinGroupPage::~JoinGroupPage()
{
    delete ui;
}

void JoinGroupPage::on_join_group_button_released()
{
    /*QString group_id = ui->join_group_lineEdit->text();
    _initialize_group();
    if(my_serv->join_group(group_id))
    {
        ui->join_group_lineEdit->setText("");
        _activate_group(group_id);
    }
    else {
        group_widget->deleteLater();
    }*/
}
