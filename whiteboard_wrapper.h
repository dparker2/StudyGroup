#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QWidget>
#include <QScrollArea>

#include "whiteboard.h"

namespace Ui {
class Whiteboard;
}

class Whiteboard_Wrapper : public QWidget
{
    Q_OBJECT
public:
    explicit Whiteboard_Wrapper(QString name, QWidget *parent = nullptr);
    ~Whiteboard_Wrapper();

signals:
    void send_whiteboard(QString&, QByteArray*);

public slots:

protected:

private slots:
    void on_comboBox_pen_color_currentTextChanged(const QString &pen_color);
    void on_save_whiteboard_button_released();
    void on_comboBox_pen_size_currentTextChanged(const QString &pen_size);
    void whiteboard_changed();

private:
    Ui::Whiteboard *ui;
    Whiteboard* whiteboard;
    QWidget* save_button;
};

#endif // WHITEBOARD_H
