#include "mb_manager.h"
#include <assert.h>
#include "postProcessing.h"
#include "engine.h"
#include "editor.h"
#include "sprite_manager.h"

AudioManager* audioR;
Engine* basicEngineRef;
uint32_t currentframe = 0;

MBManager::MBManager(Engine* _engine){
	nullArg[0].type = MB_DT_NIL;
	audioR = new AudioManager(_engine);
	basicEngineRef = _engine;
}

MBManager::~MBManager(){
}

void MBManager::managerError(int state){
	int pos;
    unsigned short row;
    unsigned short col;

    mb_error_e error = mb_get_last_error(bas, &basFile, &pos, &row, &col);
	const char* errorDes = mb_get_error_desc(error);
    if(state > 0){
        Tools::console->AddLog("[ERROR] [%i]:%s\nline: %i, col: %i, pos; %i\n",error, errorDes, row, col, pos);

		char buffer[256];
		sprintf(buffer, "ERROR [%i]\n:%s\n", error, errorDes); 
		std::string s = buffer;
		basicEngineRef->editor->SetError(row, col, s); 
	}
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
	currentframe++;
	doRoutine("TICK", MBManager::tickRoutine);
}
void MBManager::pause(){
	currentframe++;
	doRoutine("PAUSE", MBManager::pauseRoutine);
}
void MBManager::draw(){
	doRoutine("DRAW", MBManager::drawRoutine);
}
void MBManager::close(){
	doRoutine("CLOSE", MBManager::closeRoutine);
	CloseBas();
}

