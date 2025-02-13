clang-uml --progress -g mermaid

call mmdc --input build\clang_uml\class_inheritance.mmd --output build\clang_uml\class_inheritance.svg --configFile="mermaid_render_config.json" --cssFile "mermaid_render.css"
call mmdc --input build\clang_uml\imports_map.mmd       --output build\clang_uml\imports_map.svg       --configFile="mermaid_render_config.json" --cssFile "mermaid_render.css"

build\clang_uml\class_inheritance.svg
build\clang_uml\imports_map.svg

pause
