@echo off

 

set ROOT_DIR=C:\Users\Shiny#\OneDrive\Documents\GitHub\TCP-Chat-OC\TCP-Chat-OC

 
set client_exe=Client-OC\x64\Debug\Client-OC.exe
set server_exe=Server-OC\x64\Release\Server-OC.exe

set CLIENT_FULL_PATH=%ROOT_DIR%\%client_exe%
set SERVER_FULL_PATH=%ROOT_DIR%\%server_exe%

echo  "%CLIENT_FULL_PATH%" shown successfully.

REM start "" "%CLIENT_FULL_PATH%"  # Run the first client instance
REM start "" "%CLIENT_FULL_PATH%"  # Run the second client instance
REM start "" "%CLIENT_FULL_PATH%"   # Run the third client instance

 

REM This batch file starts a new command prompt window with parameters

set name=client 1
set host=127.0.0.1
set port=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name%" "%host%" "%port%"


REM echo Client started on  %name3% %host3%:%port3%.

pause