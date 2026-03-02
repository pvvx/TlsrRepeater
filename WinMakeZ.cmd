@set TLPATH=D:\MCU\TelinkIoTStudio
@set PATH=%TLPATH%\bin;%TLPATH%\opt\tc32\bin;%TLPATH%\mingw\bin;%TLPATH%\opt\tc32\tc32-elf\bin;%PATH%
@set SWVER=_v0100
@del /Q .\bin\*.bin
@del /Q .\bin\*.zigbee
@del /Q .\build
set PROJECT_NAME=ZG807Z
make -s -j clean
make -s -j VERSION_BIN=%SWVER% PROJECT_NAME=%PROJECT_NAME% POJECT_DEF="-DBOARD=BOARD_%PROJECT_NAME%"
@if not exist "bin\%PROJECT_NAME%%SWVER%.bin" goto :error
python3 make_z\zb_bin_ota.py bin\ZG807Z%SWVER%.bin bin\TuyaZG807Z%SWVER% -m0x1141 -i0x0201 -v0x10993001 -s"Tuya to Telink-pvvx"
cd .\zigpy_ota
call update.cmd %SWVER%
cd ..
@exit
:error
echo "Error!"

         