clang-uml --progress -g mermaid

call mmdc -i build\clang_uml\class_inheritance.mmd -o build\clang_uml\class_inheritance.svg
call mmdc -i build\clang_uml\imports_map.mmd -o build\clang_uml\imports_map.svg

build\clang_uml\class_inheritance.svg
build\clang_uml\imports_map.svg

pause
