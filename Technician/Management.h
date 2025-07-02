#pragma once

#include <windows.h>
#include <string>

using std::string;
using std::wstring;

constexpr DWORD HOUR_IN_MILLISECONDS{ 1000 * 60 * 60 };  // 1 hour = 1000 milliseconds * 60 seconds * 60 minutes
const wstring PROGRAM_MESSAGE{ L"MANAGEMENT PROGRAM IS UP" };
const wstring PROGRAM_TITLE{ L"MANAGEMENT PROGRAM" };
const wstring REGISTRY_RUN_VALUE_NAME{ L"ManagementProgram" };
const wstring REGISTRY_RUN_VALUE_PATH{ L"C:\\Users\\hp\\Documents\\GitHub\\TWIN\\x64\\Release\\Technician.exe" };


/* The exception thrown for mutex errors, appends the error code to the provide message. */
class MutexException : public std::exception {
public:
	explicit MutexException(string error, DWORD code);
	[[nodiscard]] const char* what() const noexcept override;
private:
	string m_error;
};


class ManagementProgram {
public:
	ManagementProgram();
	~ManagementProgram();
	ManagementProgram(const ManagementProgram&) = delete;
	ManagementProgram& operator= (const ManagementProgram&) = delete;
	void setLogonRegistryEntry();
	void showMessageBox();
	void runProgram();
private:
	HANDLE m_ghMutex;
};
