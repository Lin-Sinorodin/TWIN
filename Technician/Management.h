#pragma once

#include <windows.h>
#include "server.h"

constexpr PCSTR DEFAULT_PORT{"12345"};
constexpr DWORD HOUR_IN_MILLISECONDS{ 1000 * 60 * 60 };  // 1 hour = 1000 milliseconds * 60 seconds * 60 minutes
constexpr LPCWSTR PROGRAM_MESSAGE{ L"MANAGEMENT PROGRAM IS UP" };
constexpr LPCWSTR PROGRAM_TITLE{ L"MANAGEMENT PROGRAM" };
constexpr LPCWSTR REGISTRY_RUN_VALUE_NAME{ L"ManagementProgram" };
constexpr LPCWSTR REGISTRY_RUN_VALUE_PATH{ L"C:\\Users\\hp\\Documents\\GitHub\\TWIN\\x64\\Release\\Technician.exe" };


class ManagementProgram {
public:
	ManagementProgram();
	~ManagementProgram();
	void setLogonRegistryEntry();
	void showMessageBox();
	void runProgram();
private:
	HANDLE m_ghMutex;
    Server server;
};
