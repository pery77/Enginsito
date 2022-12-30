#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <sstream>
#include <assert.h>

#include "ini_reader.h"

#include "my_basic.h"
#include "tools.h"
#include "mb_manager.h"


struct mb_interpreter_t* bas = NULL;
static void* context = NULL;

static int drawtext(struct mb_interpreter_t* s, void** l)
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
static int textformat(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	char* arg = 0;
	int value = 0;

    mb_value_t ret;
    mb_make_string(ret, 0);

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &value));
	}

	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.string = (char *)TextFormat(arg, value);
    mb_check(mb_push_value(s, l, ret));
	return result;
}
static int cls(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	int col;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &col));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ClearBackground(tools::getColor(col));

	return result;
}

static int getkeydown(struct mb_interpreter_t* s, void** l)
{
    mb_value_t arg[1];
    mb_make_nil(arg[0]);
    mb_value_t ret;
    mb_make_int(ret, 0);
    int k = 0;

    mb_value_t routine;

    int result = MB_FUNC_OK;
	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
    if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &k));
	}
	mb_check(mb_attempt_close_bracket(s, l));

    ret.value.integer = IsKeyDown(k);

    mb_check(mb_push_value(s, l, ret));

	return result;
}

static int mousex(struct mb_interpreter_t* s, void** l)
{
    mb_value_t x;
    mb_make_int(x, 0);

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    x.value.integer = (int)GetMousePosition().x;
    mb_check(mb_push_value(s, l, x));

	return MB_FUNC_OK;
}
static int mousey(struct mb_interpreter_t* s, void** l)
{
    mb_value_t y;
    mb_make_int(y, 0);

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));

    y.value.integer = (int)GetMousePosition().y;

    mb_check(mb_push_value(s, l, y));

	return MB_FUNC_OK;
}



void OpenBas()
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
	mb_reg_fun(bas, cls);
	mb_reg_fun(bas, getkeydown);
	mb_reg_fun(bas, mousex);
	mb_reg_fun(bas, mousey);
	mb_reg_fun(bas, textformat);

    mb_load_file(bas, f);
    
	run = mb_run(bas, true);
    e = mb_get_last_error(bas, &f, &pos, &row, &col);

    if(run > 0)
    {
        printf("Run code: %i, \n",run);
        printf("ERROR Code: %i, pos = %i, row  = %i col = %i \n",e , pos, row, col);
    }
}

int main(int argc, char *argv[])
{
    iniReader* config = new iniReader();

    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "peryEngine");
    SetTargetFPS(60);

    mb_value_t arg[1];
    mb_make_nil(arg[0]);

    mb_value_t initRoutine;
    mb_value_t tickRoutine;
    mb_value_t drawRoutine;
    mb_value_t endRoutine;

    mb_value_t valueMouseX;
    valueMouseX.type=MB_DT_INT;
    mb_value_t valueMouseY;
    valueMouseY.type=MB_DT_INT;


    bool running = false;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Update
        if (running)
        {
            valueMouseX.value.integer = (int)GetMousePosition().x;
            mb_add_var(bas,&context, "MOUSEX",valueMouseX, true);

            valueMouseY.value.integer = (int)GetMousePosition().y;
            mb_add_var(bas,&context, "MOUSEY",valueMouseY, true);

            mb_get_routine(bas, &context, "TICK", &tickRoutine);
            mb_eval_routine(bas, &context, tickRoutine, arg, 0, NULL);
        }

        // Draw
        BeginDrawing();
        if (!running)
        {
            ClearBackground(BLACK);
            DrawText("Press F5",0,0,20,RED);
        }

        if (running)
        {
            mb_get_routine(bas, &context, "DRAW", &drawRoutine);
            mb_eval_routine(bas, &context, drawRoutine, arg, 0, NULL);
        }

        if (IsKeyReleased(KEY_F5)) 
        { 
            if (running)
            {
                mb_get_routine(bas, &context, "END", &endRoutine);
                mb_eval_routine(bas, &context, endRoutine, arg, 0, NULL);
                running = false;
            }
            OpenBas(); 
            if (!running)
            {
                running = true;
                mb_get_routine(bas, &context, "INIT", &initRoutine);
                mb_eval_routine(bas, &context, initRoutine, arg, 0, NULL);
            }

        }
        if (IsKeyReleased(KEY_F6)) 
        { 
            if (running)
            {
                mb_get_routine(bas, &context, "END", &endRoutine);
                mb_eval_routine(bas, &context, endRoutine, arg, 0, NULL);
                running = false;
                mb_close(&bas);
	            mb_dispose();
            }
        }

        DrawFPS(550, 15);

        EndDrawing();
    }

    mb_close(&bas);
	mb_dispose();

    CloseWindow();   

    delete config;

    return 0;
}


