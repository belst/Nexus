#ifndef GUI_ADDONSWINDOW_H
#define GUI_ADDONSWINDOW_H

#include <Windows.h>
#include <shellapi.h>

#include "../../../Shared.h"
#include "../../../Paths.h"
#include "../../../State.h"

#include "../../../Loader/Loader.h"

#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_extensions.h"

#include "../../IWindow.h"
#include "AddonItem.h"

namespace GUI
{
	class AddonsWindow : public IWindow
	{
	public:
		AddonsWindow(std::string aName);
		void Render();
	};
}

#endif