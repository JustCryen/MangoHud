#include <vector>
#include <string>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <filesystem.h>

#include "blacklist.h"
#include "string_utils.h"
#include "file_utils.h"

namespace fs = ghc::filesystem;
std::string global_proc_name;
std::string global_engine_name;

static std::string get_proc_name() {
   // Note: It is possible to use GNU program_invocation_short_name.
   const std::string proc_name = get_wine_exe_name(/*keep_ext=*/true);
   if (!proc_name.empty()) {
       return proc_name;
   }
   return get_basename(get_exe_path());
}

static  std::vector<std::string> blacklist {
    "Amazon Games UI.exe",
    "Battle.net.exe",
    "BethesdaNetLauncher.exe",
    "EADesktop.exe",
    "EALauncher.exe",
    "EpicGamesLauncher.exe",
    "EpicWebHelper.exe",
    "explorer.exe",
    "ffxivlauncher.exe",
    "ffxivlauncher64.exe",
    "GalaxyClient.exe",
    "gamescope",
    "gldriverquery",
    "halloy",
    "IGOProxy.exe",
    "IGOProxy64.exe",
    "iexplore.exe",
    "InsurgencyEAC.exe",
    "Launcher", //Paradox Interactive Launcher
    "LeagueClient.exe",
    "LeagueClientUxRender.exe",
    "MarneLauncher.exe",
    "MarvelRivals_Launcher.exe",
    "monado-service",
    "Origin.exe",
    "OriginThinSetupInternal.exe",
    "plutonium.exe",
    "plutonium-launcher-win32.exe",
    "REDlauncher.exe",
    "REDprelauncher.exe",
    "RSI Launcher.exe",
    "rundll32.exe",
    "SocialClubHelper.exe",
    "StarCitizen_Launcher.exe",
    "steam",
    "Steam.exe",
    "steamwebhelper",
    "steamwebhelper.exe",
    "tabtip.exe",
    "UplayWebCore.exe",
    "vrcompositor",
    "vulkandriverquery",
};

static std::vector<std::string> blacklist_engine {
    "GTK"
};

static bool check_blacklisted() {
    std::string proc_name = get_proc_name();
    std::string engine_name = global_engine_name;
    global_proc_name = proc_name;
    bool blacklisted = std::find(blacklist.begin(), blacklist.end(), proc_name) != blacklist.end();
    blacklisted |= std::find(blacklist_engine.begin(), blacklist_engine.end(), engine_name) != blacklist_engine.end();

    static bool printed = false;
    if(blacklisted && !printed) {
        printed = true;
        SPDLOG_INFO("process '{}' is blacklisted in MangoHud", proc_name);
    }

    return blacklisted;
}

bool is_blacklisted(bool force_recheck) {
    static bool blacklisted = check_blacklisted();
    if (force_recheck)
        blacklisted = check_blacklisted();
    return blacklisted;
}

void add_blacklist(const std::string& new_item) {
    // check if item exits in blacklist before adding new item
    if(std::find(blacklist.begin(), blacklist.end(), new_item) != blacklist.end()) {
        return;
    }

    blacklist.push_back (new_item);
    is_blacklisted(true);
}


