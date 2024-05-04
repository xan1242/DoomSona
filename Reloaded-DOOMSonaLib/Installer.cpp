//
// Reloaded mod loader DOOMSona installer launcher
//

#include "ModPath.hpp"
#include "Installer.hpp"

//#include <Shlobj_core.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

// #define JSON_THROW_ON_ERROR
// #include "json/single_include/nlohmann/json.hpp"
// 
// using ordered_json = nlohmann::ordered_json;
// using json = nlohmann::json;

namespace Installer
{
    constexpr const char* installSourceDirName = "DataForGameDir";
    constexpr const char* modVersionFilename = "DOOMSonaVersion.tag";
    constexpr const char* installerIgnoreTag = "DOOMSonaIgnore.tag";
    //constexpr const char* installerTag = "DOOMSonaInstaller";

	std::filesystem::path pathThis;
	std::filesystem::path pathGame;
    std::string modVersionString;

    // static int copyDirectory(const std::filesystem::path& sourceDir, const std::filesystem::path& destinationDir)
    // {
    //     try
    //     {
    //         // Create the destination directory if it doesn't exist
    //         if (!std::filesystem::exists(destinationDir))
    //         {
    //             printf_s("%s: Creating directory: %s\n", installerTag, destinationDir.string().c_str());
    //             std::filesystem::create_directories(destinationDir);
    //         }
    // 
    //         // Iterate through all entries in the source directory
    //         for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir))
    //         {
    //             // Get the corresponding path in the destination directory
    //             std::filesystem::path relativePath = std::filesystem::relative(entry.path(), sourceDir);
    //             std::filesystem::path destPath = destinationDir / relativePath;
    // 
    //             // Check if the current entry is a directory or a regular file
    //             if (std::filesystem::is_directory(entry.status()))
    //             {
    //                 printf_s("%s: Creating directory: %s\n", installerTag, destPath.string().c_str());
    //                 // Create the corresponding directory in the destination
    //                 std::filesystem::create_directories(destPath);
    //             }
    //             else if (std::filesystem::is_regular_file(entry.status()))
    //             {
    //                 printf_s("%s: Copying: %s --> %s\n", installerTag, entry.path().string().c_str(), destPath.string().c_str());
    //                 // Copy the file to the destination
    //                 try
    //                 {
    //                     std::filesystem::copy_file(entry.path(), destPath, std::filesystem::copy_options::overwrite_existing);
    //                 }
    //                 catch (const std::exception& e)
    //                 {
    //                     printf_s("%s: Error copying file: %s\n", installerTag, e.what());
    //                 }
    //             }
    //             // You can add more conditions for other types of file system entries if needed
    //         }
    //         printf_s("%s: Files copied successfully!\n", installerTag);
    //     }
    //     catch (const std::exception& e)
    //     {
    //         printf_s("%s: Error copying directory: %s\n", installerTag, e.what());
    //         return -1;
    //     }
    // 
    //     return 0;
    // }

    static void exitApplicationImmediately()
    {
        // Get the current process handle
        HANDLE hProcess = GetCurrentProcess();

        // Terminate the current process immediately
        TerminateProcess(hProcess, 0);
    }

    // static void restartApplication()
    // {
    //     // Get the path to the current executable
    //     std::filesystem::path pathExe = ModPath::GetModulePath<std::filesystem::path>(0);
    // 
    //     // Create a new process for the application
    //     STARTUPINFO         si = {};
    //     PROCESS_INFORMATION pi = {};
    // 
    //     if (CreateProcess(pathExe.wstring().c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    //     {
    //         CloseHandle(pi.hThread);
    //         CloseHandle(pi.hProcess);
    //     }
    // 
    //     exitApplicationImmediately();
    // }

