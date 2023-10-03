#pragma once

#include <cctype>
#include <cstdio>

#include "imgui/rlImGui.h"

struct Console
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  Clip;
    bool                  ScrollToBottom;

    Console()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        AutoScroll = true;
        Clip = false;
        ScrollToBottom = false;
    }

    ~Console()
    {
        ClearLog();
    }

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s)                           { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void    PrintLines(const char* item, bool br)
    {
        if (br) return;

         ImVec4 color;
         bool has_color = false;
         if (strstr(item, "[ERROR]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
         else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
         else if (strstr(item, "[INFO]")) { color = ImVec4(0.6f, 0.8f, 1.0f, 1.0f); has_color = true; }
         else if (strstr(item, "[WARN]")) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
         if (has_color)
             ImGui::PushStyleColor(ImGuiCol_Text, color);
         ImGui::TextUnformatted(item);
         if (has_color)
             ImGui::PopStyleColor();

    }

    void    Draw(bool* p_open)
    {

        ImGui::SetNextWindowSize(ImVec2(670, 500),ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(280, 20),ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Console", p_open))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Clear")) { ClearLog(); }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::Button("Copy");
        //TODO: FIX clip
        //ImGui::SameLine();
        //ImGui::Checkbox("Clip", &Clip);

        ImGui::SameLine();
        ImGui::Checkbox("Scroll", &AutoScroll);

        if(!Clip)
        {
            ImGui::SameLine();
            Filter.Draw("Filter", 180);
        }

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) ClearLog();

                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            
            if (copy_to_clipboard)  ImGui::LogToClipboard();

            if (Clip) //TODO FIX
            {
                ImGuiListClipper clipper;
                clipper.Begin(Items.Size);
                
                while (clipper.Step())
                {
                    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    {
                        const char* item = Items[i];            
                        PrintLines(item, false);
                    }
                }
            }
            else
            {
                for (int i = 0; i < Items.Size; i++)
                {
                    const char* item = Items[i];
                    PrintLines(item, !Filter.PassFilter(item));
                }
            }

            if (copy_to_clipboard) ImGui::LogFinish();    

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::End();
    }
};