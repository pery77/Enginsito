#include "documentation.h"
#include "imgui/imgui.h"
#include "tools.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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

Documentation::Documentation()
{
    //GenerateKeysCodeFromEnumBecauseImTooLazy();
    codeEditor.SetTabSize(4);
    codeEditor.SetReadOnly(true);
}

Documentation::~Documentation()
{
}

void Documentation::Keyword(const char* name, const char* args, const char* program )
{
    if (program == "")
        program = name;

    if (ImGui::Button(TextFormat("%s", name)))
    {
        ProgramName = program;
        std::string programFile = "assets/docs/" + ProgramName + ".bas";
        std::ifstream inFile(programFile.c_str());
        if (inFile.is_open())
        {
            std::stringstream strStream;
            strStream << inFile.rdbuf();
            std::string str = strStream.str();
            codeEditor.SetText(str);
            codeEditor.Deselect();
            inFile.close();
        }
        else
        {
            Tools::console->AddLog("[ERROR] Opening: [ %s ]\n", programFile.c_str());
            ProgramName = "";
        }
    }
    ImVec4 bas =  ImGui::ColorConvertU32ToFloat4(codeEditor.GetPalette()[1]);
    ImVec4 identifier =  ImGui::ColorConvertU32ToFloat4(codeEditor.GetPalette()[6]);
    ImVec4 key =  ImGui::ColorConvertU32ToFloat4(codeEditor.GetPalette()[19]);
    ImVec4 punt =  ImGui::ColorConvertU32ToFloat4(codeEditor.GetPalette()[5]);
    if (ImGui::BeginItemTooltip())
    {
        if (args != "-")
        {
            ImGui::TextColored(key, name);
            ImGui::SameLine();        
            ImGui::TextColored(punt,"(");
            ImGui::SameLine();
            ImGui::TextColored(identifier, args);
            ImGui::SameLine();
            ImGui::TextColored(punt,")");
        }
        else
        {
            ImGui::TextColored(bas, name);
        }
        ImGui::EndTooltip();
    }
}

