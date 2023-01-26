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
	CloseBas();
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
		mb_register_func(bas, "RING", drawRing);
		mb_register_func(bas, "ELLIPSE", drawEllipse);
		mb_register_func(bas, "TRIANGLE", drawTriangle);
		mb_register_func(bas, "RECT", drawRect);
		mb_register_func(bas, "POLY", drawPoly);

		mb_register_func(bas, "TEXT", drawText);
		mb_register_func(bas, "FONT", drawFont);
		
	mb_end_module(bas);

	mb_reg_fun(bas, intToText);
	mb_reg_fun(bas, floatToText);
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
		mb_register_func(bas, "SETPOS", setMousePosition); 
	mb_end_module(bas);

	mb_begin_module(bas, "SOUND");
		mb_register_func(bas, "MUSIC", setSequence); 
		mb_register_func(bas, "NOTE", playNote);
		mb_register_func(bas, "STOP", stopNote);
	mb_end_module(bas);

	mb_begin_module(bas, "SFX");
		mb_register_func(bas, "SET", sfxSet); 
		mb_register_func(bas, "PLAY", sfxPlay);
	mb_end_module(bas);

	mb_reg_fun(bas, sprite);
	mb_reg_fun(bas, setColor);
	mb_reg_fun(bas, restorePalette);
	mb_reg_fun(bas, measureText);

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
int MBManager::drawRing(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int rIn = 0;
	int rOut = 0;
	int startAngle = 0;
	int endAngle = 0;
	int segment = 0;
	int style = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &rIn));
		mb_check(mb_pop_int(s, l, &rOut));
		mb_check(mb_pop_int(s, l, &startAngle));
		mb_check(mb_pop_int(s, l, &endAngle));
		mb_check(mb_pop_int(s, l, &segment));
		mb_check(mb_pop_int(s, l, &style));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (style){
		case 0:
			DrawRingLines((Vector2){x, y}, rIn,rOut,startAngle,endAngle,segment,Tools::GetColor(col));
			break;
		default:
			DrawRing((Vector2){x, y}, rIn,rOut,startAngle,endAngle,segment,Tools::GetColor(col));
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
	int lineThick = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &w));
		mb_check(mb_pop_int(s, l, &h));
		mb_check(mb_pop_int(s, l, &lineThick));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (lineThick)
	{
		case 0:
			DrawRectangle(x, y, w, h, Tools::GetColor(col));
			break;
		default:
			DrawRectangleLinesEx((Rectangle){x, y, w, h}, lineThick, Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
int MBManager::drawPoly(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int sides = 0;
	int radius = 0;
	int rotation = 0;
	int lineThick = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &sides));
		mb_check(mb_pop_int(s, l, &radius));
		mb_check(mb_pop_int(s, l, &rotation));
		mb_check(mb_pop_int(s, l, &lineThick));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	switch (lineThick){
		case 0:
			DrawPoly((Vector2){x, y},sides,radius,rotation,Tools::GetColor(col));
			break;
		case 1:
			DrawPolyLines((Vector2){x, y},sides,radius,rotation,Tools::GetColor(col));
			break;
		default:
			DrawPolyLinesEx((Vector2){x, y},sides,radius,rotation,lineThick,Tools::GetColor(col));
			break;
	}

	return MB_FUNC_OK;
}
//Text
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
int MBManager::drawFont(struct mb_interpreter_t* s, void** l){

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

    DrawTextEx(Tools::GetFont(),arg, (Vector2){x, y}, size, 1,Tools::GetColor(col));

	return MB_FUNC_OK;
}
int MBManager::measureText(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    char* arg = 0;
	int size = 0;

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &size));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = MeasureText(arg, size);

    mb_check(mb_push_value(s, l, ret));
	return MB_FUNC_OK;
}
//Tools
int MBManager::intToText(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	char* arg = 0;
	mb_value_t val[4];

    mb_value_t ret;
    mb_make_string(ret, 0);

	int i = 0;
	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		while(mb_has_arg(s, l)) {
			mb_check(mb_pop_value(s, l, &val[i]));
			if(val[i].type == MB_DT_REAL)
				val[i].value.integer = (int)val[i].value.float_point;
			i++;
			if (i == 4) break;
		}
	mb_check(mb_attempt_close_bracket(s, l));
	
	switch (i)
	{	
		case 1:
			ret.value.string = (char *)TextFormat(arg, val[0].value.integer);
		break;
		case 2:
			ret.value.string = (char *)TextFormat(arg, val[0].value.integer, val[1].value.integer);
		break;
		case 3:
			ret.value.string = (char *)TextFormat(arg, val[0].value.integer, val[1].value.integer, val[2].value.integer);
		break;
		case 4:
			ret.value.string = (char *)TextFormat(arg, val[0].value.integer, val[1].value.integer, val[2].value.integer, val[3].value.integer);
		break;

		default:
		break;
	}

    mb_check(mb_push_value(s, l, ret));

	return MB_FUNC_OK;
}
int MBManager::floatToText(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	char* arg = 0;
	mb_value_t val[4];

    mb_value_t ret;
    mb_make_string(ret, 0);

	int i = 0;
	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		while(mb_has_arg(s, l)) {
			mb_check(mb_pop_value(s, l, &val[i]));
			if(val[i].type == MB_DT_INT)
				val[i].value.float_point = (float)val[i].value.integer;
			i++;
			if (i == 4) break;
		}
	mb_check(mb_attempt_close_bracket(s, l));
	
	switch (i)
	{	
		case 1:
			ret.value.string = (char *)TextFormat(arg, val[0].value.float_point);
		break;
		case 2:
			ret.value.string = (char *)TextFormat(arg, val[0].value.float_point, val[1].value.float_point);
		break;
		case 3:
			ret.value.string = (char *)TextFormat(arg, val[0].value.float_point, val[1].value.float_point, val[2].value.float_point);
		break;
		case 4:
			ret.value.string = (char *)TextFormat(arg, val[0].value.float_point, val[1].value.float_point, val[2].value.float_point, val[3].value.float_point);
		break;

		default:
		break;
	}

    mb_check(mb_push_value(s, l, ret));

	return MB_FUNC_OK;
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
int MBManager::setColor(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    int color = 0;
    int r = 0;
    int g = 0;
    int b = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &color));
		mb_check(mb_pop_int(s, l, &r));
		mb_check(mb_pop_int(s, l, &g));
		mb_check(mb_pop_int(s, l, &b));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	Tools::SetColor(color,r,g,b);

	return MB_FUNC_OK;
}
int MBManager::restorePalette(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

	Tools::CopyPalette();
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
int MBManager::setMousePosition(struct mb_interpreter_t* s, void** l){
	
	mb_assert(s && l);

	int x = 0;
	int y = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    Tools::SetVirtualMouse(x,y);
	return MB_FUNC_OK;
}

//Sound
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
int MBManager::stopNote(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    int key;
	int voice;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &key));
		mb_check(mb_pop_int(s, l, &voice));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->StopNote(key, voice);

	return MB_FUNC_OK;
}
int MBManager::sfxSet(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    int id;
	int wave;


	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &wave));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXSet(id, wave);

	return MB_FUNC_OK;
}
int MBManager::sfxPlay(struct mb_interpreter_t* s, void** l){

	mb_assert(s && l);

    int id;
    int fq;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &fq));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXPlay(id,fq);

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