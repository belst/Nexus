#include "KeybindsWindow.h"

namespace GUI
{
    std::string CurrentlyEditing;

    void KeybindsWindow::Render()
    {
        if (!Visible) { return; }

        ImGui::SetNextWindowSize(ImVec2(480.0f, 380.0f));
        if (ImGui::Begin("Keybinds", &Visible, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
        {
            Keybinds::Mutex.lock();
            for (auto& [identifier, keybind] : Keybinds::Registry)
            {
                ImGui::Text(identifier.c_str());
                ImGui::SameLine();
                if (ImGui::SmallButton(keybind.Bind.ToString().c_str()))
                {
                    CurrentlyEditing = identifier;
                    ImGui::OpenPopup("Set Keybind");
                }
            }
            Keybinds::Mutex.unlock();

            ImVec2 center(Renderer::Width * 0.5f, Renderer::Height * 0.5f);
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Set Keybind", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                Keybinds::IsSettingKeybind = true;
                if (Keybinds::CurrentKeybind == Keybind{})
                {
                    ImGui::Text(Keybinds::Registry[CurrentlyEditing].Bind.ToString().c_str());
                }
                else
                {
                    ImGui::Text(Keybinds::CurrentKeybind.ToString().c_str());
                }
                
                bool overwriting = false;

                if (Keybinds::CurrentKeybindUsedBy != CurrentlyEditing && Keybinds::CurrentKeybindUsedBy != "")
                {
                    ImGui::TextColored(ImVec4(255, 0, 0, 255), "You will overwrite %s.", Keybinds::CurrentKeybindUsedBy.c_str());
                    overwriting = true;
                }

                if (ImGui::Button("Unbind"))
                {
                    Keybinds::Set(CurrentlyEditing, "(null)");
                    CurrentlyEditing = "";
                    Keybinds::CurrentKeybindUsedBy = "";
                    Keybinds::IsSettingKeybind = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                ImGui::Spacing();
                ImGui::SameLine();
                if (ImGui::Button("Accept"))
                {
                    if (overwriting)
                    {
                        Keybinds::Set(Keybinds::CurrentKeybindUsedBy, "(null)");
                    }
                    Keybinds::Set(CurrentlyEditing, Keybinds::CurrentKeybind.ToString());
                    CurrentlyEditing = "";
                    Keybinds::CurrentKeybindUsedBy = "";
                    Keybinds::IsSettingKeybind = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    CurrentlyEditing = "";
                    Keybinds::CurrentKeybindUsedBy = "";
                    Keybinds::IsSettingKeybind = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void KeybindsWindow::MenuOption(int aCategory)
    {
        if (aCategory == 0)
        {
            ImGui::ToggleButton("Keybinds", &Visible, ImVec2(ImGui::GetFontSize() * 13.75f, 0.0f));
        }
    }
}