#include "fcbackedit.h"
#include "server.h"

FCBackEdit::FCBackEdit(QString g, int i, FCSide* s) : FCEdit(g, i, "Back", s)
{
    my_code = server::FLASHCARD_SET_BACK;
}
