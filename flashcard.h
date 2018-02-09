#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QWidget>
#include <QHBoxLayout>
#include "fcfront.h"
#include "fcback.h"

namespace Ui {
class Flashcard;
}

class Flashcard : public QWidget
{
    Q_OBJECT
public:
    explicit Flashcard(QWidget *parent = nullptr);
    explicit Flashcard(QString, QString, int, QWidget *parent = nullptr);
    ~Flashcard();
    void setFront(QString);
    void setBack(QString);
    void setCardNum(int);
    QString getFront();
    QString getBack();
    int getCardNum();

    void setEditBtn();
    void emit_init_signal();

signals:


    void check_set_card(Flashcard*, QString&, int&, int);

public slots:

private slots:
    void on_edit_card_btn_clicked();

    void on_set_front_btn_clicked();

    void on_cancel_btn_clicked();

    void on_flip_card_btn_clicked();


    void on_set_back_btn_clicked();

private:
    Ui::Flashcard *ui;
    FCFront* front;
    FCBack* back;
    QString back_text;
    QString front_text;
    int cardNum;
    int current_side;
};

#endif // FLASHCARD_H
