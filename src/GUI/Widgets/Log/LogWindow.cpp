#include "LogWindow.h"

#include "Shared.h"
#include "Paths.h"
#include "State.h"

#include "Logging/LogHandler.h"

#include "imgui.h"
#include "imgui_extensions.h"

namespace GUI
{
	float lwWidth = 38.0f;
	float lwHeight = 24.0f;

	bool SelectedOnly = false;
	std::string ChannelFilter;

	float off1 = 0.0f;
	float off2 = 0.0f;
	const char* filterLevels[] = { "Critical", "Warning", "Info", "Debug", "Trace", "All" };
	size_t amtShown = 0;

	LogWindow::LogWindow(std::string aName, ELogLevel aLogLevel)
	{
		Name = aName;
		LogLevel = aLogLevel;
	}

	void LogWindow::Render()
	{
		if (!Visible) { return; }

		if (!(off1 && off2))
		{
			off1 = ImGui::CalcTextSize("XXXXXXXXX").x;
			off2 = ImGui::CalcTextSize("XXXXXXXXXXX").x;
		}

		ImGui::SetNextWindowSize(ImVec2(lwWidth * ImGui::GetFontSize(), lwHeight * ImGui::GetFontSize()), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(Name.c_str(), &Visible, ImGuiWindowFlags_NoCollapse))
		{
			static int selectedLevel = (int)(ELogLevel::ALL)-1;
			ImGui::Text("Filter: "); ImGui::SameLine();
			ImGui::Combo("##filterLogLevel", &selectedLevel, filterLevels, IM_ARRAYSIZE(filterLevels));

			ELogLevel filterLevel = (ELogLevel)(selectedLevel + 1);

			ImGui::SameLine();
			ImGui::Checkbox("Selected level only", &SelectedOnly);

			float windowWidthQuarter = ImGui::GetWindowContentRegionWidth() / 4.0f;

			ImGui::Separator();
			{
				ImGui::BeginChild("logchannels", ImVec2(windowWidthQuarter, 0.0f));

				ImGui::Text("Channel:");

				LogHandler::Mutex.lock();
				{
					for (std::string ch : Channels)
					{
						float opacity = 0.8f;
						if (ch == ChannelFilter)
						{
							opacity = 1.0f;
						}
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, opacity);
						if (ImGui::Button(ch.c_str(), ImVec2(windowWidthQuarter, 0.0f)))
						{
							/* if the selected channel is already the set filter, reset filter */
							if (ChannelFilter == ch)
							{
								ChannelFilter = "";
							}
							else
							{
								ChannelFilter = ch.c_str();
							}
						}
						ImGui::PopStyleVar();
					}
				}
				LogHandler::Mutex.unlock();

				ImGui::TextDisabled("Showing %d out of %d", amtShown, LogEntries.size() > 400 ? 400 : LogEntries.size());

				ImGui::EndChild();
			}

			ImGui::SameLine();

			amtShown = 0;

			{
				ImGui::BeginChild("logmessages", ImVec2(windowWidthQuarter * 3 - 1, 0.0f));
				
				float wrapWidth = ImGui::GetContentRegionAvailWidth() - off1 - off2;

				LogHandler::Mutex.lock();
				{
					/* Show last 400 log messages */
					size_t start = 0;
					if (LogEntries.size() > 400) { start = LogEntries.size() - 400; }

					for (size_t i = start; i < LogEntries.size(); i++)
					{
						LogEntry entry = LogEntries[i];

						if (((filterLevel == ELogLevel::ALL) ||
							(SelectedOnly && entry.LogLevel == filterLevel) ||
							(!SelectedOnly && entry.LogLevel <= filterLevel)) &&
							((ChannelFilter == "") || ChannelFilter == entry.Channel))
						{
							amtShown++;

							const char* level;
							ImColor levelColor;
							switch (entry.LogLevel)
							{
								case ELogLevel::CRITICAL:   level = "[CRITICAL]";   levelColor = IM_COL32(255, 0, 0, 255);     break;
								case ELogLevel::WARNING:    level = "[WARNING]";    levelColor = IM_COL32(255, 255, 0, 255);   break;
								case ELogLevel::INFO:       level = "[INFO]";       levelColor = IM_COL32(0, 255, 0, 255);     break;
								case ELogLevel::DEBUG:      level = "[DEBUG]";      levelColor = IM_COL32(0, 148, 255, 255);   break;

								default:                    level = "[TRACE]";      levelColor = IM_COL32(220, 220, 220, 255); break;
							}

							/* time */
							ImGui::TextColored(levelColor, entry.TimestampString(false).c_str()); ImGui::SameLine(off1);

							/* level */
							ImGui::TextColored(levelColor, level); ImGui::SameLine(off1 + off2);

							float msgHeight = ImGui::CalcTextSize(entry.Message.c_str(), (const char*)0, false, wrapWidth - 1).y;

							/* message divider */
							ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, (ImVec4)levelColor);
							if (ImGui::BeginTable(("##" + entry.Message).c_str(), 1, ImGuiTableFlags_Borders, ImVec2(1.0f, msgHeight)))
							{
								ImGui::TableNextRow();
								ImGui::TableSetColumnIndex(0);
								ImGui::Text("");
								ImGui::EndTable();
							}
							ImGui::SameLine();
							ImGui::PopStyleColor();

							/* message */
							ImGui::TextWrapped(entry.Message.c_str());
							
							ImGui::Separator();
						}
					}
				}
				LogHandler::Mutex.unlock();

				if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				{
					ImGui::SetScrollHereY(1.0f);
				}

				ImGui::EndChild();
			}
		}
		ImGui::End();
	}

	void LogWindow::LogMessage(LogEntry aLogEntry) {}
}