    //namespace ReloadedBootstrapper
    //{
    //    static std::string cleanBackslashes(const std::string& input)
    //    {
    //        std::string result;
    //        result.reserve(input.size());
    //
    //        bool escaped = false;
    //        for (char ch : input)
    //        {
    //            if (ch == '\\' && !escaped)
    //            {
    //                escaped = true;
    //            }
    //            else
    //            {
    //                result.push_back(ch);
    //                escaped = false;
    //            }
    //        }
    //
    //        return result;
    //    }
    //
    //    static std::string ReadPathFromJson(const std::filesystem::path& filename)
    //    {
    //        std::ifstream file(filename);
    //        std::string line;
    //        std::string Bootstrapper64Path;
    //
    //        if (!file.is_open())
    //        {
    //            return "";
    //        }
    //
    //        while (std::getline(file, line))
    //        {
    //            std::size_t found = line.find("\"Bootstrapper64Path\"");
    //            if (found != std::string::npos)
    //            {
    //                std::size_t start = line.find(":", found + 1);
    //                std::size_t start_quote = line.find("\"", start + 1);
    //                std::size_t end = line.find("\"", start_quote + 1);
    //
    //                if (start != std::string::npos && start_quote != std::string::npos && end != std::string::npos)
    //                {
    //                    Bootstrapper64Path = line.substr(start_quote + 1, end - start_quote - 1);
    //                    break;
    //                }
    //            }
    //        }
    //
    //        file.close();
    //        return Bootstrapper64Path;
    //    }
    //
    //    static std::string ReadAppCfgPathFromJson(const std::filesystem::path& filename)
    //    {
    //        std::ifstream file(filename);
    //        std::string line;
    //        std::string ApplicationConfigDirectory;
    //
    //        if (!file.is_open())
    //        {
    //            return "";
    //        }
    //
    //        while (std::getline(file, line))
    //        {
    //            std::size_t found = line.find("\"ApplicationConfigDirectory\"");
    //            if (found != std::string::npos)
    //            {
    //                std::size_t start = line.find(":", found + 1);
    //                std::size_t start_quote = line.find("\"", start + 1);
    //                std::size_t end = line.find("\"", start_quote + 1);
    //
    //                if (start != std::string::npos && start_quote != std::string::npos && end != std::string::npos)
    //                {
    //                    ApplicationConfigDirectory = line.substr(start_quote + 1, end - start_quote - 1);
    //                    break;
    //                }
    //            }
    //        }
    //
    //        file.close();
    //        return ApplicationConfigDirectory;
    //    }
    //
    //    // #TODO: this doesn't work with Reloaded II because it resets it on app exit...
    //    // static int SetDontInjectFlagInAppConfig(const std::filesystem::path& filename)
    //    // {
    //    //     std::ifstream jsonfile(filename);
    //    //     if (!jsonfile.is_open())
    //    //     {
    //    //         return -1;
    //    //     }
    //    // 
    //    //     json appConfig;
    //    //     try
    //    //     {
    //    //         appConfig = json::parse(jsonfile, nullptr, true, true);
    //    //     }
    //    //     catch (const json::exception& e)
    //    //     {
    //    //         jsonfile.close();
    //    //         return -2;
    //    //     }
    //    //     catch (const std::exception& e)
    //    //     {
    //    //         jsonfile.close();
    //    //         return -2;
    //    //     }
    //    //     catch (...)
    //    //     {
    //    //         jsonfile.close();
    //    //         return -2;
    //    //     }
    //    // 
    //    //     jsonfile.close();
    //    // 
    //    //     appConfig["DontInject"] = true;
    //    //     std::ofstream ojsonfile(filename);
    //    //     if (!ojsonfile.is_open())
    //    //     {
    //    //         return -3;
    //    //     }
    //    // 
    //    //     ojsonfile << appConfig.dump(2);
    //    // 
    //    //     ojsonfile.flush();
    //    //     ojsonfile.close();
    //    //     return 0;
    //    // }
    //
    //    static int Install()
    //    {
    //        WCHAR appdatapath[MAX_PATH];
    //        BOOL result = SHGetSpecialFolderPath(nullptr, appdatapath, CSIDL_APPDATA, false);
    //        if (!result)
    //            return -1;
    //
    //        std::filesystem::path pathConfigJson = appdatapath;
    //        pathConfigJson /= "Reloaded-Mod-Loader-II";
    //        pathConfigJson /= "ReloadedII.json";
    //
    //        std::string pathBootstrapperStr = ReadPathFromJson(pathConfigJson);
    //        if (pathBootstrapperStr.empty())
    //            return -2;
    //
    //        pathBootstrapperStr = cleanBackslashes(pathBootstrapperStr);
    //
    //        std::filesystem::path srcPath = pathBootstrapperStr;
    //
    //        std::filesystem::path destPath = pathGame;
    //        destPath /= srcPath.filename();
    //        destPath.replace_extension("asi");
    //        
    //        printf_s("%s: Copying: %s --> %s\n", installerTag, srcPath.string().c_str(), destPath.string().c_str());
    //        // Copy the file to the destination
    //        try
    //        {
    //            std::filesystem::copy_file(srcPath, destPath, std::filesystem::copy_options::overwrite_existing);
    //        }
    //        catch (const std::exception& e)
    //        {
    //            printf_s("%s: Error copying file: %s\n", installerTag, e.what());
    //        }
    //        
    //        try
    //        {
    //            if (!std::filesystem::exists(destPath))
    //                return -1;
    //        }
    //        catch (const std::exception& e)
    //        {
    //            return -1;
    //        }
    //
    //        std::string pathAppCfgDirStr = ReadAppCfgPathFromJson(pathConfigJson);
    //        if (pathAppCfgDirStr.empty())
    //            return -2;
    //
    //        pathAppCfgDirStr = cleanBackslashes(pathAppCfgDirStr);
    //        std::filesystem::path pathAppCfg = pathAppCfgDirStr;
    //        pathAppCfg /= "p5r.exe";
    //        pathAppCfg /= "AppConfig.json";
    //
    //        try
    //        {
    //            if (!std::filesystem::exists(pathAppCfg))
    //                return -3;
    //        }
    //        catch (const std::exception& e)
    //        {
    //            return -3;
    //        }
    //
    //        // #TODO: this doesn't work with Reloaded II because it resets it on app exit...
    //        // SetDontInjectFlagInAppConfig(pathAppCfg);
    //
    //        return 0;
    //    }
    //}