// Init and close
int MBManager::OpenBas(const char *file){ 
	
	mb_init();
	mb_open(&bas);

	mb_set_printer(bas, my_print);
	mb_set_inputer(bas, my_input);

	mb_reg_fun(bas, cls);
	mb_begin_module(bas, "DRAW");
		mb_register_func(bas, "PIXEL", drawPixel);
		mb_register_func(bas, "LINE", drawLine);
		mb_register_func(bas, "CIRCLE", drawCircle);
		mb_register_func(bas, "RING", drawRing);
		mb_register_func(bas, "ELLIPSE", drawEllipse);
		mb_register_func(bas, "TRIANGLE", drawTriangle);
		mb_register_func(bas, "RECT", drawRect);
		mb_register_func(bas, "RECTROUND", drawRectRound);
		mb_register_func(bas, "POLY", drawPoly);

		mb_register_func(bas, "TEXT", drawText);

		mb_register_func(bas, "SPRITE", drawSprite);
		mb_register_func(bas, "META", drawMetaSprite);
		
	mb_end_module(bas);

	mb_begin_module(bas, "CRT");
		mb_register_func(bas, "ENABLED", crtEnabled);
		mb_register_func(bas, "BLURPOWER", crtBlurPower);
		mb_register_func(bas, "BLURFACTOR", crtBlurFactor);
		mb_register_func(bas, "CHROMATIC", crtChromatic);
		mb_register_func(bas, "CURVATURE", crtCurvature);
		mb_register_func(bas, "VIGNETTING", crtVignetting);
		mb_register_func(bas, "SCANLINE", crtScanline);
		mb_register_func(bas, "GRILLE", crtSetGrille);
		mb_register_func(bas, "VERTICALLINE", crtVerticalLine);
		mb_register_func(bas, "GRILLEFORCE", crtGrilleForce);
		mb_register_func(bas, "NOISE", crtNoise);
		mb_register_func(bas, "FLIKER", crtFliker);
	mb_end_module(bas);

	mb_reg_fun(bas, intToText);
	mb_reg_fun(bas, floatToText);
	mb_reg_fun(bas, delta);
	mb_reg_fun(bas, frame);
	mb_reg_fun(bas, getChar);
	mb_reg_fun(bas, setFontSpacing);

	mb_reg_fun(bas, setSprite);
	mb_reg_fun(bas, getSpriteByte);
	mb_reg_fun(bas, peek);
	mb_reg_fun(bas, poke);
	mb_reg_fun(bas, toSigned);
	mb_reg_fun(bas, dumpMemory);
	mb_reg_fun(bas, loadMemory);

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

	mb_begin_module(bas, "PAD");
		mb_register_func(bas, "ISAVIABLE", isGamepadAvailable);
		mb_register_func(bas, "NAME", getGamepadName);
		mb_register_func(bas, "PRESSED", buttonPressed);
		mb_register_func(bas, "DOWN", buttonDown);
		mb_register_func(bas, "RELEASED", buttonReleased);
		mb_register_func(bas, "UP", buttonUp);
		mb_register_func(bas, "GET", getButton);

		mb_register_func(bas, "AXISCOUNT", getAxisCount);
		mb_register_func(bas, "AXISVALUE", axisValue);
	mb_end_module(bas);

	mb_begin_module(bas, "MUSIC");
		mb_register_func(bas, "SET", setSequence); 
		mb_register_func(bas, "NOTE", playNote);
		mb_register_func(bas, "NOTEOFF", stopNote);
		mb_register_func(bas, "PLAY", musicPlay);
		mb_register_func(bas, "STOP", musicStop);
		mb_register_func(bas, "POSITION", getMusicPosition);
		mb_register_func(bas, "SIZE", getMusicSize);
		mb_register_func(bas, "TICK", getMusicTick);
		mb_register_func(bas, "ENV", setEnv);
		mb_register_func(bas, "LFO", setLFO);
		mb_register_func(bas, "OSC", setOsc);
	mb_end_module(bas);

	mb_begin_module(bas, "SFX");
		mb_register_func(bas, "RENDER", sfxRender); 
		mb_register_func(bas, "WAVE", sfxWave); 
		mb_register_func(bas, "ENV", sfxEnv); 
		mb_register_func(bas, "FREQ", sfxFreq); 
		mb_register_func(bas, "TONE", sfxTone); 
		mb_register_func(bas, "REPEAT", sfxRepeat); 
		mb_register_func(bas, "FILTER", sfxFilter); 
		mb_register_func(bas, "PLAY", sfxPlay);
		mb_register_func(bas, "STOP", sfxStop);
	mb_end_module(bas);


	mb_reg_fun(bas, addMetaSprite);
	mb_reg_fun(bas, getMetaSprite);
	mb_reg_fun(bas, setColor);
	mb_reg_fun(bas, getColor);
	mb_reg_fun(bas, measureText);
	mb_reg_fun(bas, getFiles);
	mb_reg_fun(bas, getFolders);
	mb_reg_fun(bas, saveFile);

    int loadState = mb_load_file(bas, file);
	basFile = file;
	Tools::console->AddLog("Loading: [ %s ]\n", basFile);
	managerError(loadState);
	
	return loadState;
}    

void MBManager::Run()
{
	currentframe = 0;
	int run = mb_run(bas, true);
	basicEngineRef->editor->ClearError(); 
	managerError(run);
}

void MBManager::CloseBas()
{
	mb_close(&bas);
	mb_dispose();
}

// Raylib funcions
// Draw
int MBManager::cls(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	
	int col;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ClearBackground(basicEngineRef->spriteManager->GetColor(col));

	return result;
}

