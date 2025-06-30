cd build\Desktop-Debug\test\build

tests.exe > output.txt 2>&1
set RESULT_CODE=%ERRORLEVEL%

if %RESULT_CODE% NEQ 0 (
    type output.txt
    del output.txt

    cd ..\..\..\..

    if not "%1" == "--ci" pause

    exit /b %RESULT_CODE%
)

del output.txt

CALL :try
set RESULT_CODE=%ERRORLEVEL%

if not "%1" == "--ci" ..\..\..\CoverageReport\index.html
cd ..\..\..\..

exit /b %RESULT_CODE%

:try
SET /A tries=5

:loop
IF %tries% LEQ 0 EXIT /B 1
SET /A tries-=1

if exist ..\..\..\CoverageReport rmdir ..\..\..\CoverageReport /q /s
OpenCppCoverage ^
    --sources app\src\ ^
    --excluded_sources test_* ^
    --excluded_sources *_mock.h ^
    --excluded_sources filedialog.cpp ^
    --excluded_sources messageboxutils.cpp ^
    --excluded_sources httpclient.cpp ^
    --export_type html:..\..\..\CoverageReport ^
    --export_type cobertura:..\..\..\CoverageReport\cobertura.xml ^
    -- ^
    tests.exe && (EXIT /B 0) || (GOTO loop)