    namespace ModVersion
    {
        static void TagInstalledVersion(const std::filesystem::path& destinationDir)
        {
            std::filesystem::path pathInstalledModVersionTag = destinationDir;
            pathInstalledModVersionTag /= modVersionFilename;

            std::ofstream ofile(pathInstalledModVersionTag);
            if (!ofile.is_open())
                return;

            ofile << modVersionString;
            ofile.flush();
            ofile.close();
        }

        static std::string ReadFromJson(const std::filesystem::path& filename)
        {
            std::ifstream file(filename);
            std::string line;
            std::string modVersion;

            if (!file.is_open())
            {
                return "0.0.0";
            }

            while (std::getline(file, line))
            {
                std::size_t found = line.find("\"ModVersion\"");
                if (found != std::string::npos)
                {
                    std::size_t start = line.find(":", found + 1);
                    std::size_t start_quote = line.find("\"", start + 1);
                    std::size_t end = line.find("\"", start_quote + 1);

                    if (start != std::string::npos && start_quote != std::string::npos && end != std::string::npos)
                    {
                        modVersion = line.substr(start_quote + 1, end - start_quote - 1);
                        break;
                    }
                }
            }

            file.close();
            return modVersion;
        }

        static std::string ReadFromInstall(const std::filesystem::path& filename)
        {
            std::ifstream file(filename);
            std::string modVersion;

            if (!file.is_open())
            {
                return "";
            }

            std::getline(file, modVersion);

            file.close();
            return modVersion;
        }
    }

    static bool bShouldPerformInstall()
    {
        std::filesystem::path pathIgnoreTag = pathGame;
        pathIgnoreTag /= installerIgnoreTag;

        try
        {
            if (std::filesystem::exists(pathIgnoreTag))
            {
                return false;
            }
        }
        catch (const std::exception& e)
        {
            return false;
        }



        std::filesystem::path pathInstalledModVersionTag = pathGame;
        pathInstalledModVersionTag /= modVersionFilename;

        std::string installedModVersion = ModVersion::ReadFromInstall(pathInstalledModVersionTag);

        return installedModVersion != modVersionString;
    }

    static void PerformInstall()
    {
        std::filesystem::path srcPath = pathThis;
        srcPath /= installSourceDirName;

        std::filesystem::path destPath = pathGame;

        std::filesystem::path installerPath = pathThis / "DOOMSonaInstallerGUI.exe";

        // temporarily set the work directory to the mod folder
        DWORD pathlen = GetCurrentDirectoryW(0, NULL);
        wchar_t* LastPath = new wchar_t[pathlen + 1];
        GetCurrentDirectoryW(pathlen, LastPath);

        SetCurrentDirectoryW(pathThis.wstring().c_str());

        std::string cmd = "@start ";
        cmd += installerPath.string().c_str();

        std::system(cmd.c_str());

        SetCurrentDirectoryW(LastPath);

        exitApplicationImmediately();

        // printf_s("%s: Copying files from %s to game at: %s\n", installerTag, srcPath.string().c_str(), destPath.string().c_str());
        // 
        // if (copyDirectory(srcPath, destPath) != 0)
        //     return;
        // 
        // printf_s("%s: Tagging mod install with version: %s\n", installerTag, modVersionString.c_str());
        // ModVersion::TagInstalledVersion(destPath);
        // 
        // printf_s("%s: Installing Reloaded ASI bootstrapper...\n", installerTag);
        // ReloadedBootstrapper::Install();
        // 
        // printf_s("%s: Restarting game...\n", installerTag);
        // restartApplication();
    }

	void Init()
	{
		pathThis = ModPath::GetThisModulePath<std::filesystem::path>().parent_path();
		pathGame = ModPath::GetModulePath<std::filesystem::path>(0).parent_path();

        std::filesystem::path pathModConfigJson = pathThis;
        pathModConfigJson /= "ModConfig.json";
        modVersionString = ModVersion::ReadFromJson(pathModConfigJson);

        if (bShouldPerformInstall())
            PerformInstall();
	}
}
