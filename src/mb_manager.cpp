#include "mb_manager.h"
#include <assert.h>
#include "postProcessing.h"
#include "engine.h"
#include "editor.h"
#include "sprite_manager.h"
#include <stdarg.h>
#include <cstring>

Engine* basicEngineRef;
uint32_t currentframe = 0;
mb_value_t cFrame;
mb_value_t cDelta;
mb_value_t cMouseX;
mb_value_t cMouseY;

MBManager::MBManager(Engine* _engine){
	nullArg[0].type = MB_DT_NIL;
	basicEngineRef = _engine;
}

MBManager::~MBManager(){
}
uint32_t MBManager::GetCurrentFrame()
{
	return currentframe;
}

void MBManager::managerError(int state){
	int pos;
    unsigned short row;
    unsigned short col;

    mb_error_e error = mb_get_last_error(bas, &basFile, &pos, &row, &col);
	const char* errorDes = mb_get_error_desc(error);
    if(state > 0 && error > 0){
        Tools::console->AddLog("[ERROR] [%i]:%s\nline: %i, col: %i, pos; %i\n",error, errorDes, row, col, pos);

		char buffer[256];
		sprintf(buffer, "ERROR [%i]\n:%s\n", error, errorDes); 
		std::string s = buffer;
		basicEngineRef->editor->SetError(row, col, s); 
	}
}

void MBManager::UpdateAudio(){
	basicEngineRef->audioManager->Update();
}
void MBManager::UpdateVars(){
	updateVars(bas, &context);
}
inline void MBManager::doRoutine(char* routineName, mb_value_t routine){
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
	doRoutine("PAUSE", MBManager::pauseRoutine);
}
void MBManager::draw(){
	doRoutine("DRAW", MBManager::drawRoutine);
}
void MBManager::close(){
	doRoutine("CLOSE", MBManager::closeRoutine);
	CloseBas();
}

int MBManager::updateVars(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	cFrame.value.integer = currentframe;
	cDelta.value.float_point = GetFrameTime();
	cMouseX.value.integer = basicEngineRef->VirtualMouseX;
	cMouseY.value.integer = basicEngineRef->VirtualMouseY;

	mb_add_var(s, l, "FRAME", cFrame, true);
	mb_add_var(s, l, "DELTA", cDelta, true);
	mb_add_var(s, l, "MOUSE_X", cMouseX, true);
	mb_add_var(s, l, "MOUSE_Y", cMouseY, true);

	return result;
}

