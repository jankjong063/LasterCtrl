@echo off
setlocal

:: Set the source path for the .elf file (adjust the path if necessary)
set "SOURCE_PATH=C:\Users\88691\AppData\Local\Temp\arduino\sketches\D968DBF643E3924424360F72DD82F010\Laster_Controller.ino.elf"

:: Set the destination folder (where the .ino file is located)
set "DEST_FOLDER=F:\PHD\Arduino_Projects\Laster_Controller\Laster_Controller_v18.0"

:: Path to xtensa-esp32-elf-objcopy tool (adjust the version if necessary)
set "ESP32_OBJCOPY=C:\Users\88691\AppData\Local\Arduino15\packages\esp32\tools\xtensa-esp32s3-elf-gcc\esp-2021r2-patch5-8.4.0\bin\xtensa-esp32s3-elf-objcopy.exe"

:: Copy the .elf file to the destination folder
if exist "%SOURCE_PATH%" (
    copy "%SOURCE_PATH%" "%DEST_FOLDER%\Laster_Controller.ino.elf"
    echo .elf file copied to %DEST_FOLDER%
    
    :: Convert .elf to .bin using the full path to xtensa-esp32-elf-objcopy
    "%ESP32_OBJCOPY%" -O binary "%DEST_FOLDER%\Laster_Controller.ino.elf" "%DEST_FOLDER%\Laster_Controller.ino.bin"
    echo .bin file generated at %DEST_FOLDER%
    
    :: Convert .elf to .hex using the full path to xtensa-esp32-elf-objcopy
    "%ESP32_OBJCOPY%" -O ihex "%DEST_FOLDER%\Laster_Controller.ino.elf" "%DEST_FOLDER%\Laster_Controller.ino.hex"
    echo .hex file generated at %DEST_FOLDER%
) else (
    echo File not found: %SOURCE_PATH%
)

endlocal
