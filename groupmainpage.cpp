#include "groupmainpage.h"

#include "server.h"
#include "groupwidget.h"

GroupMainPage::GroupMainPage(QString name, QWidget* parent) : SGWidget(name)
{

}

bool GroupMainPage::create_group(QString group)
{
    QString full_string = server::GROUP_CREATE + group;
    if(server::request_response(full_string, group))  // Group id replaced with the response (group name + code)
    {
        GroupWidget* group_widget = new GroupWidget(group);
        emit group_joined(group_widget, group);
        return true;
    }
    return false;
}

bool GroupMainPage::join_group(QString group)
{
    QString full_string = server::GROUP_JOIN + group;
    QString retrn;
    if(server::request_response(full_string, retrn))
    {
        GroupWidget* group_widget = new GroupWidget(group);
        emit group_joined(group_widget, group);
        return true;
    }
    return false;
}
