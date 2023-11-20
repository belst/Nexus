#include "Shared.h"

HMODULE						NexusHandle			= nullptr;
HMODULE						GameHandle			= nullptr;
HMODULE						D3D11Handle			= nullptr;
HMODULE						D3D11SystemHandle	= nullptr;

AddonVersion*				Version				= new AddonVersion{};
std::vector<std::string>	Parameters			= {};

LinkedMem*					MumbleLink			= nullptr;
Identity*					MumbleIdentity		= new Identity{};
bool						IsMoving			= false;
bool						IsCameraMoving		= false;
bool						IsGameplay			= false;

NexusLinkData*				NexusLink			= nullptr;

ImFont*						Font				= nullptr;
ImFont*						FontBig				= nullptr;
ImFont*						FontUI				= nullptr;

ImGuiWindowFlags			WindowFlags_Default =	ImGuiWindowFlags_AlwaysAutoResize |
													ImGuiWindowFlags_NoResize |
													ImGuiWindowFlags_NoCollapse;
ImGuiWindowFlags			WindowFlags_Overlay =	ImGuiWindowFlags_NoDecoration |
													ImGuiWindowFlags_AlwaysAutoResize |
													ImGuiWindowFlags_NoSavedSettings |
													ImGuiWindowFlags_NoFocusOnAppearing |
													ImGuiWindowFlags_NoNav |
													ImGuiWindowFlags_NoMove |
													ImGuiWindowFlags_NoInputs |
													ImGuiWindowFlags_NoBringToFrontOnFocus;
ImGuiWindowFlags			WindowFlags_Custom	=	ImGuiWindowFlags_AlwaysAutoResize |
													ImGuiWindowFlags_NoResize |
													ImGuiWindowFlags_NoCollapse |
													ImGuiWindowFlags_NoBackground |
													ImGuiWindowFlags_NoTitleBar;
ImGuiWindowFlags			WindowFlags_Watermark = ImGuiWindowFlags_NoDecoration |
													ImGuiWindowFlags_AlwaysAutoResize |
													ImGuiWindowFlags_NoSavedSettings |
													ImGuiWindowFlags_NoFocusOnAppearing |
													ImGuiWindowFlags_NoNav |
													ImGuiWindowFlags_NoMove |
													ImGuiWindowFlags_NoInputs |
													ImGuiWindowFlags_NoBringToFrontOnFocus |
													ImGuiWindowFlags_NoBackground;