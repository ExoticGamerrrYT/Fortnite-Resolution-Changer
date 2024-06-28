#include <iostream>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <string>
#include "simpleini/SimpleIni.h"

namespace fs = std::filesystem;

std::string errorMessages;

// Set message
void addErrorMessage(const std::string& message) {
    errorMessages += message + "\n";
}

// Get ini file path
std::string getIniPath()
{
    char userProfile[MAX_PATH];
    if (FAILED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {
        std::string errorMsg = "Error getting user profile path";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return "Error!";
    }
    std::string iniPath = std::string(userProfile) + "\\AppData\\Local\\FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini";
    return iniPath;
}

// Set resolution
void setResolution(int width, int height, std::string _iniPath)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error loading INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "ResolutionSizeX", std::to_string(width).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "ResolutionSizeY", std::to_string(height).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "LastUserConfirmedResolutionSizeX", std::to_string(width).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "LastUserConfirmedResolutionSizeY", std::to_string(height).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "DesiredScreenWidth", std::to_string(width).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "DesiredScreenHeight", std::to_string(height).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "LastUserConfirmedDesiredScreenWidth", std::to_string(width).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "LastUserConfirmedDesiredScreenHeight", std::to_string(height).c_str());

    rc = ini.SaveFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error saving INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
    }
    else {
        std::cout << "Resolutions successfully amended" << std::endl;
    }
}

// Get resolution
std::pair<int, int> getResolution(const std::string& _iniPath)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error loading INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return std::make_pair(-1, -1); // Error indicator
    }

    const char* resolutionXStr = ini.GetValue("/Script/FortniteGame.FortGameUserSettings", "ResolutionSizeX", "0");
    const char* resolutionYStr = ini.GetValue("/Script/FortniteGame.FortGameUserSettings", "ResolutionSizeY", "0");

    int resolutionX = std::stoi(resolutionXStr);
    int resolutionY = std::stoi(resolutionYStr);

    return std::make_pair(resolutionX, resolutionY);
}

// Set window mode
void setWindowMode(int mode, std::string _iniPath)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error loading INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "LastConfirmedFullscreenMode", std::to_string(mode).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "PreferredFullscreenMode", std::to_string(mode).c_str());
    ini.SetValue("/Script/FortniteGame.FortGameUserSettings", "FullscreenMode", std::to_string(mode).c_str());

    rc = ini.SaveFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error saving INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
    }
    else {
        std::cout << "Window modes successfully amended" << std::endl;
    }
}

// Get window mode
int getWindowMode(std::string _iniPath)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(_iniPath.c_str());
    if (rc < 0) {
        std::string errorMsg = "Error loading INI file";
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return 1; // Error indicator
    }

    const char* mode = ini.GetValue("/Script/FortniteGame.FortGameUserSettings", "LastConfirmedFullscreenMode", "0");

    return std::stoi(mode);
}

// Make read-only
void makeFileReadOnly(const std::string& filePath)
{
    DWORD attributes = GetFileAttributes(filePath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        std::string errorMsg = "Error getting file attributes: " + std::to_string(GetLastError());
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    attributes |= FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(filePath.c_str(), attributes)) {
        std::string errorMsg = "Error setting file attributes: " + std::to_string(GetLastError());
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    std::cout << "File set to read-only correctly." << std::endl;
}

// Make not read - only
void makeFileNotReadOnly(const std::string& filePath)
{
    DWORD attributes = GetFileAttributes(filePath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        std::string errorMsg = "Error getting file attributes: " + std::to_string(GetLastError());
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    attributes &= ~FILE_ATTRIBUTE_READONLY;

    if (!SetFileAttributes(filePath.c_str(), attributes)) {
        std::string errorMsg = "Error setting file attributes: " + std::to_string(GetLastError());
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return;
    }

    std::cout << "File set to writable correctly." << std::endl;
}

// See if it is read-only
bool isReadOnly(const fs::path& path) {
    try {
        auto fileStatus = fs::status(path);
        if (fs::exists(fileStatus) && fs::is_regular_file(fileStatus)) {
            auto perms = fileStatus.permissions();
            return (perms & fs::perms::owner_write) == fs::perms::none;
        }
        else {
            std::string errorMsg = "The file does not exist or is not a regular file.";
            std::cerr << errorMsg << std::endl;
            addErrorMessage(errorMsg);
            return false;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::string errorMsg = "Error al verificar el archivo: " + std::string(e.what());
        std::cerr << errorMsg << std::endl;
        addErrorMessage(errorMsg);
        return false;
    }
}
