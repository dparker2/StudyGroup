#ifndef DECK_H
#define DECK_H

#include "SGWidget.h"
#include "flashcard.h"

namespace Ui {
class Deck;
}

class Deck : public SGWidget
{
    Q_OBJECT

public:
    explicit Deck(QString group_name, QWidget *parent = 0);
    ~Deck();

private slots:

    void do_work();
    void on_prev_btn_clicked();
    void on_next_btn_clicked();
    void on_add_card_btn_clicked();
    void on_quiz_btn_toggled(bool checked);
    void deleteCard(int index);
    void display_card(int index);
    void init_card(int index, QString front_text, QString back_text, bool front_side, bool send_card);

private:
    QString groupID;
    bool edit_card(int index, QString front_text, QString back_text, bool front_side, bool send_card);
    Ui::Deck *ui;
    QList<Flashcard*> deck;
    Flashcard* new_card;
    int current_index;
    int new_index;
    bool quiz;
};

#endif // DECK_H
