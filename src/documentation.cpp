#include "documentation.h"
#include "imgui/imgui.h"
#include "tools.h"


#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "TextEditor.h"

//Lazy guy toys.
struct KeyData {
    std::string name;
    int value;
};
char * removeSpacesFromStr(char *string)
{
    int non_space_count = 0;
 
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ' ')
        {
            string[non_space_count] = string[i];
            non_space_count++;
        }    
    }
    
    string[non_space_count] = '\0';
    return string;
}

void GenerateKeysCodeFromEnumBecauseImTooLazy()
{
    //Copypasted from raylib.h
    std::string input = R"|(
    KEY_APOSTROPHE      = 39
    KEY_COMMA           = 44
    KEY_MINUS           = 45
    KEY_PERIOD          = 46
    KEY_SLASH           = 47
    KEY_ZERO            = 48
    KEY_ONE             = 49
    KEY_TWO             = 50
    KEY_THREE           = 51
    KEY_FOUR            = 52
    KEY_FIVE            = 53
    KEY_SIX             = 54
    KEY_SEVEN           = 55
    KEY_EIGHT           = 56
    KEY_NINE            = 57
    KEY_SEMICOLON       = 59
    KEY_EQUAL           = 61
    KEY_A               = 65
    KEY_B               = 66
    KEY_C               = 67
    KEY_D               = 68
    KEY_E               = 69
    KEY_F               = 70
    KEY_G               = 71
    KEY_H               = 72
    KEY_I               = 73
    KEY_J               = 74
    KEY_K               = 75
    KEY_L               = 76
    KEY_M               = 77
    KEY_N               = 78
    KEY_O               = 79
    KEY_P               = 80
    KEY_Q               = 81
    KEY_R               = 82
    KEY_S               = 83
    KEY_T               = 84
    KEY_U               = 85
    KEY_V               = 86
    KEY_W               = 87
    KEY_X               = 88
    KEY_Y               = 89
    KEY_Z               = 90
    KEY_LEFT_BRACKET    = 91
    KEY_BACKSLASH       = 92
    KEY_RIGHT_BRACKET   = 93
    KEY_GRAVE           = 96
    KEY_SPACE           = 32
    KEY_ESCAPE          = 256 
    KEY_ENTER           = 257
    KEY_TAB             = 258 
    KEY_BACKSPACE       = 259
    KEY_INSERT          = 260 
    KEY_DELETE          = 261 
    KEY_RIGHT           = 262
    KEY_LEFT            = 263
    KEY_DOWN            = 264
    KEY_UP              = 265
    KEY_PAGE_UP         = 266 
    KEY_PAGE_DOWN       = 267 
    KEY_HOME            = 268 
    KEY_END             = 269 
    KEY_CAPS_LOCK       = 280
    KEY_SCROLL_LOCK     = 281
    KEY_NUM_LOCK        = 282 
    KEY_PRINT_SCREEN    = 283
    KEY_PAUSE           = 284 
    KEY_F1              = 290 
    KEY_F2              = 291 
    KEY_F3              = 292 
    KEY_F4              = 293 
    KEY_F5              = 294 
    KEY_F6              = 295 
    KEY_F7              = 296 
    KEY_F8              = 297
    KEY_F9              = 298
    KEY_F10             = 299
    KEY_F11             = 300
    KEY_F12             = 301
    KEY_LEFT_SHIFT      = 340
    KEY_LEFT_CONTROL    = 341
    KEY_LEFT_ALT        = 342
    KEY_LEFT_SUPER      = 343
    KEY_RIGHT_SHIFT     = 344
    KEY_RIGHT_CONTROL   = 345
    KEY_RIGHT_ALT       = 346
    KEY_RIGHT_SUPER     = 347
    KEY_KB_MENU         = 348
    KEY_KP_0            = 320
    KEY_KP_1            = 321
    KEY_KP_2            = 322
    KEY_KP_3            = 323
    KEY_KP_4            = 324
    KEY_KP_5            = 325
    KEY_KP_6            = 326
    KEY_KP_7            = 327
    KEY_KP_8            = 328
    KEY_KP_9            = 329
    KEY_KP_DECIMAL      = 330
    KEY_KP_DIVIDE       = 331
    KEY_KP_MULTIPLY     = 332
    KEY_KP_SUBTRACT     = 333
    KEY_KP_ADD          = 334
    KEY_KP_ENTER        = 335
    KEY_KP_EQUAL        = 336
)|";
    std::vector<KeyData> keyData; 
    std::istringstream iss(input);
    std::string line;
    while (std::getline(iss, line)) {
        KeyData data;
        if (sscanf(line.c_str(), "%*s = %d", &data.value) == 1) {
            size_t equal_sign_pos = line.find('=');
            data.name = line.substr(0, equal_sign_pos);
            data.name = removeSpacesFromStr((char *)data.name.c_str());
            keyData.push_back(data);
        }
    }
    //Copy pasted from console to mb_manager.cpp
    for (const KeyData& data : keyData) {
    //Copy pasted from console to mb_manager.cpp
        std::cout << "key.value.integer = " << data.name << ";\n";
        std::cout << "mb_add_var(bas, &context, \"" << data.name << "\", key, true);\n";
    //And this for text editor keywords2
        std::cout << "\"" << data.name << "\",";
    }
}

