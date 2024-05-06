#include "ModPath.hpp"
#include "Installer.hpp"

#include <filesystem>

namespace Installer
{
    constexpr const char* modVersionFilename = "DOOMSonaVersion.tag";
    constexpr const char* installerIgnoreTag = "DOOMSonaIgnore.tag";

    static void exitApplicationImmediately()
    {
        // Get the current process handle
        HANDLE hProcess = GetCurrentProcess();

        // Terminate the current process immediately
        TerminateProcess(hProcess, 0);
    }

    static void StartUninstaller()
    {
        std::filesystem::path pathThis = ModPath::GetThisModulePath<std::filesystem::path>().parent_path();
        std::filesystem::path installerPath = pathThis / "DOOMSonaInstallerGUI.exe";

        // temporarily set the work directory to the mod folder
        DWORD pathlen = GetCurrentDirectoryW(0, NULL);
        wchar_t* LastPath = new wchar_t[pathlen + 1];
        GetCurrentDirectoryW(pathlen, LastPath);

        SetCurrentDirectoryW(pathThis.wstring().c_str());

        std::string cmd = "start \"\" ";
        cmd += "\"";
        cmd += installerPath.string().c_str();
        cmd += "\"";
        cmd += " --launchGame --disableGameRunningCheck --cleanup --autoProceed";

        std::system(cmd.c_str());

        SetCurrentDirectoryW(LastPath);

        exitApplicationImmediately();
    }

    // Temporary v1.0.0 cleaner - we switched to containing the files fully within DOOMSona's directory now
    // This should be removed with the next version release after this one ships
	void CleanupOldFiles()
	{

		std::filesystem::path pathGame = ModPath::GetModulePath<std::filesystem::path>(0).parent_path();

		std::filesystem::path pathVerTag = pathGame / modVersionFilename;
		std::filesystem::path pathIgnoreTag = pathGame / installerIgnoreTag;
		std::filesystem::path pathOldASI = pathGame / "DOOMSona.asi";

		try
		{
			if (std::filesystem::exists(pathVerTag) || std::filesystem::exists(pathIgnoreTag) || std::filesystem::exists(pathOldASI))
			{
#ifdef _DEBUG
				printf_s("Cleaning up files from version 1.0.0...\n");
#endif
				StartUninstaller();
			}
		}
		catch (const std::filesystem::filesystem_error& ex)
		{
#ifdef _DEBUG
			printf_s("ERROR: Can't check file, Filesystem error: %s\n", ex.what());
#endif
		}
		catch (const std::exception& ex)
		{
#ifdef _DEBUG
			printf_s("ERROR: Can't check file: %s\n", ex.what());
#endif
		}
		catch (...)
		{
#ifdef _DEBUG
			printf_s("ERROR: Can't check file: %s\n", "Unknown");
#endif
		}
	}
}
