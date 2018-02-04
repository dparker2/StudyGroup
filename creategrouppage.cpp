#include "creategrouppage.h"
#include "ui_creategrouppage.h"
#include "server.h"
#include "groupwidget.h"

CreateGroupPage::CreateGroupPage(QWidget *parent) :
    GroupMainPage(parent),
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
    QString group_id = ui->create_group_lineEdit->text();
    QString full_string = server::GROUP_CREATE + group_id;
    if(server::request_response(full_string, group_id))  // Group id replaced with the response (group name + code)
    {
        GroupWidget* group_widget = new GroupWidget();
        group_widget->set_groupID(group_id);
        ui->create_group_lineEdit->setText("");
        emit group_joined(group_widget, group_id);

        /*ui->back_to_group_button->setVisible(true);
        ui->back_to_group_button->setText(group_id);
        ui->leave_button->setVisible(true);
        ui->leave_button->setChecked(false);*/
    }
}
