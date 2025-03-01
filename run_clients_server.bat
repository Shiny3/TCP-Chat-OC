@echo off

 

set ROOT_DIR=D:\Rabota\OPENCODE\long coommit\TCP-Chat-OC-main\x64\Debug

 
set client_exe=Client-OC.exe
set server_exe=Server-OC.exe

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

REM echo Client started on  %name% %host%:%port%.

set name2=client 2
set host2=127.0.0.1
set port2=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name%" "%host%" "%port%"

REM echo Client started on  %name2% %host2%:%port2%.

set name3=client 3
set host3=127.0.0.1
set port3=12345

REM Replace "yourCommand" with the command you want to run
start "" "%CLIENT_FULL_PATH%" "%name3%" "%host3%" "%port3%"

REM echo Client started on  %name3% %host3%:%port3%.

pause