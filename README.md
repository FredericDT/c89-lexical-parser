reference https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf

# c89_lexical_parser

## Build

```bash
git clone https://github.com/FredericDT/c89-lexical-parser.git
cd c89-lexical-parser
mkdir build
cd build
cmake ..
cmake --build .
```

## `src/c89_lexical_parser`

### arguments

|argument|comment|
|---|---|
|h, help|print help|
|f, file|required, provide input file path|
|o, output|optional, provide output file path, if absent, output will be printed to stdout|
|v, verbose|optional, print debug information to stdout|
|a, analysis|optional, print analytical information to stdout|
|c, comments|optional, result includes comments if this argument presents|