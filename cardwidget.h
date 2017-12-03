#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "flashcard.h"
#include "ui_cardwidget.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);
    void insertCard(QString, QString, int);
    Flashcard* getCard(int index);

    //Flashcard* getNextCard(int index);
    //Flashcard* getPrevCard(int index);

    //void shuffleDeck();
    void deleteCard(int index);
    void on_addCardBtn_clicked();
    int getDeckSize();

signals:

public slots:

private slots:

    void on_save_deck_btn_clicked();



private:
    Ui::CardWidget *ui;
    QHBoxLayout *layout;
    QList<Flashcard*> deck;
    Flashcard* flashcard;
    int card_num;
};

#endif // CARDWIDGET_H
