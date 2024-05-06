using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Diagnostics;
using System.ComponentModel;
using System.Runtime.Remoting.Contexts;
using System.Windows.Forms;

namespace DOOMSonaInstallerGUI
{
    internal class InstallerLogic
    {
        public delegate void LogDelegate(string message);
        private static LogDelegate logFunction;

        public static void RegisterLogger(LogDelegate logger)
        {
            logFunction = logger;
        }

        private static void Log(string message)
        {
            // Check if a logging function is registered
            if (logFunction != null)
            {
                logFunction.Invoke(message);
            }
        }

        //static void ExtractZipWithOverwrite(string zipFilePath, string extractPath)
        //{
        //    using (ZipArchive archive = ZipFile.OpenRead(zipFilePath))
        //    {
        //        foreach (ZipArchiveEntry entry in archive.Entries)
        //        {
        //            string entryPath = Path.Combine(extractPath, entry.FullName);
        //
        //            // Ensure that the entry's parent directory exists
        //            string entryParentDir = Path.GetDirectoryName(entryPath);
        //            if (!Directory.Exists(entryParentDir))
        //                Directory.CreateDirectory(entryParentDir);
        //
        //            if ((!entry.FullName.EndsWith("/")) && (!entry.FullName.EndsWith("\\")))
        //            {
        //                // Check if the file already exists
        //                if (File.Exists(entryPath))
        //                {
        //                    //string filename = Path.GetFileName(entryPath);
        //
        //                    // if (filename == "dinput8.dll")
        //                    // {
        //                    //     // if it is the first time install...
        //                    //     if (GetGameVersion() == null)
        //                    //     {
        //                    //         string pathTag = Path.Combine(extractPath, asiLoaderTagFilename);
        //                    //         if (!File.Exists(pathTag))
        //                    //             File.Create(pathTag);
        //                    //     }
        //                    // }
        //                    // else if (filename == "Reloaded.Mod.Loader.Bootstrapper.asi")
        //                    // {
        //                    //     // if it is the first time install...
        //                    //     if (GetGameVersion() == null)
        //                    //     {
        //                    //         string pathTag = Path.Combine(extractPath, asiBootstrapperTagFilename);
        //                    //         if (!File.Exists(pathTag))
        //                    //             File.Create(pathTag);
        //                    //     }
        //                    // }
        //
        //                    File.Delete(entryPath); // Delete existing file
        //                }
        //
        //                Log("Writing: " + entryPath);
        //                // Extract the file
        //                entry.ExtractToFile(entryPath);
        //            }
        //        }
        //    }
        //}

        public static string ReadFileAndFlatten(string filePath)
        {
            try
            {
                if (!File.Exists(filePath))
                    return string.Empty;

                // Read all text from the file
                string text = File.ReadAllText(filePath);

                // Replace newline characters with spaces
                string flattenedText = text.Replace("\r\n", " ").Replace("\n", " ");

                return flattenedText;
            }
            catch (Exception)
            {
                return string.Empty;
            }
        }

        // WARNING: file gets removed if textToWrite is empty!!!
        public static void WriteTextToFile(string filePath, string textToWrite)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(textToWrite))
                {
                    if (File.Exists(filePath))
                        File.Delete(filePath);
                    return;
                }

