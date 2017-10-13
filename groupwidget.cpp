#include "groupwidget.h"
#include <QTime>

GroupWidget::GroupWidget(QWidget *parent) : QWidget(parent)
{
    // First make the required layouts to fill everything into
    main_layout = new QGridLayout();
<<<<<<< HEAD
    username_layout = new QVBoxLayout();
=======
    username_layout = new QVBoxLayout;
>>>>>>> parent of 4ff44b6... Merge branch 'master' of https://github.com/ParkerD559/StudyGroup

    // Tackle username area first.
    username_layout->addItem(new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    // Now put that layout inside the main layout, on the right side spanning all the way down
    main_layout->addLayout(username_layout, 0, 2, -1, 1);

    // Chat area. Make a line edit widget that is not editable and put it into main layout
    chat_box = new QTextEdit();
    chat_box->setReadOnly(true);
    main_layout->addWidget(chat_box, 0, 0, 1, 2); // Top left

    // Chat input area.
    chat_input = new QLineEdit();
    submit_chat = new QPushButton();
    submit_chat->setText("Chat!");
    connect(submit_chat, SIGNAL(released()), this, SLOT(on_submit_chat_released()));
    main_layout->addWidget(chat_input, 1, 0); // Bottom left
    main_layout->addWidget(submit_chat, 1, 1);

    // Finish by setting the main layout to the group widget
    this->setLayout(main_layout);
}

void GroupWidget::new_chat(QString &message)
{

}

void GroupWidget::user_joined(QString &username)
{
    username_layout->addWidget(new QLabel(username));
}

void GroupWidget::user_left(QString &username)
{

}

void GroupWidget::on_submit_chat_released()
{
    QString chat_message = chat_input->text();
    QString formatted_message = QTime::currentTime().toString("hh:mm AP") + " - " + "usernamehere:" + chat_message + "\n";
    chat_box->insertPlainText(formatted_message);
}
