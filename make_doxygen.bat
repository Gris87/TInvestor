mkdir build

doxygen

if not "%1" == "--ci" (
    build\doxygen\html\index.html
)

pause
