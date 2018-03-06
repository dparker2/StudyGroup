#include "joingrouppage.h"
#include "ui_joingrouppage.h"
#include "server.h"
#include "groupwidget.h"

JoinGroupPage::JoinGroupPage(QWidget *parent) :
    GroupMainPage("", parent),
    ui(new Ui::JoinGroupPage)
{
    ui->setupUi(this);
}

JoinGroupPage::~JoinGroupPage()
{
    delete ui;
}

void JoinGroupPage::do_work()
{

}

void JoinGroupPage::on_join_group_button_released()
{
    if(join_group(ui->join_group_lineEdit->text()))
    {
        ui->join_group_lineEdit->setText("");
    }
}
