#include "settingspage.h"
#include "ui_settingspage.h"

SettingsPage::SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPage)
{
    ui->setupUi(this);

    QPixmap exit = QPixmap(":/resources/img/exit.png").scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->exit_button->setIconSize(QSize(35, 35));
    ui->exit_button->setIcon(QIcon(exit));
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::on_exit_button_released()
{
    emit exit_settings();
}
