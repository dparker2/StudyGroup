#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "flashcard.h"
//#include "server.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = nullptr);
    void deleteCard(int index);
    void setCard(int index, QString text, bool front_side);
    void on_addCardBtn_clicked();
    void setQuiz(bool);
    int getDeckSize();
    int get_card_number();
    Flashcard* getCard(int index);
    QString get_card_text();




signals:
    void send_card(QString, int&, int);
    void display_card(Flashcard*);

public slots:

    void check_send_card(Flashcard*, QString&, int&, int);

private slots:

    void on_prev_btn_clicked();
    void on_next_btn_clicked();

private:

    void editCard(int index, QString text, bool front_side);
    Ui::CardWidget *ui;
    QVBoxLayout *layout;
    QList<Flashcard*> deck;
    Flashcard* flashcard;
    int current_index;
    bool quiz;
};

#endif // CARDWIDGET_H
