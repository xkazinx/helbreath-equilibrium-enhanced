REG ADD "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" /v %cd%\Client.exe /t REG_SZ /d "DWM8And16BitMitigation 16BITCOLOR Layer_ForceDirectDrawEmulation" /f 

