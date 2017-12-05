#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "flashcard.h"
#include "ui_cardwidget.h"
//#include "server.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);
    void insertCard(int index, QString front_text);
    void editCard(int index, QString back_text = nullptr, QString front_text = nullptr);
    void deleteCard(int index);
    void on_addCardBtn_clicked();
    void quiz();
    int getDeckSize();
    int get_card_number();
    Flashcard* getCard(int index);
    QString get_card_text();




signals:
    void set_card(QString&, int&, int&);
    void display_card(Flashcard*);

public slots:

    void check_set_card(Flashcard*, QString&, int&, int);

private slots:

    void on_prev_btn_clicked();

    void on_next_btn_clicked();

private:
    Ui::CardWidget *ui;
    QVBoxLayout *layout;
    QList<Flashcard*> deck;
    Flashcard* flashcard;
    int current_index;
};

#endif // CARDWIDGET_H
