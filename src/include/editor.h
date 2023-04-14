#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

struct Editor
{
    Editor()
    {

    }

    ~Editor()
    {

    }

    void Draw()
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::Begin("XXX");
        ImGui::End();
    }
};