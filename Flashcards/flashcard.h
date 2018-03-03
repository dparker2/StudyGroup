#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QWidget>
#include <QHBoxLayout>
#include "fcfront.h"
#include "fcback.h"
#include "fcbackedit.h"
#include "fcfrontedit.h"

namespace Ui {
class Flashcard;
}

class Flashcard : public QWidget
{
    Q_OBJECT
public:
    explicit Flashcard(QWidget *parent = nullptr);
    explicit Flashcard(QString groupID, QString, QString, int, bool, QWidget *parent = nullptr);
    ~Flashcard();

    void setFront(QString, int, bool);
    void setBack(QString, int, bool);
    void set_card_num(int);
    void show_front();

public slots:

private slots:
    void on_edit_card_btn_clicked();
    void on_flip_card_btn_clicked();
    void on_card_widget_currentChanged(int arg1);

    void on_save_button_released();

private:
    Ui::Flashcard *ui;
    FCFrontEdit* fc_front_edit;
    FCBackEdit* fc_back_edit;
    QString groupID;
    int cardNum;
    int back_side;
};

#endif // FLASHCARD_H
