#include "homepage.h"
#include "ui_homepage.h"

#include "grouplistitem.h"
#include "server.h"

Homepage::Homepage(QWidget *parent) :
    GroupMainPage("homepage", parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    is_active == false;
}

Homepage::~Homepage()
{
    delete ui;
}

Homepage::set_active(bool is)
{
    is_active = is;
}

void Homepage::do_work()
{
    while(!_work_queue.isEmpty()) {
        QByteArray message = _work_queue.dequeue();
        QByteArray code = message.left(4);
        message.remove(0, 4);
        QList<QByteArray> message_list = split(message);
        if (code == "UPRG") {
            clear_recents();
            update_recents(message_list);
        }
        else if(code == "UPFG") {
            clear_favorites();
            update_favorites(message_list);
        }
    }
}

void Homepage::clear_recents()
{
    QLayoutItem* item;
    while ( ( item = ui->recent_groups->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void Homepage::clear_favorites()
{
    QLayoutItem* item;
    while ( ( item = ui->favorite_groups->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void Homepage::update_recents(QList<QByteArray> &groups_list)
{
    // Iterate through groups list and put shit where it goes
    if(groups_list.at(0) != "") {
        for(int i = 0; i < (groups_list.size() - 1); i += 2)
        {
            GroupListItem* new_item = new GroupListItem();
            connect(new_item, SIGNAL(join_pressed(QString)), this, SLOT(on_join_group(QString)));
            new_item->set_group(groups_list.at(i));
            new_item->set_favorite(groups_list.at(i+1) == "1");
            ui->recent_groups->addWidget(new_item);
        }
    }
}

void Homepage::update_favorites(QList<QByteArray> &groups_list)
{
    if(groups_list.at(0) != "") {
        for(int i = 0; i < groups_list.size(); i += 1)
        {
            GroupListItem* new_item = new GroupListItem();
            connect(new_item, SIGNAL(join_pressed(QString)), this, SLOT(on_join_group(QString)));
            new_item->set_group(groups_list.at(i));
            new_item->set_favorite(true);
            ui->favorite_groups->addWidget(new_item);
        }
    }
}

void Homepage::on_join_group(QString group)
{
    join_group(group);
}
