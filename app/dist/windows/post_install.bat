set SCRIPT_PATH=%~dp0
certmgr /add %SCRIPT_PATH%\russian_trusted_root_ca_pem.crt /s /r localmachine root
certmgr /add %SCRIPT_PATH%\russian_trusted_sub_ca_pem.crt /s /r localmachine root