void Documentation::Draw(bool* p_open)
{

    if (ImGui::CollapsingHeader("Main"))
    {
        ImGui::BeginChild("MainCh",ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::TreeNode("Concept"))
        {
            ImGui::TextWrapped("Enginsito straddles the line between a tool and a game; it's essentially a delightful toy that will take you on a fun journey into the world of game development. Enginsito deliberately keeps its feature set minimal, offering a unique experience that encourages you to focus on creating small wonders. It includes everything you need to craft games reminiscent of the retro era, akin to the Atari console or the early 8-bit computers.");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Limitations"))
        {
            ImGui::Text(R"|(
* You should ensure that your game always runs at 60 fps.
* Strings are limited to 4000 characters.
* Declare functions and variables before using them.
* Use 'let' to declare variables so that Enginsito can color them in the editor.
* You can only use recursive calls by calling the function inside it once.
* Draw only inside draw() function.
* Enginsito automatically manages the saving of the project, 
this happens when you press play, when you change the project 
or when you close enginsito.
It is comfortable but dangerous, I highly recommend the use of
a version control system (VCS), learn about this. 
This is extremely important in video game development.
)|"); 
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Characteristics"))
        {
                if (ImGui::TreeNode("Screen"))
                {
                    ImGui::TextWrapped(R"|(
The screen has a resolution of 320 x 200 pixels.
The origin (0,0) is at the top left.
Runs at 60 FPS
)|"); 
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Colors"))
                {
                    ImGui::TextWrapped("Enginsito has 16 programmable colors per project.");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Memory Map"))
                {
                    ImGui::Text(R"|(
0    (0x000)- 47   (0x02F)  Palette (Color C -> R G B) (Color 0 -> 0 1 2) (Color 1 -> 3 4 5)
48   (0x030)- 2095 (0x82F)  Graphics (8 bytes per sprite, 256 sprites)
2096 (0x830)- 3375 (0xD2F)  MetaSprites   (20 bytes per meta, 64 metas)
3376 (0xD30)- 3551 (0xDDF)  Sfx (11 bytes per sfx, 16 sfx)
3552 (0xDE0)- 3583 (0xDFF)  Save data 2 bytes 0 -  15 slots
3584 (0xE00)- 3647 (0xE3F)  Save data 4 bytes 16 - 31 slots
3648 (0xE40)- 4079 (0xFEF)  Unused 
4080 (0xFF0)- 4090 (0xFFA)  CRT 4080 Blur, 4081 BlurFactor, 4082 Chromatic, 4083 Curvature, 
                  4084 Vignetting, 4085 ScanLine, 4086 VerticalLine, 4087 GrilleForce
                  4088 Noise, 4089 Fliker, 4090 Grille
4091 (0xFFC)- 4093 (0xFFD) not used at moment.
4094 (0xFFE)- 4095 (0xFFF) version (2 bytes)
)|");       
                    ImGui::TreePop();
                }
            ImGui::TreePop();
        }
        ImGui::EndChild();
    }

    if (ImGui::CollapsingHeader("Console"))
    {
        ImGui::BeginChild("ConsoleCh",ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::TreeNode("Console Info"))
        {
            ImGui::TextWrapped("When the editor is hidden, you can continue interacting with Enginsito through the command console.");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Console Commands"))
        {
            ImGui::Text(R"|(
HELP - show this. 
CLS  - clear screen.
EXIT - close program.
COLOR b f - change console colors.
RUN - run current program.
DIR - list current directory.
CD  - change directory.
CD. - directory up.
MEM  - print loaded program in memory.
LOAD - load program in memory.
EDITOR 0|1 - enable disable editor
FULLSCREEN - toggle fullscreen
)|");           
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Boot"))
        {
            ImGui::SeparatorText("Boot");
            ImGui::TextWrapped("There is a file called 'boot.txt' in the 'config' folder that will execute the commands written in it.");
            ImGui::TreePop();
        }
        ImGui::EndChild();

    }

    if (ImGui::CollapsingHeader("Editor"))
    {
        ImGui::BeginChild("EditorCh",ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::TreeNode("Keys"))
        {        
            ImGui::Text(R"|(
    ESC ->  Pause/Exit Game 
    F1  ->  Show/Hide Editor
    F2  ->  Adjust windows size and center
    F5  ->  Run
    F11 ->  Toggle Fullscreen
    F12 ->  Take Screenshoot
        )|"
        );
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Export"))
        { 
            ImGui::TextWrapped("Some modules have the option to export to facilitate the exchange of information between projects. When you do this, a file with the configuration of the current project will appear next to the executable.");
            ImGui::TreePop();
        }        
        if (ImGui::TreeNode("Import"))
        { 
            ImGui::TextWrapped("To import a previous export to the current project, just drag and drop the configuration file onto the editor.");
            ImGui::TreePop();
        }        
        ImGui::EndChild();
    }
    
    if (ImGui::CollapsingHeader("Code"))
    {
        ImGui::BeginChild("CodeWords",ImVec2(ImGui::GetContentRegionAvail().x * 0.3f, 0), true );
        if (ImGui::TreeNode("Basic"))
        {   
            if (ImGui::TreeNode("Comments"))
            {
                Keyword("rem", "-", "comments");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Operators"))
            {
                ImGui::TextWrapped("+, -, *, /, ^, =, <, >, <=, >=, <>");
                Keyword("mod", "-", "operators");
                Keyword("and", "-", "operators");
                Keyword("or",  "-", "operators");
                Keyword("not", "-", "operators");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Variables"))
            {
                Keyword("let", "-", "vars");
                Keyword("dim", "-", "vars");
                Keyword("is",  "-", "vars");
                Keyword("nil", "-", "vars");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Conditionals"))
            {
                Keyword("if",    "-", "if");
                Keyword("then",  "-", "if");
                Keyword("elif",  "-", "if");
                Keyword("else",  "-", "if");
                Keyword("endif", "-", "if");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Loops"))
            {
                Keyword("for",   "-", "");
                Keyword("while", "-", "");
                Keyword("do",    "-", "");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Functions"))
            {
                Keyword("goto",   "-", "goto");
                Keyword("gosub",  "-", "gosub");
                Keyword("return", "-", "gosub");
                Keyword("end",    "-", "gosub");
                Keyword("call",   "-", "call");
                ImGui::Separator();
                Keyword("def",    "-", "def");
                Keyword("enddef", "-", "def");
                ImGui::Separator();
                Keyword("print",  "-", "print");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Classes"))
            {
                Keyword("class",    "-", "class");
                Keyword("endclass", "-", "class");
                Keyword("me",   "-", "class");
                Keyword("new",  "-", "class");
                Keyword("var",  "-", "class");
                Keyword("is",   "-", "class");
                ImGui::Separator();
                Keyword("reflect", "-", "class");
                Keyword("mem",     "-", "class");
                Keyword("type",    "-", "class");
                Keyword("get",   "-", "class");
                Keyword("set",   "-", "class");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Maths"))
            {
                Keyword("abs", "number", "maths");
                Keyword("sgn", "number", "maths");
                Keyword("sqr", "number", "maths");
                Keyword("exp", "number", "maths");
                Keyword("log", "number", "maths");
                Keyword("mod", "number", "maths");
                ImGui::Separator();
                Keyword("sin",  "number", "trigonometric");
                Keyword("cos",  "number", "trigonometric");
                Keyword("tan",  "number", "trigonometric");
                Keyword("asin", "number", "trigonometric");
                Keyword("acos", "number", "trigonometric");
                Keyword("atan", "number", "trigonometric");
                ImGui::Separator();
                Keyword("ceil",  "number", "rounding");
                Keyword("round", "number", "rounding");
                Keyword("floor", "number", "rounding");
                Keyword("fix",   "number", "rounding");
                ImGui::Separator();
                Keyword("srnd",  "number",         "random");
                Keyword("rnd",   "max, [min,max]", "random");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("String"))
            {
                Keyword("asc",   "char",    "string");
                Keyword("chr",   "code",    "string");
                ImGui::Separator();
                Keyword("left",  "string, position",          "string");
                Keyword("right", "string, position",          "string");
                Keyword("mid",   "string, position, length",  "string");
                ImGui::Separator();
                Keyword("str",   "numbrer", "string");
                Keyword("val",   "string",  "string");
                Keyword("len",   "string",  "string");
                Keyword("print", "-",       "string");

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Collection"))
            {
                Keyword("list",   "...",    "");
                Keyword("dict",   "...",    "");
                ImGui::Separator();
                Keyword("push",  "list, value", "pushpop");
                Keyword("pop",   "list",        "pushpop");
                Keyword("back",  "list",        "pushpop");
                ImGui::Separator();
                Keyword("insert", "list, position, value", "insort");
                Keyword("sort",   "list",                   "insort");
                ImGui::Separator();
                Keyword("get", "list, position",        "getset");
                Keyword("set", "list, position, value", "getset");
                ImGui::Separator();
                Keyword("iterator",  "list",     "iterator");
                Keyword("move_next", "iterator", "iterator");
                ImGui::Separator();
                Keyword("exists",   "list, value", "exists");
                Keyword("index_of", "list, value", "exists");
                ImGui::Separator();
                Keyword("remove",   "list, position", "remove");
                Keyword("clear",    "list",           "remove");
                Keyword("clone",    "list",           "remove");
                Keyword("to_array", "list",           "remove");
                ImGui::Separator();                         
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Enginsito"))
        {
            if (ImGui::TreeNode("Game Flow"))
            {
                Keyword("INIT()",  "-", "flow");
                Keyword("TICK()",  "-", "flow");
                Keyword("DRAW()",  "-", "flow");
                Keyword("PAUSE()", "-", "flow");
                Keyword("CLOSE()", "-", "flow");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Inputs"))
            {
                if (ImGui::TreeNode("Keyboard"))
                {   
                    Keyword("keyPressed",  "keyCode", "keyboard");
                    Keyword("keyReleased", "keyCode", "keyboard");
                    Keyword("keyUp",       "keyCode", "keyboard");
                    Keyword("keyDown",     "keyCode", "keyboard");
                    ImGui::Separator();
                    Keyword("keyGet",  "", "keyboardGet");
                    Keyword("keyChar", "", "keyboardGet");
         
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Mouse"))
                {
                    Keyword("mousePressed",  "button", "mouse");
                    Keyword("mouseReleased", "button", "mouse");
                    Keyword("mouseUp",       "button", "mouse");
                    Keyword("mouseDown",     "button", "mouse");
                    ImGui::Separator();
                    Keyword("mouseWheel",  "", "mouseextra");
                    Keyword("mouseSetPos", "", "mouseextra");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Gamepad"))
                {
                    Keyword("joyIsAviable", "id",           "gamepad");
                    Keyword("joyName",      "id",           "gamepad");
                    ImGui::Separator();
                    Keyword("joyPressed",   "id, button",   "gamepad");
                    Keyword("joyReleased",  "id, button",   "gamepad");
                    Keyword("joyUp",        "id, button",   "gamepad");
                    Keyword("joyDown",      "id, button",   "gamepad");
                    ImGui::Separator();
                    Keyword("joyGet",       "",             "gamepad");
                    Keyword("joyAxisCount", "id",           "gamepad");
                    Keyword("joyAxisValue", "id, axisId",   "gamepad");
                    ImGui::TreePop();
                }                
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Draw"))
            {
                Keyword("cls",      "color",                              "");
                Keyword("pixel",    "x, y, color",                        "");
                Keyword("line",     "x1, y1, x2, y2, thickness, color",   "");
                Keyword("rect",     "x, y, width, height, border, color", "");
                Keyword("circle",   "x, y, radius, border, color",        "");
                ImGui::Separator();
                Keyword("text", "text, x, y, scale, color",     "");
                Keyword("sprite", "id, x, y, color, [flags]",   "");
                Keyword("meta", "id, x, y, [flags]",            "");
                ImGui::Separator();
                Keyword("rectRound", "x, y, width, height, radius, roundness, border, color",    "");
                Keyword("ring", "x, y, radiusIn, radiusOut, startAngle, endAngle, style, color", "");
                Keyword("ellipse", "x, y, radiusHorizontal, radiusVertical, style, color",       "");
                Keyword("triangle", "x1, y1, x2, y2, x3, y3, style, color",  "");
                Keyword("poly", "x, y, sides, radius, rotation, thickness, color",     "");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Sound"))
            {
                Keyword("chPreset", "channel, preset",     "sound");
                Keyword("chSet",    "channel, sequence",   "sound");
                Keyword("chPlay",   "channel, [sequence]", "sound");
                Keyword("chStop",   "channel",             "sound");
                ImGui::Separator();
                Keyword("chOn",  "channel, note, volume", "notes");
                Keyword("chOff", "channel",               "notes");
                ImGui::Separator();
                Keyword("chPos",        "channel",        "soundInfo");
                Keyword("chSize",       "channel",        "soundInfo");
                Keyword("chFrame",      "channel, frame", "soundInfo");
                Keyword("chGetNote",    "channel",        "soundInfo");
                Keyword("chGetNoteNum", "channel",        "soundInfo");
                ImGui::TreePop();
            } 
            if (ImGui::TreeNode("Memory"))
            {
                Keyword("peek", "direction",          "peekpoke");
                Keyword("poke", "direction, newValue","peekpoke");
                Keyword("loadData", "slot, data", "loadsave");
                Keyword("saveData", "slot, data", "loadsave");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tools"))
            {   
                Keyword("formatText", "text, value");
                Keyword("fontSpace",  "space");
                Keyword("textSize",   "text, scale");
                Keyword("quit",       "");

                ImGui::TreePop();
            }      
            ImGui::TreePop();
        }
        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginGroup();
        codeEditor.Render("keyword");
        ImGui::EndGroup();
    }
    else
    {
        codeEditor.SetText("");
        ProgramName = "";
    }


    if (ImGui::CollapsingHeader("MML"))
    {
        ImGui::BeginChild("MMLCh",ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::TreeNode("MML Info"))
        {

            ImGui::TextWrapped("Music Macro Language (MML) is a text-based programming language used to create music for various types of computer and video game systems, as well as some early personal computers and synthesizers. MML allows composers and programmers to define musical sequences and melodies using a series of text commands and notations.");
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
        ImGui::EndChild();
    }
  
    if (ImGui::CollapsingHeader("Key Codes"))
    {
        ImGui::BeginChild("KeyCodesCh",ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
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
        ImGui::EndChild();
    }

}