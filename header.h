#ifndef HEADER_H
#define HEADER_H

#include <QWidget>

namespace Ui {
class Header;
}

class Header : public QWidget
{
    Q_OBJECT

public:
    explicit Header(QWidget *parent = 0);
    ~Header();

private slots:
    void on_join_button_released();

    void on_create_button_released();

    void on_settings_button_released();

    void on_back_to_group_button_released();

    void on_leave_button_released();

    void set_settings_btn_icon(int);

private:
    Ui::Header *ui;
};

#endif // HEADER_H
