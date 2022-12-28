// Raylib Template C++17 64bits
//
// MIT License - Copyright (C) 2021 Fredy Rogez
// This file is subject to the terms and conditions defined in
// file "LICENSE", which is part of this source code package.

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <fstream>
#include <sstream>
#include <assert.h>
#include "inipp.h"

#include "my_basic.h"
//#include "rayfun.h"


struct mb_interpreter_t* bas = NULL;
static void* context = NULL;

int size = 1;

static Color stringToColor(char* col)
{
    int num = std::stoi(col, 0, 16);
    unsigned char r = num / 0x10000;
    unsigned char g = (num / 0x100) % 0x100;
    unsigned char b = num % 0x100;

    return Color{r,g,b,255};
    
}

static int drawtext(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	char* arg = 0;
	int x = 0;
	int y = 0;
	int size = 0;
	char* col = 0;
	int value = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &arg));
		mb_check(mb_pop_int(s, l, &x));
		mb_check(mb_pop_int(s, l, &y));
		mb_check(mb_pop_int(s, l, &size));
		mb_check(mb_pop_string(s, l, &col));
		mb_check(mb_pop_int(s, l, &value));
	}

	mb_check(mb_attempt_close_bracket(s, l));

    DrawText(TextFormat(arg, value) ,x, y, size, stringToColor(col));

	return result;
}


static int cls(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	char* col;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_string(s, l, &col));
	}

	mb_check(mb_attempt_close_bracket(s, l));

    ClearBackground(stringToColor(col));

	return result;
}

void ReadIni()
{
    inipp::Ini<char> ini;
	std::ifstream is("assets/config.ini");
	ini.parse(is);

	int crt_enabled;

	inipp::get_value(ini.sections["window"], "size", size);
	inipp::get_value(ini.sections["crt"], "enabled", crt_enabled);
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
    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "Raylib Template");

    
    SetTargetFPS(60);

    mb_value_t routine;
    mb_value_t args[1];
     mb_value_t ret;
    mb_make_string(args[0], "-- c --");
    mb_make_nil(ret);

    mb_value_t mouseX;
    mb_make_int(mouseX, GetMousePosition().x);
    mb_get_value_by_name(bas, &context, "MOUSEX", &mouseX);
    mb_value_t drawRoutine;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        mouseX.value.integer = (int)GetMousePosition().x;
        //mb_get_value_by_name(bas, &context, "MOUSEX", &mouseX);
        mb_set_var_value(bas, &context, mouseX);

        printf("%i ", mouseX.value.integer);

	    DrawRectangle(0,0,size,200, (Color){ 0, 0, 0, 220});
        if (GuiButton(Rectangle{5,5,100,20},  "Open")) { OpenBas(); }
        /*
        if (GuiButton(Rectangle{5,35,100,20},  "Hola")) 
        {
            mb_get_routine(bas, &context, "HOLA", &routine);
            mb_eval_routine(bas, &context, routine, args, 1, &ret);
        }*/
            mb_get_routine(bas, &context, "DRAW", &drawRoutine);
            mb_eval_routine(bas, &context, drawRoutine, args, 0, NULL);
/*
            mb_value_t routine;
            mb_value_t args[1];
            mb_value_t ret;
            mb_make_string(args[0], "n");
            mb_make_nil(ret);

            mb_get_routine(bas, &context, "draw", &routine);
            mb_eval_routine(bas, &context, routine, args, 1, &ret);*/

            DrawFPS(550, 15);

        EndDrawing();
    }

    mb_close(&bas);
	mb_dispose();

    CloseWindow();   

    return 0;
}


