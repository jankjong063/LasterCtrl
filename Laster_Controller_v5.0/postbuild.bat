@echo off
setlocal

:: Set the source path for the .elf file (adjust the path if necessary)
set "SOURCE_PATH=C:\Users\88691\AppData\Local\Temp\arduino\sketches\BA18611A46F6801D20057DE0335EE58E\Laster_Controller.ino.elf"

:: Set the destination folder (where the .ino file is located)
set "DEST_FOLDER=F:\PHD\Arduino_Projects\Laster_Controller\Laster_Controller_v5.0"

:: Path to avr-objcopy tool (adjust the version if necessary)
set "AVR_OBJCOPY=C:\Users\88691\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin\avr-objcopy.exe"

:: Copy the .elf file to the destination folder
if exist "%SOURCE_PATH%" (
    copy "%SOURCE_PATH%" "%DEST_FOLDER%\Laster_Controller.ino.elf"
    echo .elf file copied to %DEST_FOLDER%
    
    :: Convert .elf to .bin using the full path to avr-objcopy
    "%AVR_OBJCOPY%" -O binary "%DEST_FOLDER%\Laster_Controller.ino.elf" "%DEST_FOLDER%\Laster_Controller.ino.bin"
    echo .bin file generated at %DEST_FOLDER%
    
    :: Convert .elf to .hex using the full path to avr-objcopy
    "%AVR_OBJCOPY%" -O ihex "%DEST_FOLDER%\Laster_Controller.ino.elf" "%DEST_FOLDER%\Laster_Controller.ino.hex"
    echo .hex file generated at %DEST_FOLDER%
) else (
    echo File not found: %SOURCE_PATH%
)

endlocal