TextEditor codeEditor;
const char* lastName = "";
Documentation::Documentation()
{
    //GenerateKeysCodeFromEnumBecauseImTooLazy();
    codeEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Basic());
    codeEditor.SetTabSize(1);
    codeEditor.SetReadOnly(true);
}

Documentation::~Documentation()
{
}

void keyword(const char* name, const char* args, const char* description, const char* example)
{
    if(ImGui::Button(TextFormat("%s(%s)",name, args)))
    {
        codeEditor.SetText(TextFormat("'%s\n'Example: \n%s", description, example));
        codeEditor.Deselect();
    }
}

void Documentation::Draw(bool* p_open)
{

    if (ImGui::CollapsingHeader("Main"))
    {
        ImGui::SeparatorText(" ");
    }

    if (ImGui::CollapsingHeader("Console"))
    {
        ImGui::SeparatorText(" ");
    }

    if (ImGui::CollapsingHeader("Editor"))
    {
        ImGui::SeparatorText("Keys");
            ImGui::Text(R"|(
    ESC ->  Pause/Exit Game 
    F1  ->  Show/Hide Editor
    F2  ->  Adjust windows size and center
    F11 ->  Toggle Fullscreen
    F12 ->  Take Screenshoot
        )|"
        );
    }
    
    if (ImGui::CollapsingHeader("Code"))
    {
        ImGui::BeginGroup();
        ImGui::Text("                              ");
        if (ImGui::TreeNode("Basic"))
        {
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Enginsito"))
        {
            if (ImGui::TreeNode("Inputs"))
            {
                if (ImGui::TreeNode("Keyboard"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Mouse"))
                {
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Gamepad"))
                {
                    ImGui::TreePop();
                }                
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Draw"))
            {
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Sound"))
            {
                ImGui::TreePop();
            } 
            if (ImGui::TreeNode("Memory"))
            {
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tools"))
            {
                keyword("formatText", "text, value", "Return formatted text %i for integer or %f for float", 
                "formatText(\"Score: %06i\",score)");
                keyword("fontSpace", "space", "Set font separation in pixels", 
                "fontSpace(16)");
                keyword("textSize", "text, textSize", "Return the size in pixels of the text at current text,\n'textSize is from 1 to 4", 
                "textSize(\"Hello\", 1)");
                keyword("quit", "","Close Enginsito", "quit()");
                ImGui::TreePop();
            }      
            ImGui::TreePop();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        
        codeEditor.Render("keyword");
    }


    if (ImGui::CollapsingHeader("MML"))
    {
        if (ImGui::TreeNode("MML Info"))
        {

            ImGui::TextWrapped(R"|(Music Macro Language (MML) is a text-based programming language 
used to create music for various types of computer and video game systems, 
as well as some early personal computers and synthesizers.
MML allows composers and programmers to define musical sequences and melodies
using a series of text commands and notations.
        )|");
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("MML Code"))
        {
            ImGui::Text(R"|(
    A - G       note on         C(C4) D4.(D4+8) C12C12C12(triplets)
    + or #      sharp	
    -           flat	
    =           natural	
    R           rest            R1 (whole rest)
    O           octave          O0 ... O8 (O4 default)
    > <         octave up/down	
    L           length          L4 default
    Q           note off ratio  n/8 (Q8 default)
    ^           tie             C4^16
    &           no note off	
    T           tempo           T120 default
    V           volume          V0 ... V127(max) V+10 V-10
    KJ KI       transpose(maJor/mInor)	KJg(G major)
    [ ]         loop            [...]4 (repeat 4 times)
    :           skip on the last loop	
    @           program change
        )|");
            ImGui::TreePop();
        }
    }
  
    if (ImGui::CollapsingHeader("Key Codes"))
    {
        if (ImGui::TreeNode("Alphanumeric keys##kc"))
        {          
            ImGui::Text(R"|(
    KEY_APOSTROPHE      = 39 ->    Key: '
    KEY_COMMA           = 44 ->    Key: ,
    KEY_MINUS           = 45 ->    Key: -
    KEY_PERIOD          = 46 ->    Key: .
    KEY_SLASH           = 47 ->    Key: /
    KEY_ZERO            = 48 ->    Key: 0
    KEY_ONE             = 49 ->    Key: 1
    KEY_TWO             = 50 ->    Key: 2
    KEY_THREE           = 51 ->    Key: 3
    KEY_FOUR            = 52 ->    Key: 4
    KEY_FIVE            = 53 ->    Key: 5
    KEY_SIX             = 54 ->    Key: 6
    KEY_SEVEN           = 55 ->    Key: 7
    KEY_EIGHT           = 56 ->    Key: 8
    KEY_NINE            = 57 ->    Key: 9
    KEY_SEMICOLON       = 59 ->    Key: ;
    KEY_EQUAL           = 61 ->    Key: =
    KEY_A               = 65 ->    Key: A | a
    KEY_B               = 66 ->    Key: B | b
    KEY_C               = 67 ->    Key: C | c
    KEY_D               = 68 ->    Key: D | d
    KEY_E               = 69 ->    Key: E | e
    KEY_F               = 70 ->    Key: F | f
    KEY_G               = 71 ->    Key: G | g
    KEY_H               = 72 ->    Key: H | h
    KEY_I               = 73 ->    Key: I | i
    KEY_J               = 74 ->    Key: J | j
    KEY_K               = 75 ->    Key: K | k
    KEY_L               = 76 ->    Key: L | l
    KEY_M               = 77 ->    Key: M | m
    KEY_N               = 78 ->    Key: N | n
    KEY_O               = 79 ->    Key: O | o
    KEY_P               = 80 ->    Key: P | p
    KEY_Q               = 81 ->    Key: Q | q
    KEY_R               = 82 ->    Key: R | r
    KEY_S               = 83 ->    Key: S | s
    KEY_T               = 84 ->    Key: T | t
    KEY_U               = 85 ->    Key: U | u
    KEY_V               = 86 ->    Key: V | v
    KEY_W               = 87 ->    Key: W | w
    KEY_X               = 88 ->    Key: X | x
    KEY_Y               = 89 ->    Key: Y | y
    KEY_Z               = 90 ->    Key: Z | z
    KEY_LEFT_BRACKET    = 91 ->    Key: [
    KEY_BACKSLASH       = 92 ->    Key: \
    KEY_RIGHT_BRACKET   = 93 ->    Key: ]
    KEY_GRAVE           = 96 ->    Key: `            
    )|");
        ImGui::TreePop();  
        }

        if (ImGui::TreeNode("Functions##kc"))
        {
            ImGui::Text(R"|(
    KEY_SPACE           = 32  ->   Key: Space
    KEY_ESCAPE          = 256 ->   Key: Esc
    KEY_ENTER           = 257 ->   Key: Enter
    KEY_TAB             = 258 ->   Key: Tab
    KEY_BACKSPACE       = 259 ->   Key: Backspace
    KEY_INSERT          = 260 ->   Key: Ins
    KEY_DELETE          = 261 ->   Key: Del
    KEY_RIGHT           = 262 ->   Key: Cursor right
    KEY_LEFT            = 263 ->   Key: Cursor left
    KEY_DOWN            = 264 ->   Key: Cursor down
    KEY_UP              = 265 ->   Key: Cursor up
    KEY_PAGE_UP         = 266 ->   Key: Page up
    KEY_PAGE_DOWN       = 267 ->   Key: Page down
    KEY_HOME            = 268 ->   Key: Home
    KEY_END             = 269 ->   Key: End
    KEY_CAPS_LOCK       = 280 ->   Key: Caps lock
    KEY_SCROLL_LOCK     = 281 ->   Key: Scroll down
    KEY_NUM_LOCK        = 282 ->   Key: Num lock
    KEY_PRINT_SCREEN    = 283 ->   Key: Print screen
    KEY_PAUSE           = 284 ->   Key: Pause
    KEY_F1              = 290 ->   Key: F1
    KEY_F2              = 291 ->   Key: F2
    KEY_F3              = 292 ->   Key: F3
    KEY_F4              = 293 ->   Key: F4
    KEY_F5              = 294 ->   Key: F5
    KEY_F6              = 295 ->   Key: F6
    KEY_F7              = 296 ->   Key: F7
    KEY_F8              = 297 ->   Key: F8
    KEY_F9              = 298 ->   Key: F9
    KEY_F10             = 299 ->   Key: F10
    KEY_F11             = 300 ->   Key: F11
    KEY_F12             = 301 ->   Key: F12
    KEY_LEFT_SHIFT      = 340 ->   Key: Shift left
    KEY_LEFT_CONTROL    = 341 ->   Key: Control left
    KEY_LEFT_ALT        = 342 ->   Key: Alt left
    KEY_LEFT_SUPER      = 343 ->   Key: Super left
    KEY_RIGHT_SHIFT     = 344 ->   Key: Shift right
    KEY_RIGHT_CONTROL   = 345 ->   Key: Control right
    KEY_RIGHT_ALT       = 346 ->   Key: Alt right
    KEY_RIGHT_SUPER     = 347 ->   Key: Super right
    KEY_KB_MENU         = 348 ->   Key: KB menu            
        )|"
        );
        ImGui::TreePop();
        }

        if (ImGui::TreeNode("Keypad key##kc"))
        {
            ImGui::Text(R"|(
    KEY_KP_0            = 320 ->   Key: Keypad 0
    KEY_KP_1            = 321 ->   Key: Keypad 1
    KEY_KP_2            = 322 ->   Key: Keypad 2
    KEY_KP_3            = 323 ->   Key: Keypad 3
    KEY_KP_4            = 324 ->   Key: Keypad 4
    KEY_KP_5            = 325 ->   Key: Keypad 5
    KEY_KP_6            = 326 ->   Key: Keypad 6
    KEY_KP_7            = 327 ->   Key: Keypad 7
    KEY_KP_8            = 328 ->   Key: Keypad 8
    KEY_KP_9            = 329 ->   Key: Keypad 9
    KEY_KP_DECIMAL      = 330 ->   Key: Keypad .
    KEY_KP_DIVIDE       = 331 ->   Key: Keypad /
    KEY_KP_MULTIPLY     = 332 ->   Key: Keypad *
    KEY_KP_SUBTRACT     = 333 ->   Key: Keypad -
    KEY_KP_ADD          = 334 ->   Key: Keypad +
    KEY_KP_ENTER        = 335 ->   Key: Keypad Enter
    KEY_KP_EQUAL        = 336 ->   Key: Keypad =        
        )|"
        );
        ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mouse##kc"))
        {
            ImGui::Text(R"|(
    MOUSE_BUTTON_LEFT    = 0  ->    Mouse button left
    MOUSE_BUTTON_RIGHT   = 1  ->    Mouse button right
    MOUSE_BUTTON_MIDDLE  = 2  ->    Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = 3  ->    Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = 4  ->    Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5  ->    Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK    = 6  ->    Mouse button back (advanced mouse device)
        )|"
        );
        ImGui::TreePop();
        }

        if (ImGui::TreeNode("Gamepad##kc"))
        {
            ImGui::SeparatorText("Buttons");
            ImGui::Text(R"|(
    GAMEPAD_BUTTON_UNKNOWN          = 0     -> Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP     = 1     -> Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT  = 2     -> Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN   = 3     -> Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT   = 4     -> Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP    = 5     -> Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT = 6     -> Gamepad right button right (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN  = 7     -> Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT  = 8     -> Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1   = 9     -> Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2   = 10    -> Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1  = 11    -> Gamepad top/back trigger right (one), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2  = 12    -> Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT      = 13    -> Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE           = 14    -> Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT     = 15    -> Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB       = 16    -> Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB      = 17    -> Gamepad joystick pressed button right            
        )|");
        ImGui::SeparatorText("Axis");
        ImGui::Text(R"|(
    GAMEPAD_AXIS_LEFT_X             = 0     ->  Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y             = 1     ->  Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X            = 2     ->  Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y            = 3     ->  Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER       = 4     ->  Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER      = 5     ->  Gamepad back trigger right, pressure level: [1..-1]
        )|");
        ImGui::TreePop();  
        }
    }

}