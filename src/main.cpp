#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <sstream>
#include <assert.h>

#include "ini_reader.h"

#include "my_basic.h"
#include "tools.h"
#include "mb_manager.h"

/*
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

*/

int main(int argc, char *argv[])
{
    iniReader* config = new iniReader();
    mbManager* basic = new mbManager();

    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "peryEngine");
    SetTargetFPS(60);
/*
    mb_value_t valueMouseX;
    valueMouseX.type=MB_DT_INT;
    mb_value_t valueMouseY;
    valueMouseY.type=MB_DT_INT;
*/

    bool running = false;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Update
        if (running)
        {
            //valueMouseX.value.integer = (int)GetMousePosition().x;
            //mb_add_var(bas,&context, "MOUSEX",valueMouseX, true);

            //valueMouseY.value.integer = (int)GetMousePosition().y;
            //mb_add_var(bas,&context, "MOUSEY",valueMouseY, true);

            basic->tick();
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
            basic->draw();
        }

        if (IsKeyReleased(KEY_F5)) 
        { 
            if (running)
            {
                basic->end();
                running = false;
            }
            basic->OpenBas(); 
            if (!running)
            {
                basic->init();
                running = true;
            }

        }
        if (IsKeyReleased(KEY_F6)) 
        { 
            if (running)
            {
                basic->end();
                running = false;
                basic->CloseBas();
            }
        }

        DrawFPS(550, 15);

        EndDrawing();
    }

    basic->CloseBas();
    CloseWindow();   

    delete config;

    return 0;
}


