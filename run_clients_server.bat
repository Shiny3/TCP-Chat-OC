@echo off

 

set ROOT_DIR=C:\Users\Shiny#\OneDrive\Documents\GitHub\TCP-Chat-OC\TCP-Chat-OC\x64\Debug
 
set client_exe=Client-OC.exe

set CLIENT_FULL_PATH=%ROOT_DIR%\%client_exe%

set server_exe=Server-OC.exe
 
set SERVER_FULL_PATH=%ROOT_DIR%\%server_exe%

echo  "%CLIENT_FULL_PATH%" shown successfully.

REM start "" "%CLIENT_FULL_PATH%"  # Run the first client instance
REM start "" "%CLIENT_FULL_PATH%"  # Run the second client instance
REM start "" "%CLIENT_FULL_PATH%"   # Run the third client instance

 

REM This batch file starts a new command prompt window with parameters

set name=client999
set host=127.0.0.1
set port=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name%" "%host%" "%port%"

REM echo Client started on  %name% %host%:%port%.

set name2=client333
set host2=127.0.0.1
set port2=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name2%" "%host2%" "%port2%"

REM echo Client started on  %name2% %host2%:%port2%.

set name3=client2809
set host3=127.0.0.1
set port3=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name3%" "%host3%" "%port3%"

REM echo Client started on  %name3% %host3%:%port3%.

pause