int MBManager::drawPixel(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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

	DrawPixel(x, y, basicEngineRef->spriteManager->GetColor(col));

	return result;
}
int MBManager::drawLine(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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

	DrawLineEx((Vector2){x1,y1}, (Vector2){x2,y2}, thick, basicEngineRef->spriteManager->GetColor(col));

	return result;
}
int MBManager::drawCircle(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawCircle(x, y, r, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawCircleLines(x, y, r, basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawRing(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawRing((Vector2){x, y}, rIn,rOut,startAngle,endAngle,segment,basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawRingLines((Vector2){x, y}, rIn,rOut,startAngle,endAngle,segment,basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawEllipse(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawEllipse(x, y, rh, rv, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawEllipseLines(x, y, rh, rv, basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawTriangle(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawTriangle((Vector2){x1,y1}, (Vector2){x2,y2}, (Vector2){x3,y3}, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawTriangleLines((Vector2){x1,y1}, (Vector2){x2,y2}, (Vector2){x3,y3}, basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawRect(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawRectangle(x, y, w, h, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawRectangleLinesEx((Rectangle){x, y, w, h}, lineThick, basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawRectRound(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int lineThick = 0;
	int roundness = 0;
	int segments = 0;
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &w));
		mb_check(mb_pop_int(s, l, &h));
		mb_check(mb_pop_int(s, l, &roundness));
		mb_check(mb_pop_int(s, l, &segments));
		mb_check(mb_pop_int(s, l, &lineThick));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));
 
	float fRound = (float)(roundness) * 0.1f;
	if (segments < 1) segments = 1;
	switch (lineThick)
	{
		case 0:
			DrawRectangleRounded((Rectangle){x, y, w, h}, fRound, segments, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawRectangleRoundedLines((Rectangle){x, y, w, h}, fRound, segments, lineThick, basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
int MBManager::drawPoly(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
			DrawPoly((Vector2){x, y},sides,radius,rotation,basicEngineRef->spriteManager->GetColor(col));
			break;
		case 1:
			DrawPolyLines((Vector2){x, y},sides,radius,rotation,basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawPolyLinesEx((Vector2){x, y},sides,radius,rotation,lineThick,basicEngineRef->spriteManager->GetColor(col));
			break;
	}

	return result;
}
//Text
int MBManager::drawText(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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
	if (size < 1) size = 1;
	if (size > 4) size = 4;
    DrawTextEx(basicEngineRef->spriteManager->font,arg, (Vector2){x, y}, size * 8, basicEngineRef->spriteManager->fontSpacing, basicEngineRef->spriteManager->GetColor(col));

	return result;
}
int MBManager::measureText(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    char* arg = 0;
	int size = 0;

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &size));
	mb_check(mb_attempt_close_bracket(s, l));

	if (size < 1) size = 1;
	if (size > 4) size = 4;

    ret.value.integer = MeasureTextEx(basicEngineRef->spriteManager->font, arg, (float)size * 8, 0.0).x;

    mb_check(mb_push_value(s, l, ret));
	return result;
}
//Tools
inline int getArgIntValue(const mb_value_t& arg) {
	return arg.type == MB_DT_REAL ? (int)arg.value.float_point : arg.value.integer;
}
inline float getArgFloatValue(const mb_value_t& arg) {
	return arg.type == MB_DT_REAL ? arg.value.float_point : (float)arg.value.integer;
}

int MBManager::intToText(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	char* arg = 0;
	std::vector<mb_value_t> vals;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		while(mb_has_arg(s, l)) {
			mb_value_t val;
			mb_check(mb_pop_value(s, l, &val));
			vals.push_back(val);
		}
	mb_check(mb_attempt_close_bracket(s, l));

	std::array<mb_value_t, 8> args{ MB_DT_NIL };
	for (size_t i = 0; i < vals.size() && i < 8; i++) {
		args[i] = vals[i];
	}
	
	ret.value.string = (char *)TextFormat(arg, getArgIntValue(args[0]), getArgIntValue(args[1]), getArgIntValue(args[2]), getArgIntValue(args[3]),
												getArgIntValue(args[4]), getArgIntValue(args[5]), getArgIntValue(args[6]), getArgIntValue(args[7]));
    mb_check(mb_push_value(s, l, ret));

	return result;
}
int MBManager::floatToText(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	char* arg = 0;
	std::vector<mb_value_t> vals;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		while(mb_has_arg(s, l)) {
			mb_value_t val;
			mb_check(mb_pop_value(s, l, &val));
			vals.push_back(val);
		}
	mb_check(mb_attempt_close_bracket(s, l));
	
	std::array<mb_value_t, 8> args{ MB_DT_NIL };
	for (size_t i = 0; i < vals.size() && i < 8; i++) {
		args[i] = vals[i];
	}
	
	ret.value.string = (char *)TextFormat(arg, getArgFloatValue(args[0]), getArgFloatValue(args[1]), getArgFloatValue(args[2]), getArgFloatValue(args[3]),
												getArgFloatValue(args[4]), getArgFloatValue(args[5]), getArgFloatValue(args[6]), getArgFloatValue(args[7]));
    
    mb_check(mb_push_value(s, l, ret));

	return result;
}
int MBManager::delta(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = (int)(GetFrameTime()*1000);


    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::frame(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = currentframe;


    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getChar(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int charValue = 0;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &charValue));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.string = (char*)TextFormat("%s",Tools::GetCharFromCodepoint(charValue).c_str());
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::setFontSpacing(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int spacing = 0;


	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &spacing));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->spriteManager->SetFontSpacing(spacing);

	return result;
}

int MBManager::setSprite(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int id = 0;
	int b0,b1,b2,b3,b4,b5,b6,b7 = 0;


	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &b0));
			mb_check(mb_pop_int(s, l, &b1));
			mb_check(mb_pop_int(s, l, &b2));
			mb_check(mb_pop_int(s, l, &b3));
			mb_check(mb_pop_int(s, l, &b4));
			mb_check(mb_pop_int(s, l, &b5));
			mb_check(mb_pop_int(s, l, &b6));
			mb_check(mb_pop_int(s, l, &b7));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->spriteManager->SetSprite(id,b0,b1,b2,b3,b4,b5,b6,b7);

	return result;
}

int MBManager::getSpriteByte(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int id = 0;
	int byte = 0;
	int ret = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &byte));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	ret = basicEngineRef->spriteManager->GetSpriteByte(id, byte);
	mb_check(mb_push_int(s, l, ret));
	return result;
}
int MBManager::setColor(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
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

	basicEngineRef->spriteManager->SetColor(color,r,g,b);

	return result;
}
int MBManager::getColor(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int color = 0;
    int channel = 0;
	int ret = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &color));
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

	Color retColor = basicEngineRef->spriteManager->GetColor(color);
	
	switch (channel){
		case 1:
			ret = retColor.g;
			break;
		case 2:
			ret = retColor.b;
			break;
		default:
			ret = retColor.r;
			break;
	} 
	
	mb_check(mb_push_int(s, l, ret));

	return result;
}

int MBManager::getFiles(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	
	void* arr = 0;
	int d[1] = { 0 };
	int i = 0;

	mb_value_t val;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
	mb_check(mb_attempt_close_bracket(s, l));

	std::stringstream ss = Tools::GetFiles(arg);
    std::string temp;

	int lines = 0;
	while (std::getline(ss, temp)){
		lines++;
    }
	if (lines > 0){
		ss = Tools::GetFiles(arg);
		d[0] = lines;
		mb_init_array(s, l, MB_DT_STRING, d, 1, &arr);

		while (std::getline(ss, temp)){
			//temp.push_back('\n');
			val.type = MB_DT_STRING;
			val.value.string = (char *)temp.c_str();
			d[0] = i++;
			mb_set_array_elem(s, l, arr, d, 1, val);
		}
		val.type = MB_DT_ARRAY;
		val.value.array = arr;
		mb_check(mb_push_value(s, l, val));
	}

	return result;
}
int MBManager::getFolders(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	
	void* arr = 0;
	int d[1] = { 0 };
	int i = 0;

	mb_value_t val;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
	mb_check(mb_attempt_close_bracket(s, l));

	std::stringstream ss = Tools::GetFolders(arg);
    std::string temp;

	int lines = 0;
	while (std::getline(ss, temp)){
		lines++;
    }
	if (lines > 0){
		ss = Tools::GetFolders(arg);
		d[0] = lines;
		mb_init_array(s, l, MB_DT_STRING, d, 1, &arr);

		while (std::getline(ss, temp)){
			//temp.push_back('\n');
			val.type = MB_DT_STRING;
			val.value.string = (char *)temp.c_str();
			d[0] = i++;
			mb_set_array_elem(s, l, arr, d, 1, val);
		}
		val.type = MB_DT_ARRAY;
		val.value.array = arr;
		mb_check(mb_push_value(s, l, val));
	}

	return result;
}
int MBManager::saveFile(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	char* txt = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_string(s, l, &txt));
	mb_check(mb_attempt_close_bracket(s, l));
	
	bool saved = SaveFileText(arg, txt);
	
	if (saved) Tools::console->AddLog("Saved OK: %s\n", arg);
	else Tools::console->AddLog("[ERROR] saving: %s\n", arg);
	
	return result;
}

// Input
int MBManager::keyPressed(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::keyDown(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::keyReleased(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::keyUp(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}

int MBManager::getKey(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetKeyPressed();
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getKeyChar(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetCharPressed();
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::isGamepadAvailable(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int gamePadId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsGamepadAvailable(gamePadId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getGamepadName(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_string(ret, 0);

    int gamePadId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.string = (char *)GetGamepadName(gamePadId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::buttonPressed(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int gamePadId, buttonId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
		mb_check(mb_pop_int(s, l, &buttonId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsGamepadButtonPressed(gamePadId, buttonId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::buttonDown(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int gamePadId, buttonId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
		mb_check(mb_pop_int(s, l, &buttonId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsGamepadButtonDown(gamePadId, buttonId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::buttonReleased(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int gamePadId, buttonId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
		mb_check(mb_pop_int(s, l, &buttonId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsGamepadButtonReleased(gamePadId, buttonId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::buttonUp(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int gamePadId, buttonId = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePadId));
		mb_check(mb_pop_int(s, l, &buttonId));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsGamepadButtonUp(gamePadId, buttonId);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getButton(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetGamepadButtonPressed();
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getAxisCount(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	int gamePad = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePad));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetGamepadAxisCount(gamePad);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::axisValue(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	int gamePad, axis = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &gamePad));
		mb_check(mb_pop_int(s, l, &axis));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = GetGamepadAxisMovement(gamePad, axis) * 100;
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::mouseX(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = Tools::GetVirtualMouse(true);

    mb_check(mb_push_value(s, l, position));

	return result;
}
int MBManager::mouseY(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);
	
    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = Tools::GetVirtualMouse(false);

    mb_check(mb_push_value(s, l, position));

	return result;
}
int MBManager::mouseWheel(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);
	
    mb_value_t position;
    mb_make_int(position, 0);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    position.value.integer = (int)GetMouseWheelMove();

    mb_check(mb_push_value(s, l, position));

	return result;
}

int MBManager::mousePressed(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::mouseDown(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::mouseReleased(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::mouseUp(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}
int MBManager::setMousePosition(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
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
	return result;
}

// Sound
int MBManager::setSequence(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int id;
    char* arg = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_string(s, l, &arg));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SetSequence(id, arg);
	return result;
}
int MBManager::playNote(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int channel, note, volume;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &note));
		mb_check(mb_pop_int(s, l, &volume));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->PlayNote(channel, note, volume);

	return result;
}
int MBManager::stopNote(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int channel;
	int voice;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->StopNote(channel);

	return result;
}
int MBManager::musicPlay(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int key;
	int voice;

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->MusicPlay();

	return result;
}
int MBManager::musicStop(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int key;
	int voice;

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->MusicStop();

	return result;
}
int MBManager::getMusicPosition(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int channel = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = audioR->GetMusicPosition(channel);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getMusicSize(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

    int channel = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = audioR->GetMusicSize(channel);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::getMusicTick(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);


	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = audioR->AudioTick;
    mb_check(mb_push_value(s, l, ret));
	return result;
}
int MBManager::setEnv(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    int channel, attk, dec, sus, rel, amp = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &attk));
		mb_check(mb_pop_int(s, l, &dec));
		mb_check(mb_pop_int(s, l, &sus));
		mb_check(mb_pop_int(s, l, &rel));
		mb_check(mb_pop_int(s, l, &amp));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SetEnv(channel, attk, dec, sus, rel, amp);

	return result;
}

int MBManager::setLFO(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    int channel, note, amp = 0;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &note));
		mb_check(mb_pop_int(s, l, &amp));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SetLFO(channel, note, amp);

	return result;
}
int MBManager::setOsc(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    int channel, osc;

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &osc));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SetOSC(channel, osc);

	return result;
}
//SFX
int MBManager::sfxRender(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int note;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &note));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXRender(id, note);

	return result;
}
int MBManager::sfxWave(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int wave;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &wave));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXWave(id, wave);

	return result;
}
int MBManager::sfxEnv(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int att, susT, susP, dec;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &att));
		mb_check(mb_pop_int(s, l, &susT));
		mb_check(mb_pop_int(s, l, &susP));
		mb_check(mb_pop_int(s, l, &dec));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXEnv(id, att, susT, susP, dec);

	return result;
}
int MBManager::sfxFreq(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int slide, delta, vibratoD, vibratoS;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &slide));
		mb_check(mb_pop_int(s, l, &delta));
		mb_check(mb_pop_int(s, l, &vibratoD));
		mb_check(mb_pop_int(s, l, &vibratoS));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXFreq(id, slide, delta, vibratoD, vibratoS);

	return result;

}
int MBManager::sfxTone(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int ammount, speed, square, duty;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &ammount));
		mb_check(mb_pop_int(s, l, &speed));
		mb_check(mb_pop_int(s, l, &square));
		mb_check(mb_pop_int(s, l, &duty));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXTone(id, ammount, speed, square, duty);

	return result;
}
int MBManager::sfxRepeat(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int  speed, offset, sweep;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &speed));
		mb_check(mb_pop_int(s, l, &offset));
		mb_check(mb_pop_int(s, l, &sweep));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXRepeat(id, speed, offset, sweep);

	return result;
}
int MBManager::sfxFilter(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;
	int lpfCuttoff, lpfSweep, lpfRes, hpfCuttoff, hpfSweep;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &lpfCuttoff));
		mb_check(mb_pop_int(s, l, &lpfSweep));
		mb_check(mb_pop_int(s, l, &lpfRes));
		mb_check(mb_pop_int(s, l, &hpfCuttoff));
		mb_check(mb_pop_int(s, l, &hpfSweep));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXFilter(id, lpfCuttoff, lpfSweep, lpfRes, hpfCuttoff, hpfSweep);

	return result;
}
int MBManager::sfxPlay(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id, vol;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
		mb_check(mb_pop_int(s, l, &vol));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXPlay(id, vol);

	return result;
}
int MBManager::sfxStop(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int id;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &id));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    audioR->SFXStop(id);

	return result;
}
// Sprites
int MBManager::drawSprite(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int id = 0;
	int x = 0;
	int y = 0;
	int col = 0;
	int flag = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &x));
			mb_check(mb_pop_int(s, l, &y));
			mb_check(mb_pop_int(s, l, &col));
		}
		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &flag));
		}
	mb_check(mb_attempt_close_bracket(s, l));

	basicEngineRef->spriteManager->DrawSprite(id, x, y, col, flag);

	return result;
}
int MBManager::drawMetaSprite(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int id = 0;
	int x = 0;
	int y = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &x));
			mb_check(mb_pop_int(s, l, &y));
		}
	mb_check(mb_attempt_close_bracket(s, l));

	basicEngineRef->spriteManager->DrawMetaSprite(id, x, y);
	
	return result;
}
// id, postition, sprite_id, offset_x,  offset_y,  color, flags
int MBManager::addMetaSprite(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int id, pos = 0;
	int spriteId,x,y,col,flag = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &pos));
			mb_check(mb_pop_int(s, l, &spriteId));
			mb_check(mb_pop_int(s, l, &x));
			mb_check(mb_pop_int(s, l, &y));
			mb_check(mb_pop_int(s, l, &col));
			mb_check(mb_pop_int(s, l, &flag));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->spriteManager->AddMetaSprite(id,pos,spriteId,x,y,col,flag);

	return result;
}
int MBManager::getMetaSprite(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	int id = 0;
	
	void* arr = 0;
	int d[1] = { 0 };
	int i = 0;

	mb_value_t val;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_attempt_close_bracket(s, l));

	d[0] = 20;
	mb_init_array(s, l, MB_DT_REAL, d, 1, &arr);

	std::array<int,20> r = basicEngineRef->spriteManager->GetMetaSprite(id);
    for(int i=0;i<20;i++)
    {
		val.type = MB_DT_INT;
		val.value.integer = r[i];
		d[0] = i;
		mb_set_array_elem(s, l, arr, d, 1, val);
    }
	
	val.type = MB_DT_ARRAY;
	val.value.array = arr;
	mb_check(mb_push_value(s, l, val));

	return result;
}
// Memory
int MBManager::peek(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int dir;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &dir));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	mb_check(mb_push_int(s, l, basicEngineRef->Peek(dir)));

	return result;
}
int MBManager::poke(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int dir,value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &dir));
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->Poke(dir,value);

	return result;
}
int MBManager::toSigned(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;
	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	mb_check(mb_push_int(s, l, Tools::ToSigned(value)));

	return result;
}

