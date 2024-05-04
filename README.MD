# DOOMSona (aka DOOM in Persona 5 Royale)

This is a heavily modified and extended fork of DOOM in Sonic Mania by TheStoneBanana.

You can find more info and download links on the [GameBanana page](TheStoneBanana).

## Fork Info

This repo contains 5 projects

- ChocoDoom - the modified Chocolate DOOM Sonic Mania fork to work with D3D11

- DOOMSona - ASI code which contains game and render hooks, scripts and acts as a general bridge to allow the game to be controlled via ingame script

- DOOMSonaInstallerGUI - installer/uninstaller application which guides the user through the initial setup process, automates setting the "Do not inject" flag in Reloaded-II

- ReloadedDOOMSonaInstallerLauncher - This is a simple C# DLL which just binds the "Configure Mod" button in Reloaded-II to start the installer

- Reloaded-DOOMSonaLib - This is a Reloaded-II native library designed to auto-start the installer on game startup and to act as a pivot point for DOOMSona.asi to get the path of the mod files precisely

### Modifications

There are a few modifications applied to the DOOM fork, namely:

- Win32 keyboard and mouse input (via GetAsyncKeyState and cursor positioning)

- XInput for controllers

- BASS for sound and BASSMIDI for music

- API to communicate with other code

- Function to dump the framebuffer into a RGBA texture (for rendering to D3D11)

- Basic level completion statistic tracking (for Persona)

- Melt screen at game exit

- I_Error modifications to notify the API of errors for graceful exits

- Argument parsing via `args.txt`

- ...and many other things

## Other data

- chocolate-doom-setup - setup binaries from v3.0.1 - used only for binding keys, nothing else

- ReloadedModStuff - Has stuff specific to the Reloaded-II mod manager (ModConfig.json, Preview.png, etc.)

- sf2 - Contains the SoundFont file used by DOOMSona. By default, this is the E-MU 4MB set (CT4MGM.SF2)

## Building

1. Ensure that you have `7z` in your PATH

2. Get [ThirteenAG's Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/latest) and place the x64 DLL as `dinput8.dll` in the `post-install` directory (you only need to do this once)

3. Build the entire solution in Visual Studio

4. Open the repository as the work directory in a cmd window

5. Launch `post-install\GatherFiles.bat <build_type>` where `<build_type>` can be `Release` or `Debug`

6. You can find the files in the `.Debug Package` or `.Release Package` directory

## Credits

- KingJackSkellington - P5R scripting and testing
- TheStoneBanana - Author of DOOM in Sonic Mania
- All contributors to the Chocolate DOOM and Crispy DOOM source port projects. You rock!

## License

This project is licensed under the GNU General Public License v2.0. Please see the LICENSE file for more details.