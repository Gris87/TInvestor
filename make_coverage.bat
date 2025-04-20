cd build

set SOURCES=TInvestor-source\app\src
if not "%1" == "--ci" set SOURCES=TInvestor\app\src

Desktop-Debug\test\build\tests.exe > output.txt 2>&1
set RESULT_CODE=%ERRORLEVEL%

if %RESULT_CODE% EQU 0 (
    OpenCppCoverage ^
        --sources %SOURCES% ^
        --excluded_sources test_* ^
        --excluded_sources messagebox.cpp ^
        --excluded_sources httpclient.cpp ^
        --export_type html:CoverageReport ^
        -- ^
        Desktop-Debug\test\build\tests.exe
    set RESULT_CODE=%ERRORLEVEL%

    if not "%1" == "--ci" CoverageReport\index.html
) else (
    type output.txt
    if not "%1" == "--ci" pause
)

del output.txt

cd ..

exit /b %RESULT_CODE%
