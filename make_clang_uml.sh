#!/bin/bash

clang-uml --progress -g mermaid

mmdc --input build/clang_uml/class_inheritance.mmd --output build/clang_uml/class_inheritance.svg --configFile="mermaid_render_config.json" --cssFile "mermaid_render.css"
mmdc --input build/clang_uml/imports_map.mmd       --output build/clang_uml/imports_map.svg       --configFile="mermaid_render_config.json" --cssFile "mermaid_render.css"

firefox build/clang_uml/class_inheritance.svg > /dev/null 2>&1 &
firefox build/clang_uml/imports_map.svg > /dev/null 2>&1 &
