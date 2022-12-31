#include "mb_manager.h"
#include "tools.h"
#include <assert.h>

Tools* tools;

MBManager::MBManager(Tools* toolsref){
	nullArg[0].type = MB_DT_NIL;
	tools = toolsref;
}

MBManager::~MBManager(){
}

void MBManager::doRoutine(char* routineName, mb_value_t routine){
	mb_get_routine(bas, &context, routineName, &routine);
    mb_eval_routine(bas, &context, routine, nullArg, 0, NULL);
}

// Main loop
void MBManager::init(){
	doRoutine("INIT", MBManager::initRoutine);
}
void MBManager::tick(){
	doRoutine("TICK", MBManager::tickRoutine);
}
void MBManager::draw(){
	doRoutine("DRAW", MBManager::drawRoutine);
}
void MBManager::end(){
	doRoutine("END", MBManager::endRoutine);
}

// Init and close
void MBManager::OpenBas()
{
    const char *f = "assets/test.bas";
    int pos;
    unsigned short row;
    unsigned short col;
    mb_error_e e;
    int run;

	mb_init();
	mb_open(&bas);

	mb_reg_fun(bas, cls);
	mb_reg_fun(bas, drawtext);
	mb_reg_fun(bas, drawrect);
	
	mb_reg_fun(bas, textformat);
	mb_reg_fun(bas, delta);

	mb_reg_fun(bas, getkeydown);

	mb_begin_module(bas, "MOUSE");
		mb_register_func(bas, "X", mouseX); 
		mb_register_func(bas, "Y", mouseY); 
	mb_end_module(bas);

    mb_load_file(bas, f);
    
	run = mb_run(bas, true);
    e = mb_get_last_error(bas, &f, &pos, &row, &col);

    if(run > 0){
        printf("Run code: %i, \n",run);
        printf("ERROR Code: %i, pos = %i, row  = %i col = %i \n",e , pos, row, col);
    }
}
void MBManager::CloseBas()
{
	mb_close(&bas);
	mb_dispose();
}


// Raylib funcions
// Draw
int MBManager::cls(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);
	
	int col;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ClearBackground(tools->GetColor(col));

	return MB_FUNC_OK;
}
int MBManager::drawtext(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    char* arg = 0;
	int x = 0;
	int y = 0;
	int size = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &size));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    DrawText(arg, x, y, size, tools->GetColor(col));

	return MB_FUNC_OK;
}
int MBManager::drawrect(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int col = 0;
	int style = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &w));
		mb_check(mb_pop_int(s, l, &h));
		mb_check(mb_pop_int(s, l, &col));
		mb_check(mb_pop_int(s, l, &style));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style)
	{
		case 0:
			DrawRectangleLines(x, y, w, h, tools->GetColor(col));
			break;
		default:
			DrawRectangle(x, y, w, h, tools->GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
//Tools
int MBManager::textformat(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	char* arg = 0;
	mb_value_t value;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_value(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	if (value.type == MB_DT_INT){
    	ret.value.string = (char *)TextFormat(arg, value.value.integer);
	}else{
    	ret.value.string = (char *)TextFormat(arg, value.value.float_point);
	}

    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;

	/*
	TODO: MULTIPLE ARGS, darle una pensadita
	while(mb_has_arg(s, l)) {
	mb_check(mb_pop_real(s, l, &tmp));
	if(tmp < ret)
		ret = tmp;
	}
	*/
}
int MBManager::delta(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = (int)(GetFrameTime()*1000);


    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
// Input  
int MBManager::getkeydown(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsKeyDown(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::mouseX(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = tools->GetVirtualMouse(true);

    mb_check(mb_push_value(s, l, position));

	return MB_FUNC_OK;
}

int MBManager::mouseY(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);
	
    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = tools->GetVirtualMouse(false);

    mb_check(mb_push_value(s, l, position));

	return MB_FUNC_OK;
}