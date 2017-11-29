#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QWidget>

namespace Ui {
class Flashcard;
}

class Flashcard : public QWidget
{
    Q_OBJECT

public:
    explicit Flashcard(QWidget *parent = 0);
    ~Flashcard();

private:
    Ui::Flashcard *ui;
};

#endif // FLASHCARD_H
