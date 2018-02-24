#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = 0);
    ~SettingsPage();

signals:
    void exit_settings();

private slots:
    void on_exit_button_released();

private:
    Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