// Init and close
int MBManager::OpenBas(const char *file){ 
	
	mb_init();
	mb_open(&bas);

	mb_set_printer(bas, my_print);
	//mb_set_inputer(bas, my_input);

	mb_make_int(cFrame, 0);
	mb_make_real(cDelta, 0);
	mb_make_int(cMouseX, 0);
	mb_make_int(cMouseY, 0);

	//Vars
	mb_add_var(bas, &context, "FRAME", cFrame, true);
	mb_add_var(bas, &context, "DELTA", cDelta, true);
	mb_add_var(bas, &context, "MOUSE_X", cMouseX, true);
	mb_add_var(bas, &context, "MOUSE_Y", cMouseY, true);
	
	//Keys enum
	mb_value_t key;
	mb_make_int(key, 0);

	key.value.integer = KEY_APOSTROPHE;
	mb_add_var(bas, &context, "KEY_APOSTROPHE", key, true);
	key.value.integer = KEY_COMMA;
	mb_add_var(bas, &context, "KEY_COMMA", key, true);
	key.value.integer = KEY_MINUS;
	mb_add_var(bas, &context, "KEY_MINUS", key, true);
	key.value.integer = KEY_PERIOD;
	mb_add_var(bas, &context, "KEY_PERIOD", key, true);
	key.value.integer = KEY_SLASH;
	mb_add_var(bas, &context, "KEY_SLASH", key, true);
	key.value.integer = KEY_ZERO;
	mb_add_var(bas, &context, "KEY_ZERO", key, true);
	key.value.integer = KEY_ONE;
	mb_add_var(bas, &context, "KEY_ONE", key, true);
	key.value.integer = KEY_TWO;
	mb_add_var(bas, &context, "KEY_TWO", key, true);
	key.value.integer = KEY_THREE;
	mb_add_var(bas, &context, "KEY_THREE", key, true);
	key.value.integer = KEY_FOUR;
	mb_add_var(bas, &context, "KEY_FOUR", key, true);
	key.value.integer = KEY_FIVE;
	mb_add_var(bas, &context, "KEY_FIVE", key, true);
	key.value.integer = KEY_SIX;
	mb_add_var(bas, &context, "KEY_SIX", key, true);
	key.value.integer = KEY_SEVEN;
	mb_add_var(bas, &context, "KEY_SEVEN", key, true);
	key.value.integer = KEY_EIGHT;
	mb_add_var(bas, &context, "KEY_EIGHT", key, true);
	key.value.integer = KEY_NINE;
	mb_add_var(bas, &context, "KEY_NINE", key, true);
	key.value.integer = KEY_SEMICOLON;
	mb_add_var(bas, &context, "KEY_SEMICOLON", key, true);
	key.value.integer = KEY_EQUAL;
	mb_add_var(bas, &context, "KEY_EQUAL", key, true);
	key.value.integer = KEY_A;
	mb_add_var(bas, &context, "KEY_A", key, true);
	key.value.integer = KEY_B;
	mb_add_var(bas, &context, "KEY_B", key, true);
	key.value.integer = KEY_C;
	mb_add_var(bas, &context, "KEY_C", key, true);
	key.value.integer = KEY_D;
	mb_add_var(bas, &context, "KEY_D", key, true);
	key.value.integer = KEY_E;
	mb_add_var(bas, &context, "KEY_E", key, true);
	key.value.integer = KEY_F;
	mb_add_var(bas, &context, "KEY_F", key, true);
	key.value.integer = KEY_G;
	mb_add_var(bas, &context, "KEY_G", key, true);
	key.value.integer = KEY_H;
	mb_add_var(bas, &context, "KEY_H", key, true);
	key.value.integer = KEY_I;
	mb_add_var(bas, &context, "KEY_I", key, true);
	key.value.integer = KEY_J;
	mb_add_var(bas, &context, "KEY_J", key, true);
	key.value.integer = KEY_K;
	mb_add_var(bas, &context, "KEY_K", key, true);
	key.value.integer = KEY_L;
	mb_add_var(bas, &context, "KEY_L", key, true);
	key.value.integer = KEY_M;
	mb_add_var(bas, &context, "KEY_M", key, true);
	key.value.integer = KEY_N;
	mb_add_var(bas, &context, "KEY_N", key, true);
	key.value.integer = KEY_O;
	mb_add_var(bas, &context, "KEY_O", key, true);
	key.value.integer = KEY_P;
	mb_add_var(bas, &context, "KEY_P", key, true);
	key.value.integer = KEY_Q;
	mb_add_var(bas, &context, "KEY_Q", key, true);
	key.value.integer = KEY_R;
	mb_add_var(bas, &context, "KEY_R", key, true);
	key.value.integer = KEY_S;
	mb_add_var(bas, &context, "KEY_S", key, true);
	key.value.integer = KEY_T;
	mb_add_var(bas, &context, "KEY_T", key, true);
	key.value.integer = KEY_U;
	mb_add_var(bas, &context, "KEY_U", key, true);
	key.value.integer = KEY_V;
	mb_add_var(bas, &context, "KEY_V", key, true);
	key.value.integer = KEY_W;
	mb_add_var(bas, &context, "KEY_W", key, true);
	key.value.integer = KEY_X;
	mb_add_var(bas, &context, "KEY_X", key, true);
	key.value.integer = KEY_Y;
	mb_add_var(bas, &context, "KEY_Y", key, true);
	key.value.integer = KEY_Z;
	mb_add_var(bas, &context, "KEY_Z", key, true);
	key.value.integer = KEY_LEFT_BRACKET;
	mb_add_var(bas, &context, "KEY_LEFT_BRACKET", key, true);
	key.value.integer = KEY_BACKSLASH;
	mb_add_var(bas, &context, "KEY_BACKSLASH", key, true);
	key.value.integer = KEY_RIGHT_BRACKET;
	mb_add_var(bas, &context, "KEY_RIGHT_BRACKET", key, true);
	key.value.integer = KEY_GRAVE;
	mb_add_var(bas, &context, "KEY_GRAVE", key, true);
	key.value.integer = KEY_SPACE;
	mb_add_var(bas, &context, "KEY_SPACE", key, true);
	key.value.integer = KEY_ESCAPE;
	mb_add_var(bas, &context, "KEY_ESCAPE", key, true);
	key.value.integer = KEY_ENTER;
	mb_add_var(bas, &context, "KEY_ENTER", key, true);
	key.value.integer = KEY_TAB;
	mb_add_var(bas, &context, "KEY_TAB", key, true);
	key.value.integer = KEY_BACKSPACE;
	mb_add_var(bas, &context, "KEY_BACKSPACE", key, true);
	key.value.integer = KEY_INSERT;
	mb_add_var(bas, &context, "KEY_INSERT", key, true);
	key.value.integer = KEY_DELETE;
	mb_add_var(bas, &context, "KEY_DELETE", key, true);
	key.value.integer = KEY_RIGHT;
	mb_add_var(bas, &context, "KEY_RIGHT", key, true);
	key.value.integer = KEY_LEFT;
	mb_add_var(bas, &context, "KEY_LEFT", key, true);
	key.value.integer = KEY_DOWN;
	mb_add_var(bas, &context, "KEY_DOWN", key, true);
	key.value.integer = KEY_UP;
	mb_add_var(bas, &context, "KEY_UP", key, true);
	key.value.integer = KEY_PAGE_UP;
	mb_add_var(bas, &context, "KEY_PAGE_UP", key, true);
	key.value.integer = KEY_PAGE_DOWN;
	mb_add_var(bas, &context, "KEY_PAGE_DOWN", key, true);
	key.value.integer = KEY_HOME;
	mb_add_var(bas, &context, "KEY_HOME", key, true);
	key.value.integer = KEY_END;
	mb_add_var(bas, &context, "KEY_END", key, true);
	key.value.integer = KEY_CAPS_LOCK;
	mb_add_var(bas, &context, "KEY_CAPS_LOCK", key, true);
	key.value.integer = KEY_SCROLL_LOCK;
	mb_add_var(bas, &context, "KEY_SCROLL_LOCK", key, true);
	key.value.integer = KEY_NUM_LOCK;
	mb_add_var(bas, &context, "KEY_NUM_LOCK", key, true);
	key.value.integer = KEY_PRINT_SCREEN;
	mb_add_var(bas, &context, "KEY_PRINT_SCREEN", key, true);
	key.value.integer = KEY_PAUSE;
	mb_add_var(bas, &context, "KEY_PAUSE", key, true);
	key.value.integer = KEY_F1;
	mb_add_var(bas, &context, "KEY_F1", key, true);
	key.value.integer = KEY_F2;
	mb_add_var(bas, &context, "KEY_F2", key, true);
	key.value.integer = KEY_F3;
	mb_add_var(bas, &context, "KEY_F3", key, true);
	key.value.integer = KEY_F4;
	mb_add_var(bas, &context, "KEY_F4", key, true);
	key.value.integer = KEY_F5;
	mb_add_var(bas, &context, "KEY_F5", key, true);
	key.value.integer = KEY_F6;
	mb_add_var(bas, &context, "KEY_F6", key, true);
	key.value.integer = KEY_F7;
	mb_add_var(bas, &context, "KEY_F7", key, true);
	key.value.integer = KEY_F8;
	mb_add_var(bas, &context, "KEY_F8", key, true);
	key.value.integer = KEY_F9;
	mb_add_var(bas, &context, "KEY_F9", key, true);
	key.value.integer = KEY_F10;
	mb_add_var(bas, &context, "KEY_F10", key, true);
	key.value.integer = KEY_F11;
	mb_add_var(bas, &context, "KEY_F11", key, true);
	key.value.integer = KEY_F12;
	mb_add_var(bas, &context, "KEY_F12", key, true);
	key.value.integer = KEY_LEFT_SHIFT;
	mb_add_var(bas, &context, "KEY_LEFT_SHIFT", key, true);
	key.value.integer = KEY_LEFT_CONTROL;
	mb_add_var(bas, &context, "KEY_LEFT_CONTROL", key, true);
	key.value.integer = KEY_LEFT_ALT;
	mb_add_var(bas, &context, "KEY_LEFT_ALT", key, true);
	key.value.integer = KEY_LEFT_SUPER;
	mb_add_var(bas, &context, "KEY_LEFT_SUPER", key, true);
	key.value.integer = KEY_RIGHT_SHIFT;
	mb_add_var(bas, &context, "KEY_RIGHT_SHIFT", key, true);
	key.value.integer = KEY_RIGHT_CONTROL;
	mb_add_var(bas, &context, "KEY_RIGHT_CONTROL", key, true);
	key.value.integer = KEY_RIGHT_ALT;
	mb_add_var(bas, &context, "KEY_RIGHT_ALT", key, true);
	key.value.integer = KEY_RIGHT_SUPER;
	mb_add_var(bas, &context, "KEY_RIGHT_SUPER", key, true);
	key.value.integer = KEY_KB_MENU;
	mb_add_var(bas, &context, "KEY_KB_MENU", key, true);
	key.value.integer = KEY_KP_0;
	mb_add_var(bas, &context, "KEY_KP_0", key, true);
	key.value.integer = KEY_KP_1;
	mb_add_var(bas, &context, "KEY_KP_1", key, true);
	key.value.integer = KEY_KP_2;
	mb_add_var(bas, &context, "KEY_KP_2", key, true);
	key.value.integer = KEY_KP_3;
	mb_add_var(bas, &context, "KEY_KP_3", key, true);
	key.value.integer = KEY_KP_4;
	mb_add_var(bas, &context, "KEY_KP_4", key, true);
	key.value.integer = KEY_KP_5;
	mb_add_var(bas, &context, "KEY_KP_5", key, true);
	key.value.integer = KEY_KP_6;
	mb_add_var(bas, &context, "KEY_KP_6", key, true);
	key.value.integer = KEY_KP_7;
	mb_add_var(bas, &context, "KEY_KP_7", key, true);
	key.value.integer = KEY_KP_8;
	mb_add_var(bas, &context, "KEY_KP_8", key, true);
	key.value.integer = KEY_KP_9;
	mb_add_var(bas, &context, "KEY_KP_9", key, true);
	key.value.integer = KEY_KP_DECIMAL;
	mb_add_var(bas, &context, "KEY_KP_DECIMAL", key, true);
	key.value.integer = KEY_KP_DIVIDE;
	mb_add_var(bas, &context, "KEY_KP_DIVIDE", key, true);
	key.value.integer = KEY_KP_MULTIPLY;
	mb_add_var(bas, &context, "KEY_KP_MULTIPLY", key, true);
	key.value.integer = KEY_KP_SUBTRACT;
	mb_add_var(bas, &context, "KEY_KP_SUBTRACT", key, true);
	key.value.integer = KEY_KP_ADD;
	mb_add_var(bas, &context, "KEY_KP_ADD", key, true);
	key.value.integer = KEY_KP_ENTER;
	mb_add_var(bas, &context, "KEY_KP_ENTER", key, true);
	key.value.integer = KEY_KP_EQUAL;
	mb_add_var(bas, &context, "KEY_KP_EQUAL", key, true);

	//Mouse enum
	key.value.integer = MOUSE_BUTTON_LEFT;
	mb_add_var(bas, &context, "MOUSE_BUTTON_LEFT", key, true);
	key.value.integer = MOUSE_BUTTON_RIGHT;
	mb_add_var(bas, &context, "MOUSE_BUTTON_RIGHT", key, true);
	key.value.integer = MOUSE_BUTTON_MIDDLE;
	mb_add_var(bas, &context, "MOUSE_BUTTON_MIDDLE", key, true);
	key.value.integer = MOUSE_BUTTON_SIDE;
	mb_add_var(bas, &context, "MOUSE_BUTTON_SIDE", key, true);
	key.value.integer = MOUSE_BUTTON_EXTRA;
	mb_add_var(bas, &context, "MOUSE_BUTTON_EXTRA", key, true);
	key.value.integer = MOUSE_BUTTON_FORWARD;
	mb_add_var(bas, &context, "MOUSE_BUTTON_FORWARD", key, true);
	key.value.integer = MOUSE_BUTTON_BACK;
	mb_add_var(bas, &context, "MOUSE_BUTTON_BACK", key, true);
 
	//Gamepad enum       
	key.value.integer = GAMEPAD_BUTTON_UNKNOWN;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_UNKNOWN", key, true);   
	key.value.integer = GAMEPAD_BUTTON_LEFT_FACE_UP;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_FACE_UP", key, true);    
	key.value.integer = GAMEPAD_BUTTON_LEFT_FACE_RIGHT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_FACE_RIGHT", key, true);      
	key.value.integer = GAMEPAD_BUTTON_LEFT_FACE_DOWN;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_FACE_DOWN", key, true);      
	key.value.integer = GAMEPAD_BUTTON_LEFT_FACE_LEFT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_FACE_LEFT", key, true);       
	key.value.integer = GAMEPAD_BUTTON_RIGHT_FACE_UP;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_FACE_UP", key, true);     
	key.value.integer = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_FACE_RIGHT", key, true);     
	key.value.integer = GAMEPAD_BUTTON_RIGHT_FACE_DOWN;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_FACE_DOWN", key, true);     
	key.value.integer = GAMEPAD_BUTTON_RIGHT_FACE_LEFT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_FACE_LEFT", key, true);      
	key.value.integer = GAMEPAD_BUTTON_LEFT_TRIGGER_1;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_TRIGGER_1", key, true);       
	key.value.integer = GAMEPAD_BUTTON_LEFT_TRIGGER_2;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_TRIGGER_2", key, true);    
	key.value.integer = GAMEPAD_BUTTON_RIGHT_TRIGGER_1;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_TRIGGER_1", key, true);      
	key.value.integer = GAMEPAD_BUTTON_RIGHT_TRIGGER_2;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_TRIGGER_2", key, true);          
	key.value.integer = GAMEPAD_BUTTON_MIDDLE_LEFT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_MIDDLE_LEFT", key, true);               
	key.value.integer = GAMEPAD_BUTTON_MIDDLE;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_MIDDLE", key, true);        
	key.value.integer = GAMEPAD_BUTTON_MIDDLE_RIGHT;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_MIDDLE_RIGHT", key, true);          
	key.value.integer = GAMEPAD_BUTTON_LEFT_THUMB;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_LEFT_THUMB", key, true);         
	key.value.integer = GAMEPAD_BUTTON_RIGHT_THUMB;
	mb_add_var(bas, &context, "GAMEPAD_BUTTON_RIGHT_THUMB", key, true);

    
	key.value.integer = GAMEPAD_AXIS_LEFT_X;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_LEFT_X", key, true);
	key.value.integer = GAMEPAD_AXIS_LEFT_Y;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_LEFT_Y", key, true);
	key.value.integer = GAMEPAD_AXIS_RIGHT_X;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_RIGHT_X", key, true);
	key.value.integer = GAMEPAD_AXIS_RIGHT_Y;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_RIGHT_Y", key, true);
	key.value.integer = GAMEPAD_AXIS_LEFT_TRIGGER;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_LEFT_TRIGGER", key, true);
	key.value.integer = GAMEPAD_AXIS_RIGHT_TRIGGER;
	mb_add_var(bas, &context, "GAMEPAD_AXIS_RIGHT_TRIGGER", key, true);
	
	//Draw
	mb_register_func(bas, "CLS", 		cls);

	mb_register_func(bas, "PIXEL", 		drawPixel);
	mb_register_func(bas, "LINE", 		drawLine);
	mb_register_func(bas, "CIRCLE", 	drawCircle);
	mb_register_func(bas, "RING", 		drawRing);
	mb_register_func(bas, "ELLIPSE", 	drawEllipse);
	mb_register_func(bas, "TRIANGLE",	drawTriangle);
	mb_register_func(bas, "RECT", 		drawRect);
	mb_register_func(bas, "RECTROUND",  drawRectRound);
	mb_register_func(bas, "POLY", 		drawPoly);

	mb_register_func(bas, "TEXT", 		drawText);

	mb_register_func(bas, "SPRITE", 	drawSprite);
	mb_register_func(bas, "META", 		drawMetaSprite);

	//Mouse inputs
	mb_register_func(bas, "MOUSEWHEEL", 	mouseWheel); 
	mb_register_func(bas, "MOUSEPRESSED", 	mousePressed); 
	mb_register_func(bas, "MOUSEDOWN", 		mouseDown); 
	mb_register_func(bas, "MOUSERELEASED", 	mouseReleased); 
	mb_register_func(bas, "MOUSEUP", 		mouseUp); 
	mb_register_func(bas, "MOUSESETPOS", 	setMousePosition); 

	//Keys inputs
	mb_register_func(bas, "KEYPRESSED", 	keyPressed);
	mb_register_func(bas, "KEYDOWN", 		keyDown);
	mb_register_func(bas, "KEYRELEASED", 	keyReleased);
	mb_register_func(bas, "KEYUP", 			keyUp);
	mb_register_func(bas, "KEYGET", 		getKey);
	mb_register_func(bas, "KEYCHAR", 		getKeyChar);
	
	//Joy
	mb_register_func(bas, "JOYISAVIABLE",  	isGamepadAvailable);
	mb_register_func(bas, "JOYNAME", 		getGamepadName);
	mb_register_func(bas, "JOYPRESSED",		buttonPressed);
	mb_register_func(bas, "JOYDOWN",		buttonDown);
	mb_register_func(bas, "JOYRELEASED", 	buttonReleased);
	mb_register_func(bas, "JOYUP",			buttonUp);
	mb_register_func(bas, "JOYGET",			getButton);

	mb_register_func(bas, "JOYAXISCOUNT", 	getAxisCount);
	mb_register_func(bas, "JOYAXISVALUE", 	axisValue);

	//Text tools
	mb_register_func(bas, "FORMATTEXT", 	formatText);
	mb_register_func(bas, "FONTSPACE",  	setFontSpacing);
	mb_register_func(bas, "TEXTSIZE",   	measureText);

	//Sound
	mb_register_func(bas, "CHPRESET",  	  setPreset); 
	mb_register_func(bas, "CHSET",  	  setSequence); 
	mb_register_func(bas, "CHON",   	  playNote);
	mb_register_func(bas, "CHOFF",  	  stopNote);
	mb_register_func(bas, "CHPLAY", 	  musicPlay);
	mb_register_func(bas, "CHSTOP", 	  musicStop);
	mb_register_func(bas, "CHPOS",  	  getMusicPosition);
	mb_register_func(bas, "CHSIZE", 	  getMusicSize);
	mb_register_func(bas, "CHFRAME",	  getFrameAverage);
	mb_register_func(bas, "CHGETNOTE",	  getNoteName);
	mb_register_func(bas, "CHGETNOTENUM", getNoteNum);

	//Memory
	mb_register_func(bas, "PEEK", 		peek);
	mb_register_func(bas, "POKE", 		poke);
	mb_register_func(bas, "SAVEDATA", 	savedata);
	mb_register_func(bas, "LOADDATA", 	loaddata);

	mb_register_func(bas, "QUIT", 		quit);

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

void MBManager::Reset()
{
	mb_reset(&bas, true, true);
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
		case 1:
			DrawCircleLines(x, y, r, basicEngineRef->spriteManager->GetColor(col));
			break;
		default:
			DrawRing((Vector2){x, y}, r - style, r, 0, 360, 32, basicEngineRef->spriteManager->GetColor(col));
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
	int segments = 0; //Segment auto
	int col = 0;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &w));
		mb_check(mb_pop_int(s, l, &h));
		mb_check(mb_pop_int(s, l, &roundness));
		mb_check(mb_pop_int(s, l, &lineThick));
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));
 
	float fRound = (float)(roundness) * 0.1f;
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

