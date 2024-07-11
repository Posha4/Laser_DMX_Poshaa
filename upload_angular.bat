RMDIR C:\Users\cleme\Documents\PlatformIO\Projects\240618-190049-esp32dev\data\ /S /Q

XCOPY C:\Users\cleme\Documents\Code\laser-dmx\laser-dmx-web\dist\laser-dmx-web\browser C:\Users\cleme\Documents\PlatformIO\Projects\240618-190049-esp32dev\data\ /C /S /Y /I /E

C:\Users\cleme\.platformio\penv\Scripts\platformio.exe run --target buildfs

C:\Users\cleme\.platformio\penv\Scripts\platformio.exe run --target uploadfs