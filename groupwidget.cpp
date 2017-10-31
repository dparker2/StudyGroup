#include "groupwidget.h"
#include "ui_groupwidget.h"
#include <QTime>
#include <QDebug>

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    whiteboard = new Whiteboard();

    ui->study_mode->addWidget(whiteboard);
    ui->study_mode->setCurrentWidget(whiteboard);
    ui->study_mode->setStyleSheet("background-color: #ffffff;");
}

QString GroupWidget::get_groupID()
{
    return group_id;
}

void GroupWidget::new_chat(QString username, QString time, QString message)
{
    qDebug() << message;
    ui->chat_box->append(time+" - "+username+": "+message);
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
    username_label->setStyleSheet("color: white; font-size: 20px;");
    ui->username_layout->addWidget(username_label);
}

void GroupWidget::set_groupID(QString &groupID)
{
    group_id = groupID;
    ui->groupid_label->setText("GroupID: "+groupID);
}

void GroupWidget::on_submit_chat_released()
{
    QString chat_message = ui->chat_input->text();
    ui->chat_input->setText("");
    QString groupID = group_id;
    emit send_chat(groupID, chat_message); // Send the chat signal
}
