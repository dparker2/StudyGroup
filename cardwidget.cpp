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

    current_index = -1;
    quiz = false;
}
void CardWidget::editCard(int index, QString back_text, QString front_text, Flashcard* new_card)
{
    // Is this a new card?
    if((index < deck.size()) && (deck.at(index) != nullptr)) { // Editing existing card
        if(front_text != nullptr)
        {
            deck.at(index)->setFront(front_text);
        }
        if(back_text != nullptr)
        {
            deck.at(index)->setBack(back_text);
        }
    }
    else { // New card!
        while((deck.size() - 1) < index) { // If the index we get is out of range, put nullptrs in until we get to where we want
            deck.append(nullptr);
        }
        if(new_card == nullptr) {
            new_card = new Flashcard(front_text, back_text, index);
            connect(new_card, SIGNAL(check_set_card(Flashcard*,QString&,int&,int)), this, SLOT(check_set_card(Flashcard*,QString&,int&,int)));
        }
        deck[index] = new_card; // The above loop guarantees we are in the proper spot to append at the right index
                                // even if the index was too high at first
        if(deck.size() > 1)
        {
            ui->prev_btn->show(); // Show the prev/next buttons
            ui->next_btn->show();
        } else {
            current_index = index; // If first card AND back is defined, set that in the stacked widget
            if(back_text != nullptr) {
                ui->stackedWidget_card_edit->addWidget(new_card);
                ui->stackedWidget_card_edit->setCurrentWidget(new_card);
            }
        }
    }
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
    Flashcard* new_card = new Flashcard(nullptr, nullptr, -1);
    connect(new_card, SIGNAL(check_set_card(Flashcard*,QString&,int&,int)), this, SLOT(check_set_card(Flashcard*,QString&,int&,int)));
    new_card->emit_init_signal();

    ui->stackedWidget_card_edit->addWidget(new_card);
    ui->stackedWidget_card_edit->setCurrentWidget(new_card); // Put card on front to edit
}

int CardWidget::getDeckSize()
{
    return deck.size();
}

void CardWidget::check_set_card(Flashcard* card, QString& text, int& index, int side)
{
    qDebug() << "CHECK_SET_CARD" << endl;
    if(side == 0) { // Front
        emit set_card(text, index, side); // The index returned is index
        card->setCardNum(index);
        editCard(index, nullptr, text, card); // Edit/Make new card with proper index
    }
    else { // Back
        emit set_card(text, index, side);
        card->setCardNum(index);
        editCard(index, text, nullptr, card); // Edit/Make new card
        if(ui->stackedWidget_card_edit->currentIndex() > 0) {
            ui->stackedWidget_card_edit->removeWidget(card); // Remove the card (done editing)
        }
    }
}

void CardWidget::on_prev_btn_clicked()
{
    ui->stackedWidget_card_edit->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    do {
        if(quiz) {
            current_index = rand() % deck.size();
        } else {
            current_index = (current_index - 1) < 0 ? deck.size() - 1 : current_index - 1; // Update index
            // If current index - 1 is negative, loop back to top. Otherwise, current_index - 1.
        }
    } while(deck.at(current_index) == nullptr); // Keep updating until we get to one that isnt a nullptr

    ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Add the widget at the new index
    ui->stackedWidget_card_edit->setCurrentWidget(deck.at(current_index)); // Make sure its the one being displayed

}

void CardWidget::on_next_btn_clicked()
{
    ui->stackedWidget_card_edit->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    do {
        if(quiz) {
            current_index = rand() % deck.size();
        } else {
            current_index = (current_index + 1) % deck.size(); // Update index, mod so it loops back to beginning if too far
        }
    } while(deck.at(current_index) == nullptr); // Keep updating until we get to one that isnt a nullptr

    ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Add the widget at the new index
    ui->stackedWidget_card_edit->setCurrentWidget(deck.at(current_index)); // Make sure its the one being displayed
}

void CardWidget::setQuiz(bool is_set)
{
    quiz = is_set;
    if(deck.size() > 0) { // Make sure that the deck even has any cards...
        ui->stackedWidget_card_edit->removeWidget(deck.at(current_index));  // remove current displayed widget
        if(quiz) {
            current_index = rand() % deck.size();
            ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Put random
        }
        else {
            current_index = 0;
            ui->stackedWidget_card_edit->addWidget(deck.at(current_index)); // Put first card
        }
    }
}

