#include "joingrouppage.h"
#include "ui_joingrouppage.h"
#include "server.h"
#include "groupwidget.h"

JoinGroupPage::JoinGroupPage(QWidget *parent) :
    GroupMainPage(parent),
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
    QString group_id = ui->join_group_lineEdit->text();
    if(server::request_response(group_id, group_id))
    {
        GroupWidget* group_widget = new GroupWidget();
        group_widget->set_groupID(group_id);
        ui->join_group_lineEdit->setText("");
        emit group_joined(group_widget);

        /*ui->back_to_group_button->setVisible(true);
        ui->back_to_group_button->setText(group_id);
        ui->leave_button->setVisible(true);
        ui->leave_button->setChecked(false);*/
    }
}
