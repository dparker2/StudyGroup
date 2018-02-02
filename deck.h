#ifndef DECK_H
#define DECK_H

#include <QWidget>

namespace Ui {
class Deck;
}

class Deck : public QWidget
{
    Q_OBJECT

public:
    explicit Deck(QWidget *parent = 0);
    ~Deck();

private:
    Ui::Deck *ui;
};

#endif // DECK_H
