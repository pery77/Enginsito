#include "bios.h"

Bios::Bios(){
    std::stringstream ss;
    ss << ASSETS_FOLDER << "/default.mem";
    Tools::LoadMemory(ss.str().c_str());
    Tools::InitFont();
}

Bios::~Bios(){}

void Bios::LoadBoot(){
    std::string line;
    std::ifstream myfile (BOOT_FILE);
    if (myfile.is_open()){
        while ( getline (myfile, line) ){
            currentLine =  line;
            ProcessCommand();
        }
        myfile.close();
    }else{
        printf("No boot file found.\n");
    }
}

void Bios::Update(){
    ClearBackground(Tools::GetColor(backColor));

    delta += GetFrameTime();
    cursor = (delta > 0.5) == 0 ? "_" : "";
    if (delta > 1.0) delta = 0;

    std::stringstream ss(screenLines);
    int lineY = 0;
    std::string temp;
    bool overLine = false;

    while (std::getline(ss, temp)){
        DrawTextEx(Tools::GetFont(), temp.c_str(), (Vector2){0, lineY}, 8, 0, Tools::GetColor(frontColor));
        //DrawText(temp.c_str(), 0, lineY, 8, Tools::GetBiosColor(frontColor));
        lineY += 9;
        if (lineY > 184){   
            overLine = true;
            break;
        }
    }

    int ch = GetCharPressed();
    int key = GetKeyPressed();

    if (!overLine){
        DrawTextEx(Tools::GetFont(),TextFormat("%s:>%s%s",CurrentPath.c_str(), currentLine.c_str(), cursor), (Vector2){0, lineY}, 8, 0,Tools::GetColor(frontColor));
        //DrawText(TextFormat("%s:>%s%s",CurrentPath.c_str(), currentLine.c_str(), cursor), 0, lineY, 8,Tools::GetBiosColor(frontColor));
        
        if (key != 0){
            if (key == 257) { //Enter
                ProcessCommand();
                if (lineY > 171) screenLines.erase(0, screenLines.find("\n") + 1);
            }
            if (key == 259 && strlen(currentLine.c_str()) > 0) currentLine.pop_back(); //Backspace
            if (key == 261 && strlen(currentLine.c_str()) > 0) currentLine.clear(); 
            if (key == 298) printf(CurrentPath.c_str());//F9 tests
            //https://www.barcodefaq.com/ascii-chart-char-set/

        }
        if (ch != 0 && MeasureTextEx(Tools::GetFont(), currentLine.c_str(),8,0).x < 312){
            currentLine += Tools::GetCharFromCodepoint(ch);
        }
    }
    else{
        DrawTextEx(Tools::GetFont(),"Press Enter to continue.",(Vector2){0,lineY},8,0,Tools::GetBiosColor(frontColor));
        if (IsKeyReleased(KEY_ENTER))
            screenLines.erase(0, screenLines.find("\n") + 1);
    }

}
bool Bios::checkCommand(std::string command, std::string value){
        return std::equal(command.begin(), command.end(),
                      value.begin(), value.end(),
                      [](char command, char value) {
                          return tolower(command) == tolower(value);
                      });
}

void Bios::ProcessCommand()
{
    currentLine.push_back('\n');
    screenLines += currentLine;

    int init_size = strlen(currentLine.c_str());
    char delim[] = " ";

    Tools::Trim((char*)currentLine.c_str());
    char *ptr = strtok((char*)currentLine.c_str(), delim);
    
    lastCommand.command = "";
    for (int i = 0; i < 9; i++)	{
        lastCommand.args[i] = "";
	}

    int i = 0;
    while(ptr != NULL){
        if (i == 0) 
            lastCommand.command = ptr;//Tools::ToUpper(ptr);
        else
            lastCommand.args[i-1] = ptr;//Tools::ToUpper(ptr);

        i++;
		ptr = strtok(NULL, delim);
        if (i>9) break;
	}

/*
    printf("Command: '%s'\n", lastCommand.command.c_str()); 
    for (int i = 0; i < 9; i++)	{
        if (lastCommand.args[i] == "") break;
		printf("Args: '%s', ", lastCommand.args[i].c_str()); 
	}
*/
    currentLine.clear();

    if(checkCommand(lastCommand.command,"HELP")){
        screenLines += HelpInfo;
        return;
    }

    if (checkCommand(lastCommand.command,"CLS")){
        screenLines.clear();
        return;
    }

    if (checkCommand(lastCommand.command,"EXIT")){
        ShouldClose = true;
        return;
    }

    if (checkCommand(lastCommand.command,"COLOR")){
        int bc = atoi(lastCommand.args[0].c_str());
        int fc = atoi(lastCommand.args[1].c_str());
        if (bc != fc && bc < 64 && fc < 64){
            backColor = bc;
            frontColor = fc;
        }
        return;
    }

    if (checkCommand(lastCommand.command,"RUN")){
        ShouldRun = true;
        return;
    }

    if (checkCommand(lastCommand.command,"DIR")){
        std::stringstream ss = Tools::GetFolders(CurrentPath.c_str());
        std::string temp;
        while (std::getline(ss, temp)){
            screenLines += "[" + temp + "]\n";
        }
        ss = Tools::GetFiles(CurrentPath.c_str());
        while (std::getline(ss, temp)){
            screenLines += "> " + temp + "\n";
        }
        return;
    }

    if (checkCommand(lastCommand.command,"PRINT")){
        lastCommand.args[0].push_back('\n');
        screenLines += lastCommand.args[0];
        return;
    }

    if (checkCommand(lastCommand.command,"CD.")){
        removeSubPath();
        
    }
    if (checkCommand(lastCommand.command,"CD..")){
        CurrentPath = "";
    }

    if (checkCommand(lastCommand.command,"CD")){
        if (lastCommand.args[0].find('/') != std::string::npos) return;
        if (lastCommand.args[0] == "" ) return;
        if (CurrentPath == ""){
            if (lastCommand.args[0].find('.') != std::string::npos) return;
            addSubPath(lastCommand.args[0]);
        }
        else{
            if (lastCommand.args[0].find('.') != std::string::npos) {
                    removeSubPath();
                }
                else {
                    addSubPath(lastCommand.args[0]);
                }
        }
        return;
    }

    if (checkCommand(lastCommand.command,"MEM")){
        if (CurrentProgram != "")
            screenLines += CurrentProgram + " loaded.\n";
        else
            screenLines += "No program loaded.\n";
        return;
    }

    if (checkCommand(lastCommand.command,"LOAD")){
        if (Tools::FileExist(CurrentPath, lastCommand.args[0])){
            CurrentProgram = lastCommand.args[0];
            screenLines += "Loaded " + CurrentProgram + " in memory.\n";
        }
        else{
            screenLines += "Fail loading " + lastCommand.args[0] + "\n";
            CurrentProgram = "";
        }

        return;
    }
}

