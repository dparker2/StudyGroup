#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class Header;
}

class Header : public QWidget
{
    Q_OBJECT

public:
    explicit Header(QWidget *parent = 0);
    ~Header();

public slots:
    void set_active_button(int index);

private slots:
    void on_join_button_released();
    void on_create_button_released();
    void on_settings_button_released();
    void on_back_to_group_button_released();
    void on_leave_button_released();
    void set_settings_btn_icon(int);

private:
    Ui::Header *ui;
    std::vector<QPushButton*> left_to_right_buttons;
};

#endif // HEADER_H
