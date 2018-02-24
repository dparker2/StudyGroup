#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QDate>

#include "whiteboard_wrapper.h"
#include "sgwidget.h"
#include "deck.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public SGWidget
{
    Q_OBJECT
public:
    explicit GroupWidget(QString id, QWidget *parent = nullptr);
    ~GroupWidget();
    QString get_groupID();
    Whiteboard_Wrapper* whiteboard_ptr();

signals:
    void send_chat(QString& groupID, QString& message);
    void line_drawn(QString groupID, const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);
    void whiteboard_draw_line(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);
    void save_whiteboard(QString&, QByteArray*);
    void send_card(QString&, QString&, int&, int&);

public slots:
    void new_chat(QString username, QString str_date_time, QString message);
    void users_changed();
    void user_joined(QString username);
    void set_groupID(QString& groupID);
    void on_submit_chat_released();


private slots:
    void on_comboBox_study_mode_currentIndexChanged(int index);

private:
    void do_work();
    Ui::GroupWidget* ui;
    Whiteboard_Wrapper* whiteboard;
    Deck* deck;
    QString group_id;

    QDate last_date_printed;
};

#endif // GROUPWIDGET_H
