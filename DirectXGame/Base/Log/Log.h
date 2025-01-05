#pragma once
#include <string>
#include <format>
#include <Windows.h>

//string -> wstring
std::wstring ConvertString(const std::string& str);
//wstring -> string
std::string ConvertString(const std::wstring& str);
//Log出力
void LogText(const std::string& message);