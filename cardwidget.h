#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "flashcard.h"
#include "ui_cardwidget.h"
#include "user_account_check.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);
    void insertCard(QString, QString, int);
    //void setDeckName(QString);
    Flashcard* getCard(int index);

    //Flashcard* getNextCard(int index);
    //Flashcard* getPrevCard(int index);

    //void shuffleDeck();
    void deleteCard(int index);

signals:

public slots:

private slots:
    void on_addCardBtn_clicked();

    void on_save_deck_btn_clicked();

private:
    Ui::CardWidget *ui;
    QHBoxLayout *layout;
    server* my_serv;

    QList<Flashcard*> deck;
    Flashcard* flashcard;
    int card_num;
    //QString deck_name;
};

#endif // CARDWIDGET_H
