
@echo off
SetLocal EnableExtensions
set patch="%~dp0"
cd /d "%~dp0"

del /q /f /a "Binary\Aitotal.exe
rmdir /q /s "%patch%Binary\AitotalTMP"
rmdir /q /s "%patch%\Release"
del /q /f Binary\tools\*.dll

pause
goto :eof
