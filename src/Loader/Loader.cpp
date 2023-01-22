#include "Loader.h"

#include "../core.h"
#include "../State.h"
#include "../Shared.h"
#include "../Paths.h"
#include "../Renderer.h"

namespace Loader
{
    std::mutex AddonsMutex;
    std::map<std::filesystem::path, AddonDefinition*> AddonDefs;
    AddonAPI APIDef{};

    std::thread UpdateThread;

    std::set<std::filesystem::path> ExistingLibs;
    std::set<std::filesystem::path> Blacklist;

	void Initialize()
	{
        State::AddonHost = ggState::ADDONS_LOAD;

        /* setup APIDefs */
        APIDef.SwapChain = Renderer::SwapChain;
        APIDef.ImguiContext = ImGui::GetCurrentContext();
        APIDef.MumbleLink = MumbleLink;

        VTableMinhook minhook{};
        minhook.CreateHook = MH_CreateHook;
        minhook.RemoveHook = MH_RemoveHook;
        minhook.EnableHook = MH_EnableHook;
        minhook.DisableHook = MH_DisableHook;
        APIDef.MinhookFunctions = minhook;

        VTableLogging logging{};
        logging.LogA = LogMessageA;
        logging.LogW = LogMessageW;
        logging.RegisterLogger = RegisterLogger;
        logging.UnregisterLogger = UnregisterLogger;
        APIDef.LoggingFunctions = logging;

        APIDef.RaiseEvent = EventHandler::RaiseEvent;
        APIDef.SubscribeEvent = EventHandler::SubscribeEvent;

        APIDef.RegisterKeybind = KeybindHandler::RegisterKeybind;

        State::AddonHost = ggState::ADDONS_READY;

        UpdateThread = std::thread(Update);
        UpdateThread.detach();
	}

    void Shutdown()
    {
        State::AddonHost = ggState::ADDONS_SHUTDOWN;
    }

    void LoadAddon(std::filesystem::path aPath)
    {
        std::string pathStr = aPath.string();
        const char* path = pathStr.c_str();
        GETADDONDEF getAddonDef = 0;
        HMODULE hMod = LoadLibraryA(path);

        /* lib load failed */
        if (!hMod)
        {
            Blacklist.insert(aPath);
            return;
        }

        getAddonDef = (GETADDONDEF)GetProcAddress(hMod, "GetAddonDef");
        if (getAddonDef == 0)
        {
            ARC_GETINITADDR getInitAddr = (ARC_GETINITADDR)GetProcAddress(hMod, "get_init_addr"); /* load arc mod instead */

            LogWarning("%s: %s", getInitAddr ? "ArcDPS extension found in directory" : "Unknown library found in directory", path);

            Blacklist.insert(aPath);
            FreeLibrary(hMod);
            return;
        }

        AddonDefinition* addon = getAddonDef();
        if (hMod && !addon->HasMinimumRequirements())
        {
            LogWarning("Addon loading cancelled. %s does not fulfill minimum requirements. At least define Name, Version, Author, Description as well as Load and Unload functions.", path);

            Blacklist.insert(aPath);
            FreeLibrary(hMod);
            return;
        }

        AddonDefs.insert({ aPath, addon });
        addon->Load(APIDef);

        LogInfo("Loaded addon: %s", path);
    }

    void UnloadAddon(std::filesystem::path aPath, bool manual)
    {
        std::string pathStr = aPath.string();
        const char* path = pathStr.c_str();
        
        AddonDefs[aPath]->Unload();
        AddonDefs.erase(aPath);

        LogInfo("Unloaded addon: %s", path);
    }

	void Update()
	{
        for (;;)
        {
            if (State::AddonHost == ggState::ADDONS_SHUTDOWN) { return; }

            std::set<std::filesystem::path> currentLibs;

            for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(Path::D_GW2_ADDONS))
            {
                if (entry.is_regular_file())
                {
                    std::filesystem::path fsPath = entry.path();
                    std::string pathStr = fsPath.string();
                    std::string dll = ".dll";

                    /* ends with .dll */
                    if (pathStr.size() >= dll.size() && 0 == pathStr.compare(pathStr.size() - dll.size(), dll.size(), dll))
                    {
                        AddonsMutex.lock();
                        if (AddonDefs.find(fsPath) == AddonDefs.end())
                        {
                            /* reload if not blacklisted */
                            if (Blacklist.find(fsPath) == Blacklist.end())
                            {
                                LoadAddon(fsPath);
                            }
                        }
                        AddonsMutex.unlock();
                        currentLibs.insert(fsPath);
                    }
                }
            }

            AddonsMutex.lock();
            if (currentLibs != ExistingLibs)
            {
                for (std::filesystem::path dllPath : ExistingLibs)
                {
                    if (currentLibs.find(dllPath) == currentLibs.end())
                    {
                        if (AddonDefs.find(dllPath) != AddonDefs.end())
                        {
                            UnloadAddon(dllPath);
                        }
                    }
                }
            }
            AddonsMutex.unlock();

            ExistingLibs = currentLibs;

            Sleep(5000);
        }
	}
}