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
    explicit Flashcard(QString groupID, QString, QString, int, bool, QWidget *parent = nullptr);
    ~Flashcard();

    void setFront(QString, int, bool);
    void setBack(QString, int, bool);
    void set_card_num(int);
    void send_index(int);

public slots:

private slots:
    void on_edit_card_btn_clicked();
    void on_flip_card_btn_clicked();


private:
    Ui::Flashcard *ui;
    FCFront* front;
    FCBack* back;
    QString groupID;
    int cardNum;
    int current_side;
};

#endif // FLASHCARD_H
