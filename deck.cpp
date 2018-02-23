#include "deck.h"
#include "ui_deck.h"

#include <QDebug>
#include "server.h"

Deck::Deck(QString name, QWidget *parent) :
    SGWidget(name, parent),
    ui(new Ui::Deck)
{
    ui->setupUi(this);
    groupID = name.section(" ", 0, 0);
    current_index = 0;
    ui->prev_btn->setEnabled(false);    // Disabled when deck < 1 card
    ui->next_btn->setEnabled(false);
    quiz = false;
}

Deck::~Deck()
{
    delete ui;
}

/*
 * Receive flashcards from server
*/
void Deck::do_work()
{
    while(!_work_queue.isEmpty())
    {
        QByteArray message = _work_queue.dequeue();
        QList<QByteArray> message_list = split(message, 3);
        int index = message_list[1].toInt();
        if (message_list[0] == "FCFT")
        {
            init_card(index, QString(message_list[2]),"", true, false);
        }
        else if(message_list[0] == "FCBK")
        {
            init_card(index, "", QString(message_list[2]),false, false);
        }
    }
}

/*
 *  When a new card needs to be properly inserted/edited
*/
void Deck::init_card(int index, QString front_text, QString back_text, bool front_side, bool send_card)
{
    // Is this a new card?
    if(edit_card(index, front_text, back_text, front_side, send_card))
    {
        return;
    }
    else { // New card!

        new_card = new Flashcard(groupID, front_text, back_text, index, send_card);
        //new_index = index;

        while((deck.size()-1) < index)       // If the index we get is out of range, put nullptrs in until we get to where we want
        {
            deck.append(nullptr);
        }

        deck[index] = new_card;              // The above loop guarantees we are in the proper spot to append at the right index
                                             // even if the index was too high at first
        ui->total->setText("Total: " + QString::number(deck.size()));
        if(deck.size() > 1)
        {
            ui->prev_btn->setEnabled(true);  // Show the prev/next buttons
            ui->next_btn->setEnabled(true);
        } else {
            current_index = index;           // If first card AND back is defined, set that in the stacked widget
            ui->card_area->addWidget(new_card);
            ui->card_area->setCurrentWidget(new_card);
        }
    }
}
/*
 * Adds new card to ui and sends card to server
 * server returns new index and card is displayed
*/
void Deck::on_add_card_btn_clicked()
{
    QString new_card = server::FLASHCARD_SET_FRONT + groupID + " -1";
    server::send(new_card);
}

/*
 *  Returns true if editing existing card, false if new card
*/
bool Deck::edit_card(int index, QString front_text, QString back_text, bool front_side, bool send_card)
{
    // Is this a new card?
    if((index < deck.size()) && (deck.at(index) != nullptr))  // Editing existing card
    {
        if(front_side)
        {
            deck.at(index)->setFront(front_text, index, send_card);
        }
        else {
            deck.at(index)->setBack(back_text, index, send_card);
        }
        ui->card_area->setCurrentWidget(deck.at(index));
        return true;
    }
    return false;
}

/*
 * Flashcard navigation controls
 * Move to previous or next card in the deck
 */
void Deck::on_prev_btn_clicked()
{
    qDebug() << "ON PREV BUTTON CLICKED: " << current_index << " DECK SIZE :" << deck.size();
    ui->card_area->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    do {
        if(quiz) {
            current_index = rand() % deck.size();
        } else {
            current_index = (current_index - 1) < 0 ? deck.size() - 1 : current_index - 1; // Update index
            // If current index - 1 is negative, loop back to top. Otherwise, current_index - 1.
        }
    } while(deck.at(current_index) == nullptr);    // Keep updating until we get to one that isnt a nullptr

    display_card(current_index);                   // Add the widget at the new index and display it
}

void Deck::on_next_btn_clicked()
{
    qDebug() << "ON PREV BUTTON CLICKED: " << current_index << " DECK SIZE :" << deck.size();
    ui->card_area->removeWidget(deck.at(current_index)); // Remove the widget currently displayed
    do {
        if(quiz)
        {
            current_index = rand() % deck.size();
        } else {
            current_index = (current_index + 1) % deck.size();   // Update index, mod so it loops back to beginning if too far
        }
    } while(deck.at(current_index) == nullptr);                  // Keep updating until we get to one that isnt a nullptr

    display_card(current_index);                                 // Add the widget at the new index and display it
}

void Deck::deleteCard(int index){
    Flashcard* temp = deck[index];
    deck[index] =deck[deck.size()-1];
    deck[deck.size()-1] = temp;
    deck.pop_back();
}
/*
 * If toggled true, sets a random index and displays in ui
 * otherwise resets deck to start from index 0 (first card)
*/
void Deck::on_quiz_btn_toggled(bool is_set)
{
    quiz = is_set;
    if(deck.size() > 0)   // Make sure that the deck even has any cards...
    {
        ui->card_area->removeWidget(deck.at(current_index));  // remove current displayed widget
        if(quiz) {
            current_index = rand() % deck.size();
            ui->card_area->addWidget(deck.at(current_index)); // Put random
        }
        else {
            current_index = 0;
            ui->card_area->addWidget(deck.at(current_index)); // Put first card
        }
    }
}
/*
 * Helper function to display current card in ui
*/
void Deck::display_card(int index)
{
    ui->card_area->addWidget(deck.at(index));
    ui->card_area->setCurrentWidget(deck.at(index));
}