std::string Bios::GetFile(){
    namespace fs = std::filesystem;
    fs::path dir (ASSETS_FOLDER);
    fs::path file (CurrentProgram + PROGRAM_EXTENSION);
    fs::path full_path = dir / CurrentPath / file;

    printf("Get file> %s\n",full_path.string().c_str());
    return full_path.string();
}
void Bios::SetFile(std::string filePath){
    namespace fs = std::filesystem;
    fs::path file = filePath;

    CurrentProgram = file.stem().string();
    CurrentPath = file.parent_path().string();
    CurrentPath = CurrentPath.erase(0, 9); //Delete > ./assets/ bye the dirty way. :(
    std::replace(CurrentPath.begin(), CurrentPath.end(), '\\', '/');
    printf("Set file> %s\n",file.string().c_str());
}

void Bios::addSubPath(std::string subPath){
    std::string prePath = (CurrentPath == "") ? "" : CurrentPath + "/";
     if (Tools::DirExist(prePath + subPath)) {
        if(CurrentPath == ""){
            CurrentPath = subPath;
            return;
        }
        else{
            CurrentPath += "/" + subPath;
        }
     }
}
void Bios::removeSubPath(){
    std::vector<std::string> paths = Tools::Split(CurrentPath, '/');

    if (paths.empty()) return;
    if (paths.size() == 1){
        CurrentPath = "";
        return;
    }
    paths.pop_back();

    std::string tmp = "";
    
    for (int i = 0; i < paths.size(); i++){       
        if (i < paths.size() - 1)
            tmp += paths[i] + '/';
        else
            tmp += paths[i];        
    }

    CurrentPath = tmp;
}

void Bios::DrawImGui(){

        ImGui::Begin(Tools::GetEngineName());
        static bool showConsole = false;
        ImGui::Checkbox("Console", &showConsole);

    if (showConsole){
        ImGui::Begin("Console");

        ImGui::End();
    }

    if (ImGui::CollapsingHeader("Info")){
        ImGui::SeparatorText("FPS");
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = GetFPS();
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }
        float average = 0.0f;
        for (int n = 0; n < IM_ARRAYSIZE(values); n++)
            average += values[n];
        average /= (float)IM_ARRAYSIZE(values);
        char overlay[32];
        sprintf(overlay, "average framerate: %.3f fps", average);

        ImGui::PlotLines("FPS", values, IM_ARRAYSIZE(values), values_offset,overlay ,0.0f, 60.0f, ImVec2(0, 80.0f));
        ImGui::SeparatorText("Program");
        ImGui::Text("Current Program:");
        ImGui::SeparatorText("Memory");
        ImGui::Text("Current Memory:");
    }

    if (ImGui::CollapsingHeader("Palette"))
    {
    }
    if (ImGui::CollapsingHeader("Graphics"))
    {
    }
    if (ImGui::CollapsingHeader("CRT filter"))
    {
    }
    if (ImGui::CollapsingHeader("SFX synth"))
    {
    }
    if (ImGui::CollapsingHeader("Music"))
    {
    }
    if (ImGui::CollapsingHeader("Online"))
    {
    }

    ImGui::End();
}