//Keyboard
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

//Gamepad
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

//Mouse
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

	basicEngineRef->SetVirtualMouse(x,y);
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

    basicEngineRef->audioManager->SetSequence(id, arg);
	return result;
}

int MBManager::setPreset(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	int channel;
	int preset;


	mb_check(mb_attempt_open_bracket(s, l));

		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &preset));
	
	mb_check(mb_attempt_close_bracket(s, l));

    basicEngineRef->audioManager->SetChannelPreset(channel, preset);
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

    basicEngineRef->audioManager->PlayNote(channel, note, volume);

	return result;
}

int MBManager::stopNote(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int channel;

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    basicEngineRef->audioManager->StopNote(channel);

	return result;
}

int MBManager::musicPlay(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int channel;
	char* arg = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &channel));
		if(mb_has_arg(s, l)) 
		{
			mb_check(mb_pop_string(s, l, &arg));
		}
	mb_check(mb_attempt_close_bracket(s, l));

	if (arg == 0)
    	basicEngineRef->audioManager->ChannelPlay(channel);
	else
    	basicEngineRef->audioManager->ChannelPlay(channel, arg);
	
	return result;
}

int MBManager::musicStop(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

    int channel;;

	mb_check(mb_attempt_open_bracket(s, l));	
	if(mb_has_arg(s, l)) 
	{
		mb_check(mb_pop_int(s, l, &channel));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    basicEngineRef->audioManager->ChannelStop(channel);

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

    ret.value.integer = basicEngineRef->audioManager->GetChannelPosition(channel);
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

    ret.value.integer = basicEngineRef->audioManager->GetChannelSize(channel);
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::getFrameAverage(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	int channel;
	int frame;

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &channel));
		mb_check(mb_pop_int(s, l, &frame));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = basicEngineRef->audioManager->GetSynth()->GetFrameAverage(channel, frame);
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::getNoteName(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_string(ret, 0);

	int channel;

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &channel));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.string = basicEngineRef->audioManager->GetNoteName(channel);
    mb_check(mb_push_value(s, l, ret));
	return result;
}

