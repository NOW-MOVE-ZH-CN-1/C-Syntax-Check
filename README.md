# C-Syntax-Check
Simple fatal compile-error static checker written in C++.
No class, pure function implementation.
Text level scan ONLY, NO style lint, NO AST, NO compiler invocation.

Detect fatal CE:
- Unclosed block comment /* */
- Mismatched braces {}
- Mismatched parentheses ()
- Missing semicolon (simple heuristic)

## Build & Run
```bash
mkdir build && cd build
cmake ..
make
./cpp-lint-mini ../test/bad_code.cpp
