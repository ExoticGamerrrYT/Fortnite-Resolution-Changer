#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string getIniPath();

void setResolution(int width, int height, std::string _iniPath);
std::pair<int, int> getResolution(const std::string& _iniPath);

void setWindowMode(int mode, std::string _iniPath);
int getWindowMode(std::string _iniPath);

void makeFileReadOnly(const std::string& filePath);
void makeFileNotReadOnly(const std::string& filePath);
bool isReadOnly(const fs::path& path);


extern std::string errorMessages;