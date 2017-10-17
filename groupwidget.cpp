#include "groupwidget.h"
#include "ui_groupwidget.h"
#include <QTime>

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

void GroupWidget::new_chat(QString message)
{

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
    //username_layout->removeWidget()
    QLabel* username_label = new QLabel(username);
    username_label->setStyleSheet("color: white; font-size: 20px;");
    ui->username_layout->addWidget(username_label);
}

void GroupWidget::user_left(QString username)
{

}

void GroupWidget::on_submit_chat_released()
{
    QString chat_message = ui->chat_input->text();
    QString formatted_message = QTime::currentTime().toString("hh:mm AP") + " - " + "usernamehere:" + chat_message + "\n";
    ui->chat_box->insertPlainText(formatted_message);
}
