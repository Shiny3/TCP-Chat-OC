@echo off

set ROOT_DIR=C:\Users\Shiny#\OneDrive\Documents\GitHub\TCP-Chat-OC\TCP-Chat-OC\x64\Debug
 
set server_exe=Server-OC.exe

set SERVER_FULL_PATH=%ROOT_DIR%\%server_exe%

REM echo  "%SERVER_FULL_PATH%" shown successfully.

set name=server...
set host=127.0.0.1
set port=12345

start "" "%SERVER_FULL_PATH%" "%name%" "%host%" "%port%"

pause