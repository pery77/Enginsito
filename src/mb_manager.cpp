#include "mb_manager.h"
#include "tools.h"
#include <assert.h>

mbManager::mbManager()
{

}

mbManager::~mbManager()
{

}



int mbManager::drawtext(struct mb_interpreter_t* s, void** l)
{
    char* arg = 0;
	int x = 0;
	int y = 0;
	int size = 0;
	int col = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &size));
		mb_check(mb_pop_int(s, l, &col));
	}

	mb_check(mb_attempt_close_bracket(s, l));

    DrawText(arg, x, y, size, tools::getColor(col));

	return MB_FUNC_OK;
}
    
