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
    explicit Deck(QString name, QWidget *parent = 0);
    ~Deck();

    void deleteCard(int index);
    void add_card();
    void init_card(int index, QString text, bool front_side);
    //void set_new_card();
    //void set_existing_card()

    void set_quiz(bool);
    int getDeckSize();
    int get_card_number();
    Flashcard* getCard(int index);
    QString get_card_text();

signals:
    void set_card(QString, int&, int);
    void display_card(Flashcard*);

public slots:

    void check_set_card(Flashcard*, QString&, int&, int);

private slots:

    void on_prev_btn_clicked();
    void on_next_btn_clicked();
    void do_work();

    void on_add_cart_btn_clicked();

private:
    QString groupID;
    bool edit_card(int index, QString text, bool front_side);
    Ui::Deck *ui;
    QVBoxLayout *layout;
    QList<Flashcard*> deck;
    Flashcard* flashcard;
    int current_index;
    bool quiz;
};

#endif // DECK_H
