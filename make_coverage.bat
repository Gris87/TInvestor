cd build

Desktop-Debug\test\build\tests.exe > output.txt 2>&1

if %ERRORLEVEL% EQU 0 (
    OpenCppCoverage --sources TInvestor\app\src --excluded_sources test_*. --export_type html:CoverageReport -- Desktop-Debug\test\build\tests.exe
    CoverageReport\index.html
) else (
    type output.txt
    pause
)

del output.txt

cd ..
