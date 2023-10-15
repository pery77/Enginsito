#include "documentation.h"
#include "imgui/imgui.h"

Documentation::Documentation(/* args */)
{
}

Documentation::~Documentation()
{
}

void Documentation::Draw(bool* p_open)
{
    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::SeparatorText("ABOUT THIS DEMO:");
    }
}