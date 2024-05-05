# DOOMSona Installer

This is a guided installer/uninstaller designed to automate the copy process and set up Reloaded-II.

It usually launches automatically for the first time a new version is detected (if the version tag file `DOOMSonaVersion.tag` has a different version)

## Arguments

You can pass `--help` to get the arguments in a dialog box.

```
--help = Show this help
--path PathToGame = Define the destination/game path
--launchGame = Launches Persona 5 Royale through Reloaded-II after finishing the installation
--disableGameRunningCheck = Disables checks if the game is already running or not
--disableRldRunningCheck = Disables checks if Reloaded-II is already running or not
```

## Installation process

Running it should be pretty straightforward.

- Launch installer from within the mod directory

- Close Reloaded-II if running

- Follow the guided installer

- The installer will copy files into the game directory and tag the installation with the version from `ModConfig.json`

It should automatically pick up the game path via Reloaded-II's app config, but in case it doesn't you can always override it with `--path`.

## Uninstall / cleanup process

The cleanup process works on the "best effort" method.

The cleanup file list is located in a text document `CleanupList.txt` (should be updated).

It will only remove the files that are found from the list.

You may launch the cleanup process at any time (even if the mod isn't installed).

NOTE: If ZMenu is detected as installed, `bass.dll` will not be touched.

## Reinstall / update process

In case the mod version doesn't match the tagged version, the installer will change the "Next" button into the "Update" button.

In case it is the same, the "Next" button will be named "Reinstall".

It doesn't perform any version comparisons per se. It only acts when it's different.

The process will firstly:

- clean up the old files via the uninstall procedure

- place the new files via the install procedure
