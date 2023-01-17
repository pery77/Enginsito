#include "mb_manager.h"
#include <assert.h>

AudioManager* audioR;
Texture2D texture;

MBManager::MBManager(){
	nullArg[0].type = MB_DT_NIL;
	audioR = new AudioManager();
	texture = LoadTexture("assets/texture.png");
}

MBManager::~MBManager(){
}
void MBManager::UpdateAudio(){
	audioR->Update();
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
int MBManager::OpenBas(const char * file){
	basFile = file;
	mb_init();
	mb_open(&bas);

	mb_reg_fun(bas, cls);
	mb_begin_module(bas, "DRAW");
		mb_register_func(bas, "PIXEL", drawPixel);
		mb_register_func(bas, "LINE", drawLine);
		mb_register_func(bas, "CIRCLE", drawCircle);
		mb_register_func(bas, "ELLIPSE", drawEllipse);
		mb_register_func(bas, "TRIANGLE", drawTriangle);
		mb_register_func(bas, "RECT", drawRect);

		mb_register_func(bas, "TEXT", drawText);
		
	mb_end_module(bas);

	mb_reg_fun(bas, textFormat);
	mb_reg_fun(bas, delta);

	mb_begin_module(bas, "KEY");
		mb_register_func(bas, "PRESSED", keyPressed);
		mb_register_func(bas, "DOWN", keyDown);
		mb_register_func(bas, "RELEASED", keyReleased);
		mb_register_func(bas, "UP", keyUp);
		mb_register_func(bas, "GET", getKey);
		mb_register_func(bas, "CHAR", getKeyChar);
	mb_end_module(bas);

	mb_begin_module(bas, "MOUSE");
		mb_register_func(bas, "X", mouseX); 
		mb_register_func(bas, "Y", mouseY); 
		mb_register_func(bas, "WHEEL", mouseWheel); 
		mb_register_func(bas, "PRESSED", mousePressed); 
		mb_register_func(bas, "DOWN", mouseDown); 
		mb_register_func(bas, "RELEASED", mouseReleased); 
		mb_register_func(bas, "UP", mouseUp); 
	mb_end_module(bas);

	mb_begin_module(bas, "SOUND");
		mb_register_func(bas, "MUSIC", setSequence); 
		mb_register_func(bas, "NOTE", playNote);
	mb_end_module(bas);

	mb_reg_fun(bas, sprite);

    int loadState = mb_load_file(bas, file);
	switch (loadState){
		case 0:
			printf("Loading %s\n",file);
			break;
		case 3:
			printf("Error loading %s\n",file);
		default:
			printf("ERROR %i\n", loadState);
			break;
	}
	return loadState;
	
}    


void MBManager::Run(){
	int pos;
    unsigned short row;
    unsigned short col;
    mb_error_e e;

	int run = mb_run(bas, true);
    e = mb_get_last_error(bas, &basFile, &pos, &row, &col);

    if(run > 0){
        printf("Run code: %i, \n",run);
        printf("ERROR Code: %i, pos = %i, row  = %i col = %i \n",e , pos, row, col);
    }

}
void MBManager::CloseBas(){
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

    ClearBackground(Tools::GetColor(col));

	return MB_FUNC_OK;
}

int MBManager::drawPixel(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	DrawPixel(x, y, Tools::GetColor(col));

	return MB_FUNC_OK;
}
int MBManager::drawLine(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int thick = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x1));
		mb_check(mb_pop_int(s, l, &y1));
		mb_check(mb_pop_int(s, l, &x2));
		mb_check(mb_pop_int(s, l, &y2));
		mb_check(mb_pop_int(s, l, &thick));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	DrawLineEx((Vector2){x1,y1}, (Vector2){x2,y2}, thick, Tools::GetColor(col));

	return MB_FUNC_OK;
}
int MBManager::drawCircle(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int r = 0;
	int style = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &r));
		mb_check(mb_pop_int(s, l, &style));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style){
		case 0:
			DrawCircleLines(x, y, r, Tools::GetColor(col));
			break;
		default:
			DrawCircle(x, y, r, Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
int MBManager::drawEllipse(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int rh = 0;
	int rv = 0;
	int style = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &rh));
		mb_check(mb_pop_int(s, l, &rv));
		mb_check(mb_pop_int(s, l, &style));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style){
		case 0:
			DrawEllipseLines(x, y, rh, rv, Tools::GetColor(col));
			break;
		default:
			DrawEllipse(x, y, rh, rv, Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
int MBManager::drawTriangle(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int x3 = 0;
	int y3 = 0;
	int style = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x1));
		mb_check(mb_pop_int(s, l, &y1));
		mb_check(mb_pop_int(s, l, &x2));
		mb_check(mb_pop_int(s, l, &y2));
		mb_check(mb_pop_int(s, l, &x3));
		mb_check(mb_pop_int(s, l, &y3));
		mb_check(mb_pop_int(s, l, &style));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style){
		case 0:
			DrawTriangleLines((Vector2){x1,y1}, (Vector2){x2,y2}, (Vector2){x3,y3}, Tools::GetColor(col));
			break;
		default:
			DrawTriangle((Vector2){x1,y1}, (Vector2){x2,y2}, (Vector2){x3,y3}, Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
int MBManager::drawRect(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int style = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &w));
		mb_check(mb_pop_int(s, l, &h));
		mb_check(mb_pop_int(s, l, &style));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style)
	{
		case 0:
			DrawRectangleLines(x, y, w, h, Tools::GetColor(col));
			break;
		default:
			DrawRectangle(x, y, w, h, Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}

int MBManager::drawText(struct mb_interpreter_t* s, void** l){

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

    DrawText(arg, x, y, size, Tools::GetColor(col));

	return MB_FUNC_OK;
}

//Tools
int MBManager::textFormat(struct mb_interpreter_t* s, void** l){

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
int MBManager::keyPressed(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsKeyPressed(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::keyDown(struct mb_interpreter_t* s, void** l){
	
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
int MBManager::keyReleased(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsKeyReleased(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::keyUp(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsKeyUp(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}

int MBManager::getKey(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetKeyPressed();
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::getKeyChar(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetCharPressed();
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}

int MBManager::mouseX(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = Tools::GetVirtualMouse(true);

    mb_check(mb_push_value(s, l, position));

	return MB_FUNC_OK;
}
int MBManager::mouseY(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);
	
    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = Tools::GetVirtualMouse(false);

    mb_check(mb_push_value(s, l, position));

	return MB_FUNC_OK;
}
int MBManager::mouseWheel(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);
	
    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = (int)GetMouseWheelMove();

    mb_check(mb_push_value(s, l, position));

	return MB_FUNC_OK;
}

int MBManager::mousePressed(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsMouseButtonPressed(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::mouseDown(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsMouseButtonDown(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::mouseReleased(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsMouseButtonReleased(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::mouseUp(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int keyCode = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &keyCode));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsMouseButtonUp(keyCode);
    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
int MBManager::setSequence(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    char* arg = 0;
	int voice;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &voice));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SetSequence(arg, voice);
	return MB_FUNC_OK;
}
int MBManager::playNote(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    int key;
	int voice;
	int velocity;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &key));
		mb_check(mb_pop_int(s, l, &voice));
		mb_check(mb_pop_int(s, l, &velocity));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->PlayNote(key, voice, velocity);

	return MB_FUNC_OK;
}
int MBManager::sprite(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int id = 0;
	int x = 0;
	int y = 0;
	int size = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &size));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    DrawTexturePro(texture, (Rectangle){id*size,0,size,size}, (Rectangle){x,y,size,size}, {0,0}, 0.0, WHITE);

	return MB_FUNC_OK;
}