int MBManager::getNoteNum(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;	
	mb_assert(s && l);

    mb_value_t ret;
    mb_make_int(ret, 0);

	int channel;

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &channel));
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = basicEngineRef->audioManager->GetNoteNum(channel);
    mb_check(mb_push_value(s, l, ret));
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
	int f = 0;

	mb_check(mb_attempt_open_bracket(s, l));
		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &id));
			mb_check(mb_pop_int(s, l, &x));
			mb_check(mb_pop_int(s, l, &y));
		}
		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &f));
		}
	mb_check(mb_attempt_close_bracket(s, l));

	basicEngineRef->spriteManager->DrawMetaSprite(id, x, y, f);
	
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

int MBManager::savedata(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	int slot = 0;
	int data = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &slot));
		mb_check(mb_pop_int(s, l, &data));
	mb_check(mb_attempt_close_bracket(s, l));

	if (slot < 0) slot = 0;
	if (slot > 31) slot = 31;

	unsigned char byte0 = (data >> 0) & 0xFF;
	unsigned char byte1 = (data >> 8) & 0xFF;
	unsigned char byte2 = (data >> 16) & 0xFF;
	unsigned char byte3 = (data >> 24) & 0xFF;

	if (slot < 16)
	{
		int dir = 3552 + (slot * 2);
		basicEngineRef->Poke(dir    , byte0);
		basicEngineRef->Poke(dir + 1, byte1);
	}
	else
	{
		int dir = 3584 + ((slot - 16) * 4);
		basicEngineRef->Poke(dir    , byte0);
		basicEngineRef->Poke(dir + 1, byte1);
		basicEngineRef->Poke(dir + 2, byte2);
		basicEngineRef->Poke(dir + 3, byte3);
	}

	return result;
}

