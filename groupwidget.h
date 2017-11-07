#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>

#include "whiteboard.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupWidget(QWidget *parent = nullptr);
    QString get_groupID();

signals:
    void send_chat(QString& groupID, QString& message);
    void line_drawn(QString& groupID, const QPoint& first_mouse_pos, const QPoint& second_mouse_pos);
    void whiteboard_draw_line(QPoint& first_mouse_pos, QPoint& second_mouse_pos);

public slots:
    void new_chat(QString username, QString time, QString message);
    void users_changed();
    void user_joined(QString username);

    void send_line_drawn(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos) {
        emit line_drawn(group_id, first_mouse_pos, second_mouse_pos);
    }

    void set_groupID(QString& groupID);

    void on_submit_chat_released();

private:
    Ui::GroupWidget* ui;
    Whiteboard* whiteboard;
    QString group_id;
};

#endif // GROUPWIDGET_H
