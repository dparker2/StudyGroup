#include "creategrouppage.h"
#include "ui_creategrouppage.h"
#include "server.h"
#include "groupwidget.h"

CreateGroupPage::CreateGroupPage(QWidget *parent) :
    GroupMainPage("", parent),
    ui(new Ui::CreateGroupPage)
{
    ui->setupUi(this);
}

CreateGroupPage::~CreateGroupPage()
{
    delete ui;
}

void CreateGroupPage::do_work()
{

}

void CreateGroupPage::on_create_group_button_released()
{
    if(create_group(ui->create_group_lineEdit->text()))  // Group id replaced with the response (group name + code)
    {
        ui->create_group_lineEdit->setText("");
        /*ui->back_to_group_button->setVisible(true);
        ui->back_to_group_button->setText(group_id);
        ui->leave_button->setVisible(true);
        ui->leave_button->setChecked(false);*/
    }
}
