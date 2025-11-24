# ZIPTool

## An useful tool to extract zip file

## Technologies:

- C++20
- C++ STL
- zlib
- zip file structure
- Windows API

## Zip file structure:

- https://en.wikipedia.org/wiki/ZIP_%28file_format%29
- https://docs.fileformat.com/compression/zip/

## Usage:

```
ZIPTool.exe -x file.zip [-o output-path/]
```

Note:

- If you do not specify output path, ZIPTool will extract data on the parent folder of zip file
- Currently, it supports stored/deflate compression modes
