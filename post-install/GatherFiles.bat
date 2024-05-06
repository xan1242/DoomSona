@echo off
setlocal

::
:: This is a simple script to gather all files and put them in the right spot for a mod package.
:: 
:: p5rpc.addon.doomsona - contains Reloaded-II mod stuff
::
:: USAGE:
:: - build entire solution
:: - set repo root directory as the work directory
:: - launch "post-install\GatherFiles.bat" <build_type> -- where <build_type> can be Release or Debug
::

if "%~1"=="" (
    echo Error: First argument is empty. Exiting script.
    exit /b 1
)

SET BuildType=%1

:: Ensure that the output directory structure exists
IF NOT EXIST ".%BuildType% Package" MKDIR ".%BuildType% Package"
IF NOT EXIST ".%BuildType% Package\p5rpc.addon.doomsona" MKDIR ".%BuildType% Package\p5rpc.addon.doomsona"
IF NOT EXIST ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona" MKDIR ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
IF NOT EXIST ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona\WADs" MKDIR ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona\WADs"

:: Copy over the C++ stuff if it exists
SET _FILECHOCODOOM="x64\%BuildType%\ChocoDoom.dll"
IF EXIST "%_FILECHOCODOOM%"  (
    COPY /Y "%_FILECHOCODOOM%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

SET _FILEBASS="ChocoDoom\includes\bass\x64\bass.dll"
IF EXIST "%_FILEBASS%"  (
    COPY /Y "%_FILEBASS%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona""
)

SET _FILEBASSMIDI="ChocoDoom\includes\bassmidi\x64\bassmidi.dll"
IF EXIST "%_FILEBASSMIDI%" (
    COPY /Y "%_FILEBASSMIDI%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona""
)

SET _FILESF2="sf2\DOOMSona.sf2"
IF EXIST "%_FILESF2%" (
    COPY /Y "%_FILESF2%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

:: chocolate-doom-setup stuff (likely to be removed, here just for simplicity)
SET _FILECHOCODOOMSETUP="chocolate-doom-setup\chocolate-doom-setup.exe"
IF EXIST "%_FILECHOCODOOMSETUP%" (
    COPY /Y "%_FILECHOCODOOMSETUP%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

SET _FILECHOCODOOMSDL2="chocolate-doom-setup\SDL2.dll"
IF EXIST "%_FILECHOCODOOMSDL2%" (
    COPY /Y "%_FILECHOCODOOMSDL2%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

SET _FILECHOCODOOMSDL2MIXER="chocolate-doom-setup\SDL2_mixer.dll"
IF EXIST "%_FILECHOCODOOMSDL2MIXER%" (
    COPY /Y "%_FILECHOCODOOMSDL2MIXER%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

SET _FILECHOCODOOMSDL2NET="chocolate-doom-setup\SDL2_net.dll"
IF EXIST "%_FILECHOCODOOMSDL2NET%" (
    COPY /Y "%_FILECHOCODOOMSDL2NET%" ".%BuildType% Package\p5rpc.addon.doomsona\DOOMSona"
)

SET _FILEASI="x64\%BuildType%\DOOMSona.dll"
IF EXIST "%_FILEASI%"  (
    COPY /Y "%_FILEASI%"  ".%BuildType% Package\p5rpc.addon.doomsona"
)

:: Copy over the C# stuff if it exists
SET _FILELAUNCHER="ReloadedDOOMSonaInstallerLauncher\bin\ReloadedDOOMSonaInstallerLauncher.dll"
IF EXIST "%_FILELAUNCHER%" (
    COPY /Y "%_FILELAUNCHER%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

SET _FILEINSTALLER="DOOMSonaInstallerGUI\bin\%BuildType%\DOOMSonaInstallerGUI.exe"
IF EXIST "%_FILEINSTALLER%" (
    COPY /Y "%_FILEINSTALLER%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

SET _FILEINSTALLERCLEANUPLIST="DOOMSonaInstallerGUI\CleanupList.txt"
IF EXIST "%_FILEINSTALLERCLEANUPLIST%" (
    COPY /Y "%_FILEINSTALLERCLEANUPLIST%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

SET _FILEJSONDLL="DOOMSonaInstallerGUI\bin\%BuildType%\Newtonsoft.Json.dll"
IF EXIST "%_FILEJSONDLL%" (
    COPY /Y "%_FILEJSONDLL%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

:: Copy over Reloaded stuff
SET _FILERELOADEDJSON="ReloadedModStuff\ModConfig.json"
IF EXIST "%_FILERELOADEDJSON%" (
    COPY /Y "%_FILERELOADEDJSON%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

SET _FILERELOADEDPNG="ReloadedModStuff\Preview.png"
IF EXIST "%_FILERELOADEDPNG%" (
    COPY /Y "%_FILERELOADEDPNG%" ".%BuildType% Package\p5rpc.addon.doomsona"
)

endlocal
