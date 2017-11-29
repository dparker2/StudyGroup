#include "flashcard.h"
#include "ui_flashcard.h"

Flashcard::Flashcard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Flashcard)
{
    ui->setupUi(this);
}

Flashcard::~Flashcard()
{
    delete ui;
}