int MBManager::loaddata(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	int slot = 0;
	int data = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_int(s, l, &slot));
	mb_check(mb_attempt_close_bracket(s, l));

	if (slot < 0) slot = 0;
	if (slot > 31) slot = 31;
	int value = 0;

	if (slot < 16)
	{
		int dir = 3552 + (slot * 2);
		unsigned char byte0 = basicEngineRef->Peek(dir);
		unsigned char byte1 = basicEngineRef->Peek(dir + 1);
		value = (byte0) | (byte1 << 8);
	}
	else
	{
		int dir = 3584 + ((slot - 16) * 4);
		unsigned char byte0 = basicEngineRef->Peek(dir);
		unsigned char byte1 = basicEngineRef->Peek(dir + 1);
		unsigned char byte2 = basicEngineRef->Peek(dir + 2);
		unsigned char byte3 = basicEngineRef->Peek(dir + 3);
		value = (byte0) | (byte1 << 8) | (byte2 << 16) | (byte3 << 24);
	}

	mb_check(mb_push_int(s, l, value));

	return result;
}

//Tools
int MBManager::quit(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));
	
	basicEngineRef->bios->ShouldClose = true;
	
	return result;
}

int MBManager::formatText(struct mb_interpreter_t* s, void** l){
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	char* arg = nullptr;
	mb_value_t val;
	mb_make_nil(val);
	std::vector<mb_value_t> vals;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_check(mb_attempt_open_bracket(s, l));
		mb_check(mb_pop_string(s, l, &arg));	
		while(mb_has_arg(s, l)) 
		{
			mb_make_nil(val);
			mb_check(mb_pop_value(s, l, &val));
			vals.push_back(val);
		}
	mb_check(mb_attempt_close_bracket(s, l));

	std::array<mb_value_t, 8> args{ MB_DT_NIL };
	for (size_t i = 0; i < vals.size() && i < 8; i++) {
		args[i] = vals[i];
	}

	if (args[0].type == MB_DT_INT) 
	{
		ret.value.string = (char *)TextFormat(arg, args[0].value.integer, args[1].value.integer, args[2].value.integer, args[3].value.integer,
												   args[4].value.integer, args[5].value.integer, args[6].value.integer, args[7].value.integer);
	}
	else
	{
		ret.value.string = (char *)TextFormat(arg, args[0].value.float_point, args[1].value.float_point, args[2].value.float_point, args[3].value.float_point,
												   args[4].value.float_point, args[5].value.float_point, args[6].value.float_point, args[7].value.float_point);
	}

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

//..
int MBManager::my_print(struct mb_interpreter_t* s, const char* fmt, ...) {
	char buf[256];
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
		Tools::console->AddLog(ptr); 

	if(ptr != buf)
		free(ptr);

	return result;
}

int MBManager::my_input(struct mb_interpreter_t* s, const char* pmt, char* buf, int n) {
	int result = 0;
	mb_unrefvar(s);

	const char* input_str = Tools::console->InputBuf;
	int input_len = strlen(input_str);

	if (input_len >= n) {
		fprintf(stderr, "Error: input too long.\n");
		return 0;
	}

	strcpy(buf, input_str);
	result = input_len;
	if (buf[result - 1] == '\n') {
		buf[result - 1] = '\0';
	}

	return result;
}
