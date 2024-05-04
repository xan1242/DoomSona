using ReloadedDOOMSonaInstallerLauncher.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;
using System.ComponentModel;
using System.Diagnostics;

namespace ReloadedDOOMSonaInstallerLauncher.Configuration;
public class Config : Configurable<Config>
{
}

/// <summary>
/// Allows you to override certain aspects of the configuration creation process (e.g. create multiple configurations).
/// Override elements in <see cref="ConfiguratorMixinBase"/> for finer control.
/// </summary>
public class ConfiguratorMixin : ConfiguratorMixinBase
{
    public override bool TryRunCustomConfiguration(Configurator configurator)
    {
        // Specify the new working directory
        string newWorkingDir = @"Mods\p5rpc.addon.doomsona";

        // Create a ProcessStartInfo object
        ProcessStartInfo psi = new ProcessStartInfo
        {
            FileName = "Mods\\p5rpc.addon.doomsona\\DOOMSonaInstallerGUI.exe",
            WorkingDirectory = newWorkingDir, // Set the working directory
            UseShellExecute = false, // Required for setting working directory
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            CreateNoWindow = false // This will hide the cmd window
        };

        // Start the process
        Process process = new Process { StartInfo = psi };
        process.Start();

        return true;
    }
}
