#ifndef ADDONAPI_H
#define ADDONAPI_H

#include <dxgi.h>
#include "../Mumble/LinkedMem.h"
#include "../Logging/FuncDefs.h"
#include "../Events/FuncDefs.h"
#include "../WndProc/FuncDefs.h"
#include "../Keybinds/FuncDefs.h"
#include "../imgui/imgui.h"
#include "../minhook/mh_hook.h"
#include "../DataLink/FuncDefs.h"
#include "../Textures/FuncDefs.h"
#include "../GUI/FuncDefs.h"
#include "../GUI/Widgets/QuickAccess/FuncDefs.h"
#include "NexusLinkData.h"

using namespace Mumble;

// Base
struct AddonAPI {};

// Revision 1
struct AddonAPI1 : AddonAPI
{
	/* Renderer */
	IDXGISwapChain*				SwapChain;
	ImGuiContext*				ImguiContext;
	void*						ImguiMalloc;
	void*						ImguiFree;
	GUI_ADDREM					RegisterRender;
	GUI_ADDREM					UnregisterRender;

	/* Minhook */
	MINHOOK_CREATE				CreateHook;
	MINHOOK_REMOVE				RemoveHook;
	MINHOOK_ENABLE				EnableHook;
	MINHOOK_DISABLE				DisableHook;

	/* Logging */
	LOGGER_LOGA					Log;
	LOGGER_ADDREM				RegisterLogger;
	LOGGER_ADDREM				UnregisterLogger;

	/* Events */
	EVENTS_RAISE				RaiseEvent;
	EVENTS_SUBSCRIBE			SubscribeEvent;
	EVENTS_SUBSCRIBE			UnsubscribeEvent;

	/* WndProc */
	WNDPROC_ADDREM				RegisterWndProc;
	WNDPROC_ADDREM				UnregisterWndProc;

	/* Keybinds */
	KEYBINDS_REGISTER			RegisterKeybind;
	KEYBINDS_UNREGISTER			UnregisterKeybind;

	/* DataLink */
	DATALINK_GETRESOURCE		GetResource;
	DATALINK_SHARERESOURCE		ShareResource;

	/* Textures */
	TEXTURES_GET				GetTexture;
	TEXTURES_LOADFROMFILE		LoadTextureFromFile;
	TEXTURES_LOADFROMRESOURCE	LoadTextureFromResource;

	/* Shortcuts */
	QUICKACCESS_ADDSHORTCUT		AddShortcut;
	QUICKACCESS_REMOVE			RemoveShortcut;
	QUICKACCESS_ADDSIMPLE		AddSimpleShortcut;
	QUICKACCESS_REMOVE			RemoveSimpleShortcut;

	/* API */
		// GW2 API FUNCS
		// LOGITECH API FUNCS
};

#endif