int MBManager::dumpMemory(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* path = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &path));
	mb_check(mb_attempt_close_bracket(s, l));
	
	basicEngineRef->DumpMemory(path);
	
	return result;
}
int MBManager::loadMemory(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* path = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &path));
	mb_check(mb_attempt_close_bracket(s, l));
	
	basicEngineRef->LoadMemory(path);
	
	return result;
}

// CRT
int MBManager::crtEnabled(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetState(value == 0 ? false : true);
	return result;
}
int MBManager::crtBlurPower(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::BlurPower, value);
	return result;
}
int MBManager::crtBlurFactor(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::BlurFactor, value);
	return result;
}
int MBManager::crtChromatic(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::Chromatic, value);
	return result;
}
int MBManager::crtCurvature(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::Curvature, value);
	return result;
}
int MBManager::crtVignetting(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::Vignetting, value);
	return result;
}
int MBManager::crtScanline(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::ScanLine, value);
	return result;
}
int MBManager::crtSetGrille(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetGrilleTexture(value);
	return result;
}
int MBManager::crtVerticalLine(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::VerticalLine, value);
	return result;
}
int MBManager::crtGrilleForce(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::GrilleForce, value);
	return result;
}
int MBManager::crtNoise(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::Noise, value);
	return result;
}
int MBManager::crtFliker(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int value;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &value));
	}
	mb_check(mb_attempt_close_bracket(s, l));

   	basicEngineRef->postProcessing->SetCRTValue(CRTProperty::Fliker, value);
	return result;
}

int MBManager::my_print(struct mb_interpreter_t* s, const char* fmt, ...) {
	char buf[64];
	char* ptr = buf;
	size_t len = sizeof(buf);
	int result = 0;
	va_list argptr;
	mb_unrefvar(s);

	va_start(argptr, fmt);
	result = vsnprintf(ptr, len, fmt, argptr);
	if(result < 0) {
		fprintf(stderr, "Encoding error.\n");
	} else if(result > (int)len) {
		len = result + 1;
		ptr = (char*)malloc(result + 1);
		result = vsnprintf(ptr, len, fmt, argptr);
	}
	va_end(argptr);
	if(result >= 0)
		Tools::console->AddLog(ptr); /* Change me */
	if(ptr != buf)
		free(ptr);

	return result;
}

int MBManager::my_input(struct mb_interpreter_t* s, const char* pmt, char* buf, int n) {
	int result = 0;
	mb_unrefvar(s);

	if(fgets(buf, n, stdin) == 0) { /* Change me */
		fprintf(stderr, "Error reading.\n");
		exit(1);
	}
	result = (int)strlen(buf);
	if(buf[result - 1] == '\n')
		buf[result - 1] = '\0';

	return result;
}