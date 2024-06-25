#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string getIniPath();
void setResolution(int width, int height, std::string _iniPath);
void makeFileReadOnly(const std::string& filePath);
void makeFileNotReadOnly(const std::string& filePath);
bool isReadOnly(const fs::path& path);
void setWindowMode(int mode, std::string _iniPath);

extern std::string errorMessages;