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
    ui->next_btn->hide();
    ui->prev_btn->hide();

    card_num = 0;
}
void CardWidget::insertCard(QString front, QString back, int cardNum){
    Flashcard* card = new Flashcard(front, back, cardNum);
    deck.push_back(card);
    connect(card, SIGNAL(check_set_card(QString,int,int)), this, SLOT(check_set_card(QString,int,int)));
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
    Flashcard* new_card = new Flashcard();
    //connect(new_card, SIGNAL(check_set_card(QString&,int&,int&)), this, SLOT(check_set_card(QString&,int&,int&)));

    if(deck.size() > 1){

        ui->next_btn->show();
        ui->prev_btn->show();
    }
    if(!deck.isEmpty()){

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(deck[card_num-1]);
        ui->gridLayout_4->removeWidget(deck[card_num-1]);
        deck[card_num-1]->hide();
        ui->gridLayout_4->addWidget(new_card);

    }
    else{
        ui->gridLayout_4->addWidget(new_card);
    }

    new_card->setCardNum(card_num++);
    deck.push_back(new_card);
}

int CardWidget::getDeckSize()
{
    return deck.size();
}

void CardWidget::check_set_card(QString front_text, int card_num, int side)
{
    qDebug() << "CHECK_SET_CARD" << endl;
    if(side == 0){
        emit set_card(front_text, card_num, side);
    }
    else{
        emit set_card(front_text, card_num, side);
    }
}

void CardWidget::on_prev_btn_clicked()
{
    qDebug() << "previous" << deck.size() << " " << card_num <<  endl;
    card_num--;
    if(card_num > 0 && card_num < deck.size()){

        ui->gridLayout_4->replaceWidget(deck[card_num], deck[--card_num]);
        deck[card_num]->show();
        //ui->gridLayout_4->addWidget(deck[--card_num]);
    }
    // error message

}
