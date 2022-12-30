#include "mb_manager.h"
#include "tools.h"
#include <assert.h>

mbManager::mbManager()
{
	nullArg[0].type = MB_DT_NIL;
}

mbManager::~mbManager()
{
}

void mbManager::doRoutine(char* routineName, mb_value_t routine){
	mb_get_routine(bas, &context, routineName, &routine);
    mb_eval_routine(bas, &context, routine, nullArg, 0, NULL);
}
// Main loop
void mbManager::init(){
	doRoutine("INIT", mbManager::initRoutine);
}
void mbManager::tick(){
	doRoutine("TICK", mbManager::tickRoutine);
}
void mbManager::draw(){
	doRoutine("DRAW", mbManager::drawRoutine);
}
void mbManager::end(){
	doRoutine("END", mbManager::endRoutine);
}

void mbManager::OpenBas()
{
    const char *f = "assets/test.bas";
    int pos;
    unsigned short row;
    unsigned short col;
    mb_error_e e;
    int run;

	mb_init();
	mb_open(&bas);

	mb_reg_fun(bas, drawtext);
	/*
	mb_reg_fun(bas, cls);
	mb_reg_fun(bas, getkeydown);
	mb_reg_fun(bas, mousex);
	mb_reg_fun(bas, mousey);
	mb_reg_fun(bas, textformat);
	*/
    mb_load_file(bas, f);
    
	run = mb_run(bas, true);
    e = mb_get_last_error(bas, &f, &pos, &row, &col);

    if(run > 0)
    {
        printf("Run code: %i, \n",run);
        printf("ERROR Code: %i, pos = %i, row  = %i col = %i \n",e , pos, row, col);
    }
}
void mbManager::CloseBas()
{
	mb_close(&bas);
	mb_dispose();
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
    
