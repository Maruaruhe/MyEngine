#pragma once
#include <string>
#include <format>
#include <Windows.h>

std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
void LogText(const std::string& message);