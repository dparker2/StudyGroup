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
    QString _;
    if(server::request_response(full_string, _))
    {
        GroupWidget* group_widget = new GroupWidget(group_id);
        ui->join_group_lineEdit->setText("");
        emit group_joined(group_widget, group_id);
    }
}
