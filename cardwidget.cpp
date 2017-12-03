#include "cardwidget.h"
#include <QInputDialog>
#include <QHBoxLayout>
#include <QDebug>
#include <QDir>

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    //my_serv = new server();
    //my_serv->connect_server();
    flashcard = new Flashcard();
    layout = new QHBoxLayout(ui->card_container);
    card_num = 0;
}
void CardWidget::insertCard(QString question, QString answer, int cardNum){
    Flashcard* card = new Flashcard(question, answer, cardNum);
    deck.push_back(card);
}
Flashcard* CardWidget::getCard(int index){
    return flashcard;
}

int CardWidget::get_card_number(){
    return flashcard->getCardNum();
}
QString CardWidget::get_card_text(){
    //return flashcard->get_card_text();
}

void CardWidget::deleteCard(int index){
    Flashcard* temp = deck[index];
    deck[index] =deck[deck.size()-1];
    deck[deck.size()-1] = temp;
    deck.pop_back();
}

void CardWidget::on_addCardBtn_clicked()
{
    QWidget* new_card = new Flashcard();
    layout->addWidget(new_card);

    //flashcard = new Flashcard();
    flashcard->setCardNum(card_num++);
    deck.push_back(flashcard);
}

void CardWidget::on_save_deck_btn_clicked()
{
    qDebug() << "DECK SIZE: " << deck.size() << endl;
}

int CardWidget::getDeckSize()
{
    return deck.size();
}

