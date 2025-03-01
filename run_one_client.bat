@echo off

set ROOT_DIR=C:\Users\Shiny#\OneDrive\Documents\GitHub\TCP-Chat-OC\TCP-Chat-OC\x64\Debug
 
set client_exe=Client-OC.exe

set CLIENT_FULL_PATH=%ROOT_DIR%\%client_exe%

echo  "%CLIENT_FULL_PATH%" shown successfully.

set name=client
set host=127.0.0.1
set port=12345

start "" "%CLIENT_FULL_PATH%" "%name%" "%host%" "%port%"

pause

