#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QDate>

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
    Whiteboard* whiteboard_ptr();

signals:
    void send_chat(QString& groupID, QString& message);
    void line_drawn(QString groupID, const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);
    void whiteboard_draw_line(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);
    void save_whiteboard(QString&, QByteArray*);

public slots:
    void new_chat(QString username, QString time, QString message);
    void users_changed();
    void user_joined(QString username);
    // Whiteboard
    void send_line_drawn(const QPoint& first_mouse_pos, const QPoint& second_mouse_pos, const QColor& pen_color, const int& pen_size);

    void set_groupID(QString& groupID);

    void on_submit_chat_released();

private slots:
    void on_save_whiteboard_button_released();

    void on_comboBox_pen_color_currentTextChanged(const QString &arg1);

    void on_comboBox_pen_size_currentTextChanged(const QString &arg1);

private:
    Ui::GroupWidget* ui;
    Whiteboard* whiteboard;
    QString group_id;

    QDate last_date_printed;
};

#endif // GROUPWIDGET_H