                //if (File.Exists(filePath))
                File.WriteAllText(filePath, textToWrite);
            }
            catch (Exception)
            {
            }
        }

        private static void OptionallyLaunchGame()
        {
            if (IsCmdFlagPresent("--launchGame"))
            {
                try
                {
                    string pathConfigJson = Reloaded_GetCfgPath();
                    if (pathConfigJson == null)
                        return;

                    dynamic jsonObject = Reloaded_ParseJSON(pathConfigJson);
                    if (jsonObject == null)
                        return;

                    string pathLauncher = Reloaded_GetLauncherPath(jsonObject);
                    if (pathLauncher == null)
                        return;

                    string pathGameExe = Path.Combine(GetGamePath(), "P5R.exe");

                    ProcessStartInfo startInfo = new ProcessStartInfo();
                    startInfo.FileName = pathLauncher;
                    startInfo.Arguments = "--launch \"" + pathGameExe + "\"";

                    // Start the process
                    Process.Start(startInfo);

                }
                catch (Exception)
                {
                    return;
                }
            }
            return;
        }

        public static void DoExit()
        {
            OptionallyLaunchGame();
            Application.Exit();
        }

        private const string versionTagFilename = "DOOMSonaVersion.tag";
        private const string ignoreTagFilename = "DOOMSonaIgnore.tag";
        private const string zmenuFilename = "ZMenuP5R.asi";
        private const string cleanupFileListFilename = "CleanupList.txt";
        public static string[] cleanupFileList;

        // WAD management
        public static string DOOMArgs;
        public static string DOOMOverrideArgs;

        //private static string pathDoom1WAD;
        //private static string pathDoom2WAD;

        // public static int lastInstallErrorLevel = -1;
        // public static int lastTagErrorLevel = -1;
        // public static int lastBootstrapErrorLevel = -1;
        public static int lastDontInjectSetterErrorLevel = -1;

        public static bool bInstallCompleted = false;

        public static bool bUninstallMode = false;
        public static bool bGotoUninstall = false;
        public static bool bCleanupASILoader = false;

        // public static void SetDoom1WADPath(string path)
        // { 
        //     pathDoom1WAD = path;
        // }
        // 
        // public static void SetDoom2WADPath(string path)
        // {
        //     pathDoom2WAD = path;
        // }

        // public static string GetDoom1WADPath() {  return pathDoom1WAD; }
        // 
        // public static string GetDoom2WADPath() { return pathDoom2WAD; }

        public static string GetArgumentValue(string argName)
        {
            string[] commandLineArgs = Environment.GetCommandLineArgs();
            string argValue = null;

            for (int i = 0; i < commandLineArgs.Length; i++)
            {
                if (commandLineArgs[i] == argName && i + 1 < commandLineArgs.Length)
                {
                    argValue = commandLineArgs[i + 1];
                    break;
                }
            }

            return argValue;
        }

        public static bool IsCmdFlagPresent(string flag)
        {
            string[] commandLineArgs = Environment.GetCommandLineArgs();

            foreach (string arg in commandLineArgs)
            {
                if (arg == flag)
                {
                    return true;
                }
            }

            return false;
        }

        public static string GetGamePath()
        {
            // string[] args = Environment.GetCommandLineArgs();
            // 
            // if ((args != null) && (args.Length >= 2))
            // {
            //     return args[1];
            // }

            string overridePath = GetArgumentValue("--path");
            if (overridePath != null) 
            { 
                return overridePath; 
            }

            string rldPath = Reloaded_GetGamePath();
            if (rldPath != null)
                return rldPath;

            return null;
        }

        // public static string GetGameVersion()
        // {
        //     string line;
        //     try
        //     {
        //         string gamePath = GetGamePath();
        //         if (gamePath == null)
        //         {
        //             return null;
        //         }
        // 
        //         string versionTagPath = Path.Combine(gamePath, versionTagFilename);
        //         if (!File.Exists(versionTagPath))
        //         {
        //             return null;
        //         }
        // 
        //         line = null;
        // 
        //         // Open the file and create a StreamReader
        //         using (StreamReader reader = new StreamReader(versionTagPath))
        //         {
        //             line = reader.ReadLine();
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: GetGameVersion failed, exception: " + ex);
        //         return null;
        //     }
        // 
        //     return line;
        // }

        public static string GetModVersion()
        {
            try
            {
                string jsonText = File.ReadAllText("ModConfig.json");
                dynamic jsonObject = JsonConvert.DeserializeObject(jsonText);
                return jsonObject.ModVersion;
            }
            catch (Exception ex)
            {
                Log("ERROR: GetModVersion failed, exception: " + ex);
                return null;
            }
        }
        public static string Reloaded_GetGamePath()
        {
            string pathAppData = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            string pathConfigJson = Path.Combine(pathAppData, "Reloaded-Mod-Loader-II", "ReloadedII.json");
            try
            {
                string jsonText = File.ReadAllText(pathConfigJson);
                dynamic jsonObject = JsonConvert.DeserializeObject(jsonText);

                string pathAppsCfgDir = jsonObject.ApplicationConfigDirectory;
                string pathAppJson = Path.Combine(pathAppsCfgDir, "p5r.exe", "AppConfig.json");
                if (!File.Exists(pathAppJson))
                {
                    return null;
                }

                string jsonText2 = File.ReadAllText(pathAppJson);
                JObject jsonObject2 = JObject.Parse(jsonText2);

                return jsonObject2["WorkingDirectory"].ToString();
            }
            catch (Exception ex)
            {
                Log("ERROR: Reloaded_GetGamePath failed, exception: " + ex);
                return null;
            }
        }

        // public static int TagInstallationVersion(string destPath)
        // {
        //     string pathVersionTag = Path.Combine(destPath, "DOOMSonaVersion.tag");
        // 
        //     try
        //     {
        //         Log("Tagging with version: " + GetModVersion() + " into file: " + pathVersionTag);
        //         if (File.Exists(pathVersionTag))
        //         {
        //             Log("Removing old tag...");
        //             File.Delete(pathVersionTag);
        //         }
        // 
        //         using (StreamWriter writer = new StreamWriter(pathVersionTag))
        //         {
        //             writer.WriteLine(GetModVersion());
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         lastTagErrorLevel = -1;
        //         Log("ERROR: TagInstallationVersion failed, exception: " + ex);
        //         return -1;
        //     }
        // 
        //     lastTagErrorLevel = 0;
        //     return 0;
        // }

        public static string Reloaded_GetCfgPath()
        {
            string pathAppData = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            return Path.Combine(pathAppData, "Reloaded-Mod-Loader-II", "ReloadedII.json"); ;
        }

        public static dynamic Reloaded_ParseJSON(string jsonPath)
        {
            dynamic jsonObject;

            string jsonText = File.ReadAllText(jsonPath);
            jsonObject = JsonConvert.DeserializeObject(jsonText);

            return jsonObject;
        }

        public static string Reloaded_GetBoostrapper64Path(dynamic jsonObject)
        {
            return jsonObject.Bootstrapper64Path;
        }

        public static string Reloaded_GetLauncherPath(dynamic jsonObject)
        {
            return jsonObject.LauncherPath;
        }

        public static string Reloaded_GetAppCfgJsonPath(dynamic jsonObject)
        {
            string pathAppsCfgDir = jsonObject.ApplicationConfigDirectory;
            string pathAppJson = Path.Combine(pathAppsCfgDir, "p5r.exe", "AppConfig.json");

            return pathAppJson;
        }

        public static void Reloaded_SetDontInject(string pathJson, bool val)
        {
            JObject jsonObject;
            try
            {
                Log("Parsing app config from: " + pathJson);
                string jsonText = File.ReadAllText(pathJson);
                jsonObject = JObject.Parse(jsonText);
            }
            catch (Exception ex)
            {
                lastDontInjectSetterErrorLevel = -1;
                Log("ERROR: Reloaded DontInject setter parser failed, exception: " + ex);
                return;
            }
        
            try
            {
                Log("Setting \"DontInject\" flag...");
                jsonObject["DontInject"] = val;
        
                try
                {
                    int backupCount = 1;
                    string pathJsonBackup = pathJson + ".jsonbackup" + backupCount;
                    while (File.Exists(pathJsonBackup))
                    {
                        backupCount++;
                        pathJsonBackup = pathJson + ".jsonbackup" + backupCount;
                    }
        
                    Log("Backing up app config to: " + pathJsonBackup);
        
                    File.Copy(pathJson, pathJsonBackup);
                    File.Delete(pathJson);
                }
                catch (Exception ex)
                {
                    Log("ERROR: Reloaded DontInject backup failed, exception: " + ex);
                }
        
                Log("Writing JSON to: " + pathJson);
                File.WriteAllText(pathJson, jsonObject.ToString());
            }
            catch (Exception ex)
            {
                Log("ERROR: Reloaded DontInject setter writer failed, exception: " + ex);
                lastDontInjectSetterErrorLevel = -1;
                return;
            }
        
            Log("Reloaded_SetDontInject success!");
            lastDontInjectSetterErrorLevel = 0;
        }

        // public static int Reloaded_InstallBootstrap(string destPath)
        // {
        //     string pathConfigJson = Reloaded_GetCfgPath();
        // 
        //     Log("Installing Reloaded bootstrap...");
        //     
        //     dynamic jsonObject;
        //     try
        //     {
        //         Log("Parsing Reloaded config from: " + pathConfigJson);
        //         jsonObject = Reloaded_ParseJSON(pathConfigJson);
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Reloaded config JSON parsing failed, exception: " + ex);
        //         return -1;
        //     }
        // 
        //     try
        //     {
        //         string pathBootstrapper64 = Reloaded_GetBoostrapper64Path(jsonObject);
        //         string pathBootstrapperOut = Path.Combine(destPath, "Reloaded.Mod.Loader.Bootstrapper.asi");
        // 
        // 
        //         if (!File.Exists(pathBootstrapperOut))
        //         {
        //             Log("Copying bootstrapper: " + pathBootstrapper64 + " --> " + pathBootstrapperOut);
        //             File.Copy(pathBootstrapper64, pathBootstrapperOut);
        //         }
        //         else
        //         {
        //             Log("Bootstrapper already exists, skipping...");
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Reloaded bootstrapper copy failed, exception: " + ex);
        //         lastBootstrapErrorLevel = -1;
        //         return -1;
        //     }
        // 
        //     try
        //     {
        //         //string pathAppsCfgDir = jsonObject.ApplicationConfigDirectory;
        //         //string pathAppsCfgDir = 
        //         //string pathAppJson = Path.Combine(pathAppsCfgDir, "p5r.exe", "AppConfig.json");
        //         string pathAppJson = Reloaded_GetAppCfgJsonPath(jsonObject);
        // 
        //         Log("Setting \"DontInject\" to true on Reloaded game config: " + pathAppJson);
        // 
        //         if (File.Exists(pathAppJson))
        //         {
        //             Reloaded_SetDontInject(pathAppJson, true);
        //         }
        //         else
        //         {
        //             Log("WARNING: Cannot find app configuration at: " + pathAppJson);
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Reloaded DontInject setter failed, exception: " + ex);
        //         lastBootstrapErrorLevel = -1;
        //         return -1;
        //     }
        // 
        //     Log("Reloaded_InstallBootstrap success!");
        //     lastBootstrapErrorLevel = 0;
        //     return 0;
        // }

        // public static int PerformInstall(string srcZip, string destPath, string destModPath)
        // {
        //     bool bDOOM1WadCopyFailed = false;
        //     bool bDOOM2WadCopyFailed = false;
        // 
        //     if (!Directory.Exists(destPath))
        //     {
        //         lastInstallErrorLevel = -1;
        //         return -1;
        //     }
        // 
        //     try
        //     {
        //         Log("Extracting " + srcZip + " to " + destPath);
        //         //ZipFile.ExtractToDirectory(srcZip, destPath, true);
        //         ExtractZipWithOverwrite(srcZip, destPath);
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Extraction failed, exception: " + ex);
        //         lastInstallErrorLevel = -1;
        //         return -1;
        //     }
        // 
        //     try
        //     {
        //         if ((pathDoom1WAD != null) && (pathDoom1WAD.Length > 1))
        //         {
        //             if (!Directory.Exists(Path.Combine(destModPath, "DOOMSona")))
        //             {
        //                 Directory.CreateDirectory(Path.Combine(destModPath, "DOOMSona"));
        //             }
        // 
        //             string DOOMSonaWAD1Path = Path.Combine(destModPath, "DOOMSona", "DOOM.WAD");
        //             Log("Copying Doom 1 WAD: " + pathDoom1WAD + " to " + DOOMSonaWAD1Path);
        //             File.Copy(pathDoom1WAD, DOOMSonaWAD1Path, true);
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Doom 1 WAD copy failed, exception: " + ex);
        //         bDOOM1WadCopyFailed = true;
        //         //lastInstallErrorLevel = -1;
        //         //return -1;
        //     }
        // 
        //     try
        //     {
        //         if ((pathDoom2WAD != null) && (pathDoom2WAD.Length > 1))
        //         {
        //             if (!Directory.Exists(Path.Combine(destModPath, "DOOMSona")))
        //             {
        //                 Directory.CreateDirectory(Path.Combine(destModPath, "DOOMSona"));
        //             }
        // 
        //             string DOOMSonaWAD2Path = Path.Combine(destModPath, "DOOMSona", "DOOM2.WAD");
        //             Log("Copying Doom 2 WAD: " + pathDoom2WAD + " to " + DOOMSonaWAD2Path);
        //             File.Copy(pathDoom2WAD, DOOMSonaWAD2Path, true);
        //         }
        //     }
        //     catch (Exception ex)
        //     {
        //         Log("ERROR: Doom 2 WAD copy failed, exception: " + ex);
        //         bDOOM2WadCopyFailed = true;
        //         //lastInstallErrorLevel = -1;
        //         //return -1;
        //     }
        // 
        //     if (bDOOM1WadCopyFailed || bDOOM2WadCopyFailed)
        //         Log("PerformInstall finished with errors.");
        //     else
        //         Log("PerformInstall success!");
        // 
        //     lastInstallErrorLevel = 0;
        //     return 0;
        // }

        public static bool BuildCleanupList()
        {
            try
            {
                Log("Loading cleanup list from: " + cleanupFileListFilename);
                if (cleanupFileList != null)
                {
                    if (cleanupFileList.Length > 0)
                        Array.Clear(cleanupFileList, 0, cleanupFileList.Length);
                }
                cleanupFileList = File.ReadAllLines(cleanupFileListFilename);
            }
            catch (Exception ex)
            {
                Log("ERROR: Can't read cleanup list file: " + ex);
                return false;
            }

            return true;
        }

        public static int PerformCleanup(string destPath)
        {
            if (!BuildCleanupList())
                return -1;

            try
            {
                string tagPath = Path.Combine(destPath, versionTagFilename);
                if (File.Exists(tagPath))
                {
                    Log("Removing: " + tagPath);
                    File.Delete(tagPath);
                }

                string tag2Path = Path.Combine(destPath, ignoreTagFilename);
                if (File.Exists(tag2Path))
                {
                    Log("Removing: " + tag2Path);
                    File.Delete(tag2Path);
                }

                foreach (string path in cleanupFileList)
                {
                    string fullPath = Path.Combine(destPath, path);
                    
                    if (path.EndsWith("/") || path.EndsWith("\\"))
                    {
                        Log("Removing directory: " + fullPath);
                        Directory.Delete(fullPath, true);
                    }
                    else if (File.Exists(fullPath))
                    {
                        if (path == "bass.dll")
                        {
                            string pathZMenu = Path.Combine(destPath, zmenuFilename);
                            if (File.Exists(pathZMenu))
                            {
                                Log("Skipping because of ZMenu: " + fullPath);
                                continue;
                            }
                        }

                        Log("Removing: " + fullPath);
                        File.Delete(fullPath);
                    }
                }
            }
            catch (Exception ex)
            {
                Log("ERROR: Can't perform cleanup: " + ex);
            }

            return 0;
        }

        public static int PerformASILoaderCleanup(string destPath)
        {
            string fullPath = Path.Combine(destPath, "dinput8.dll");
            try
            {
                if (File.Exists(fullPath))
                {
                    Log("Removing ASI loader from: " + fullPath);
                    File.Delete(fullPath);
                }
            }
            catch (Exception ex)
            {
                Log("ERROR: Can't remove ASI loader: " + ex);
                return -1;
            }

            return 0;
        }

        public static int PerformASIBootstrapperCleanup(string destPath)
        {
            bool bPerformedCleanup = false;

            string fullPath = Path.Combine(destPath, "Reloaded.Mod.Loader.Bootstrapper.asi");
            try
            {
                if (File.Exists(fullPath))
                {
                    Log("Removing ASI bootstrapper from: " + fullPath);
                    File.Delete(fullPath);
                    bPerformedCleanup = true;
                }
            }
            catch (Exception ex)
            {
                Log("ERROR: Can't remove ASI bootstrapper: " + ex);
            }

            // don't set the flag if the user didn't have the bootstrapper...
            if (!bPerformedCleanup)
                return 0;

            Log("Setting \"DontInject\" flag to false...");
            try
            {
                string pathConfigJson = Reloaded_GetCfgPath();
                dynamic jsonObject;

                try
                {
                    Log("Parsing Reloaded config from: " + pathConfigJson);
                    jsonObject = Reloaded_ParseJSON(pathConfigJson);
                }
                catch (Exception ex)
                {
                    Log("ERROR: Reloaded config JSON parsing failed, exception: " + ex);
                    return -1;
                }

                try
                {
                    string pathAppJson = Reloaded_GetAppCfgJsonPath(jsonObject);

                    Log("Setting \"DontInject\" to false on Reloaded game config: " + pathAppJson);

                    if (File.Exists(pathAppJson))
                    {
                        Reloaded_SetDontInject(pathAppJson, false);
                    }
                    else
                    {
                        Log("WARNING: Cannot find app configuration at: " + pathAppJson);
                    }
                }
                catch (Exception ex)
                {
                    Log("ERROR: Reloaded DontInject setter failed, exception: " + ex);
                    return -1;
                }
            }
            catch (Exception ex)
            {
                Log("ERROR: Can't set DontInject flag: " + ex);
                return -1;
            }

            return 0;
        }

        public static bool IsProcessRunning(string processName)
        {
            Process[] processes = Process.GetProcessesByName(processName);
            return processes.Length > 0;
        }
    }
}
