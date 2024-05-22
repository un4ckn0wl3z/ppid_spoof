#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <memory>

BOOL CreateProcessWithParent(DWORD parentId, PWSTR commandline)
{
	auto hProcess = ::OpenProcess(PROCESS_CREATE_PROCESS, FALSE, parentId);
	if (!hProcess)
	{
		return FALSE;
	}

	SIZE_T size;
	::InitializeProcThreadAttributeList(nullptr, 1, 0, &size);

	auto buffer = std::make_unique<BYTE[]>(size);
	auto attributes = reinterpret_cast<PPROC_THREAD_ATTRIBUTE_LIST>(buffer.get());
	::InitializeProcThreadAttributeList(attributes, 1, 0, &size);

	::UpdateProcThreadAttribute(attributes, 0,
		PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hProcess, sizeof(hProcess), nullptr, nullptr);

	STARTUPINFOEX si = { sizeof(si) };
	si.lpAttributeList = attributes;
	PROCESS_INFORMATION pi;
	BOOL created = ::CreateProcess(nullptr, commandline, nullptr, nullptr,
		FALSE, EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
		(STARTUPINFO*) & si, &pi
	);

	::CloseHandle(hProcess);
	::DeleteProcThreadAttributeList(attributes);
	return created;

}

int main(int argc, const char* argv[])
{
	WCHAR name[] = L"mspaint";
	CreateProcessWithParent(22624, name);
	return 0;
}
