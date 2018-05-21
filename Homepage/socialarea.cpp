#include "socialarea.h"
#include "ui_socialarea.h"

#include "server.h"
#include "userlistitem.h"

SocialArea::SocialArea(QWidget *parent, QString name) :
    SGWidget(name, parent),
    ui(new Ui::SocialArea)
{
    ui->setupUi(this);
}

SocialArea::~SocialArea()
{
    delete ui;
}

void SocialArea::do_work()
{
    while(!_work_queue.isEmpty()) {
        QByteArray message = _work_queue.dequeue();
        QString code = message.left(4);
        message.remove(0, 4);
        if (code == "RSLT") {
            clear_list(ui->search_results->layout());
            QList<QByteArray> search_results = message.split(' ');
            put_into_list(ui->search_results->layout(), search_results);
        }
    }
}

void SocialArea::on_username_search_textEdited(const QString &arg1)
{
    clear_list(ui->search_results->layout());
    if (arg1.size() >= 3)
    {
        qDebug() << "searching for " << arg1;
        server::send(server::SOCIAL_SEARCH_USER + arg1);
    }
}

void SocialArea::clear_list(QLayout *layout)
{
    QLayoutItem* item;
    while ( ( item = layout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void SocialArea::put_into_list(QLayout *layout, QList<QByteArray> items)
{
    for (int i = 0; i < items.size(); i++)
    {
        UserListItem* item = new UserListItem();
        item->set_name(items.at(i));
        item->show_add_button();
        layout->addWidget(item);
    }
}
