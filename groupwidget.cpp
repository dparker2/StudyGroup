#include "groupwidget.h"
#include "ui_groupwidget.h"
#include <QTime>
#include <QDebug>
#include <QLine>
#include <QVBoxLayout>
#include <whiteboard_wrapper.h>

GroupWidget::GroupWidget(QString id, QWidget *parent) :
    SGWidget(id, parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
    group_id = id;
    set_groupID(group_id);

    // Begin set whiteboard
    whiteboard = new Whiteboard_Wrapper(group_id + " whiteboard");
    ui->study_mode->addWidget(whiteboard);
    ui->study_mode->setCurrentWidget(whiteboard);
    // End set whiteboard

    // Flaschard
    deck = new Deck(group_id + " flashcard");
    ui->study_mode->insertWidget(1, deck);
}

GroupWidget::~GroupWidget()
{
    deck->deleteLater();
    whiteboard->deleteLater();
}

void GroupWidget::do_work()
{
    while(!_work_queue.isEmpty()) {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message, 5);
        if (message_list[0] == "USCH") {
            users_changed();
        }
        else if(message_list[0] == "NUSR") {
            user_joined(message_list[1]);
        }
        else if(message_list[0] == "NCHT") {
            new_chat(message_list[1], message_list[2] + ' ' + message_list[3], message_list[4]);
        }
    }
}

/********
 *
 * PUBLIC
 *
 */

QString GroupWidget::get_groupID()
{
    return group_id;
}

Whiteboard_Wrapper* GroupWidget::whiteboard_ptr()
{
    return whiteboard;
}

/********
 *
 * SLOTS
 *
 */

void GroupWidget::new_chat(QString username, QString str_date_time, QString message)
{
    qDebug() << username;
    qDebug() << str_date_time;
    qDebug() << message;
    QDateTime date_time = QDateTime::fromString(str_date_time, "yyyy-MM-dd HH:mm:ss");
    date_time.setTimeSpec(Qt::UTC);
    //QDateTime updated_date_time(date_time.toTimeSpec(timestamps.timeSpec()));
    QString updated_time = date_time.toString("yyyy-MM-dd hh:mm:ss AP");

    qDebug() << message;
    QDate new_date = QDate::fromString(updated_time.section(' ', 0, 0), "yyyy-MM-dd");
    qDebug() << updated_time.section(' ', 0, 0);
    updated_time.remove(0, 11);
    qDebug() << new_date.toString("dddd MMM dd yyyy");
    qDebug() << last_date_printed.toString("dddd MMM dd yyyy");
    if(new_date != last_date_printed)
    {
        ui->chat_box->append(new_date.toString("dddd MMM dd yyyy"));
        last_date_printed = new_date;
    }
    ui->chat_box->append(updated_time+" - "+username+": "+message);
}

void GroupWidget::users_changed()
{
    QLayoutItem* item;
    while ( ( item = ui->username_layout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}

void GroupWidget::user_joined(QString username)
{
    QLabel* username_label = new QLabel(username);
    username_label->setStyleSheet("color: white");
    username_label->setFont(QFont("Trebuchet MS", 20));
    ui->username_layout->addWidget(username_label);
}

void GroupWidget::set_groupID(QString &groupID)
{
    group_id = groupID;
    ui->groupid_label->setText("GroupID: "+groupID);
}

/********
 *
 * UI SLOTS
 *
 */

void GroupWidget::on_submit_chat_released()
{
    QString chat_message = ui->chat_input->text();
    if(chat_message != "") {
        ui->chat_input->setText("");
        server::send(server::CHAT_SEND + group_id + ' ' + chat_message);
    }
}

void GroupWidget::on_comboBox_study_mode_currentIndexChanged(int index)
{
    if(index){
        ui->study_mode->setCurrentWidget(deck);
        ui->study_menu->setCurrentIndex(1);
    }
    else{
        ui->study_mode->setCurrentWidget(whiteboard);
        ui->study_menu->setCurrentIndex(0);
    }
}
