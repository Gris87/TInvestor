set SCRIPT_PATH=%~dp0
certmgr /add %SCRIPT_PATH%\tbank.ru.crt /s /r localmachine root
