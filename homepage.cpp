#include "homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QWidget *parent) :
    GroupMainPage(parent), SGWidget("homepage"),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::do_work()
{
    while(!_work_queue.isEmpty()) {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message);
        /*if (message_list[0] == "USCH") {
            users_changed();
        }
        else if(message_list[0] == "NUSR") {
            user_joined(message_list[1]);
        }
        else if(message_list[0] == "NCHT") {
            new_chat(message_list[1], message_list[2] + ' ' + message_list[3], message_list[4]);
        }*/
    }
}

void Homepage::clear_recents()
{
    for(unsigned i = 0; i < recent_groups.size(); i++) {
        recent_groups.removeAt(i);
    }
}

void Homepage::clear_favorites()
{
    for(unsigned i = 0; i < favorite_groups.size(); i++) {
        favorite_groups.removeAt(i);
    }
}

void Homepage::insert_recents(QList<QByteArray> &groups_list)
{
    // Iterate through groups list and put shit where it goes
}

void Homepage::insert_favorites(QList<QByteArray> &groups_list)
{

}

void Homepage::update_recents()
{
    ui->
}

void Homepage::update_favorites()
{

}

