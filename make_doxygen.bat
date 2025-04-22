mkdir build

doxygen
set RESULT_CODE=%ERRORLEVEL%

if not "%1" == "--ci" (
    build\doxygen\html\index.html

    pause
)

exit /b %RESULT_CODE%
