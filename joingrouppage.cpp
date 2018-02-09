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
    QString full_string = server::GROUP_JOIN + group_id;
    if(server::request_response(full_string, group_id))
    {
        GroupWidget* group_widget = new GroupWidget(group_id);
        ui->join_group_lineEdit->setText("");
        emit group_joined(group_widget, group_id);

        /*ui->back_to_group_button->setVisible(true);
        ui->back_to_group_button->setText(group_id);
        ui->leave_button->setVisible(true);
        ui->leave_button->setChecked(false);*/
    }
}
