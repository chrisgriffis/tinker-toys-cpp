#include "precompiled.h"
#include "InsituCommon.h"
#include "InsituDebug.h"
#include "InsituDataTypes.h"
#include "TestHarness.h"
#include <string>
#include <iostream>

#include "../Distance.h"
#include "ConfigFile.hpp"

#define COMPILER_TIMEOUT 60000 /* in ms. 1 minute should be more than enough for any test case */

// Function that tries to compile a given file, and returns "false" if failed, or "true" if successful
// If expectSuccess == false, the return code is inverted.
bool AttemptToCompile(STD_STRING const &filename, STD_STRING const &compilerArgs = _T(""), bool expectSuccess = true)
{
    // Generic pipe we can use for redirecting stderr
    HANDLE stderrRead, stderrWrite;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    if (FALSE == CreatePipe(&stderrRead, &stderrWrite, &sa, 0))
    {
        InsituAssert(false /* test failed - could not create stderr redirection pipe */);
        return false;
    }

    // We need to turn off an environment variable to get cl.exe to actually send output to the handles we gave it.
    // Yes. I agree this makes no sense. Let us just move on assuming that the below line really does ensure that
    // we output to the pipes, and think no more of it.
    static TCHAR originalEnvironmentVariable[32768];
    if (false == expectSuccess) 
    {
        GetEnvironmentVariable(_T("VS_UNICODE_OUTPUT"), originalEnvironmentVariable, sizeof(originalEnvironmentVariable) / sizeof(TCHAR));
        SetEnvironmentVariable(_T("VS_UNICODE_OUTPUT"), NULL);
    }

    // Load property sheet so we can figure out what "AdditionalIncludeDirectories" is (keep it consistent)
    Insitu::Config::CConfigFile configFile;
    if (FAILED(configFile.LoadConfigFile(_T("..\\..\\..\\..\\..\\PropertySheets\\BaseSheets\\Base.props"), Insitu::Config::FILES_DIRECTORY_POLICY_NO_MOD_ONLY)))
    {
        InsituAssert(false /* test failed - could not open base property sheet */);
        return false;
    }

    STD_STRING additionalIncludeDirectories;
    if (FAILED(configFile.Get(additionalIncludeDirectories, _T("Project/ItemDefinitionGroup/ClCompile/AdditionalIncludeDirectories"))))
    {
        InsituAssert(false /* test failed - could not read AdditionalIncludeDirectories */);
        return false;
    }

    // Build command line
    STD_STRING commandLine = _T("cl /c /nologo /errorReport:none /EHsc /TP /DWIN32 /DDEBUG /I..\\..\\..\\..\\..\\Common\\UnitLib\\");
    TCHAR const SOLUTION_DIRECTORY[] = _T("..\\..\\..\\..\\..\\");
    TCHAR const MACRO_TO_SEARCH[] = _T("$(SolutionDir)");
    for (size_t start = additionalIncludeDirectories.find(MACRO_TO_SEARCH, 0), end = 0; start != std::string::npos; start = additionalIncludeDirectories.find(MACRO_TO_SEARCH, end + 1))
    {
        end = additionalIncludeDirectories.find(_T("\""), start + sizeof(MACRO_TO_SEARCH) / sizeof(TCHAR) - 1);
        commandLine += _T(" /I\"");
        commandLine += SOLUTION_DIRECTORY;
        commandLine += additionalIncludeDirectories.substr(start + sizeof(MACRO_TO_SEARCH) / sizeof(TCHAR) - 1, 1 + end - start - sizeof(MACRO_TO_SEARCH) / sizeof(TCHAR));
        commandLine += _T("\"");
    }

    


    // Start compilation
    commandLine += _T(" ") + compilerArgs +_T(" ") + filename;
    // std::wcerr << commandLine << std::endl;
    STARTUPINFO info = {0};
    info.cb = sizeof(info);
    info.lpReserved = NULL;
    info.lpDesktop = NULL;
    info.lpTitle = NULL;
    info.dwFlags = STARTF_USESTDHANDLES;
    info.cbReserved2 = 0;
    info.lpReserved2 = NULL;
    info.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    info.hStdOutput = expectSuccess ? GetStdHandle(STD_OUTPUT_HANDLE) : stderrWrite;
    info.hStdError = expectSuccess ? GetStdHandle(STD_OUTPUT_HANDLE) : stderrWrite;

    PROCESS_INFORMATION pi;
    TCHAR buffer[4096];
    _tcscpy_s(buffer, sizeof(buffer) / sizeof(TCHAR), commandLine.c_str());
    if (FALSE == CreateProcess(NULL, buffer, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &info, &pi))
    {
        InsituAssert(false /* test failed - could not create process */);
        return false;
    }
    
    // Wait for it to exit
    DWORD result = WaitForSingleObject(pi.hProcess, COMPILER_TIMEOUT);
    if (result != WAIT_OBJECT_0)
    {
        InsituAssert(false /* test failed - compiler did not respond within timeout period */ );
        return false;
    }

    // Get the exit code
    DWORD returnCode;
    if (FALSE == GetExitCodeProcess(pi.hProcess, &returnCode))
    {
        InsituAssert(false /* test failed - failed to retrieve return code from compiler */ );
        return false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(stderrRead);
    CloseHandle(stderrWrite);

    // Restore old environment variable
    if (false == expectSuccess)
    {
        SetEnvironmentVariable(_T("VS_UNICODE_OUTPUT"), originalEnvironmentVariable);
    }

    // Compiled successfully if exit code is zero
    return expectSuccess ? returnCode == 0 : returnCode != 0;
}

TEST(SourceGoodCases, UnitBase)
{
    TCHAR const PASS_PATH[] = _T(".\\UnitLibTests\\Pass\\*");

    WIN32_FIND_DATA findData;
    HANDLE search = FindFirstFile(PASS_PATH, &findData);
    if (search == INVALID_HANDLE_VALUE)
    {
        DWORD le = GetLastError();
        CHECK(false /*Could not find any tests to run*/);
        return;
    }

    do 
    {
        // Make sure it's a cpp file
        TCHAR const * const extension = _tcsrchr(findData.cFileName, _T('.'));
        if (extension != NULL && _tcscmp(extension, _T(".cpp")) == 0)
        {
            // Construct the path name
            TCHAR path[MAX_PATH];
            ZeroMemory(path, sizeof(path));
            memcpy(path, PASS_PATH, sizeof(PASS_PATH) - sizeof(TCHAR) * 2); // Chop off the * (and null terminator)
            _tcscat_s(path, findData.cFileName);

            CHECK(AttemptToCompile(STD_STRING(path)));
        }

        
    } while (TRUE == FindNextFile(search, &findData));

    CHECK(GetLastError() == ERROR_NO_MORE_FILES);
}

TEST(SourceBadCases, UnitBase)
{
    TCHAR const FAIL_PATH[] = _T(".\\UnitLibTests\\Fail\\*");

    WIN32_FIND_DATA findData;
    HANDLE search = FindFirstFile(FAIL_PATH, &findData);
    if (search == INVALID_HANDLE_VALUE)
    {
        CHECK(false /* Could not find any tests to run */);
        return;
    }

    do 
    {
        // Make sure it's a cpp file
        TCHAR const * const extension = _tcsrchr(findData.cFileName, _T('.'));
        if (extension != NULL && _tcscmp(extension, _T(".cpp")) == 0)
        {
            // Construct the path name
            TCHAR path[MAX_PATH];
            ZeroMemory(path, sizeof(path));
            memcpy(path, FAIL_PATH, sizeof(FAIL_PATH) - sizeof(TCHAR) * 2); // Chop off the * (and null terminator)
            _tcscat_s(path, findData.cFileName);

            // For failure cases, we'll run up to 10 different cases in a single file, to allow us to 
            // compact the number of files. Cases can be isolated by checking define CASE_N
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_1"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_2"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_3"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_4"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_5"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_6"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_7"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_8"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_9"), false));
            CHECK(AttemptToCompile(STD_STRING(path), _T("/DCASE_10"), false));
        }
    } while (TRUE == FindNextFile(search, &findData));

    CHECK(GetLastError() == ERROR_NO_MORE_FILES);
}