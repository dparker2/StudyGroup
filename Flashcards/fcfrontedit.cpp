#include "fcfrontedit.h"
#include "server.h"

FCFrontEdit::FCFrontEdit(QString g, int i, FCSide* s) : FCEdit(g, i, "Front", s)
{
    my_code = server::FLASHCARD_SET_FRONT;
}
