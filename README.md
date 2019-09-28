# Byte Sudoku

<p  align="center"><a href="https://github.com/alexandre-lavoie/byte-sudoku"><img  src="https://github.com/alexandre-lavoie/byte-sudoku/raw/master/images/logo.png?raw=true" width="72" height="72"></a></p>

<p  align="center">
    Sudoku Optimized to the Byte
</p>

## Table of contents

- [Overview](#overview)
-  [Getting Started](#getting-started)
- [Usage](#usage)

## Overview

Byte Sudoku is a C++ implementation of sudoku. It uses byte manipulations for optimal calculations and storage. The program is lightweight and very easy to compile in non-Windows operating system. The current version includes a solver, but a generator could be added in the future.

## Getting Started

### Required

- [Visual Studio Code](https://code.visualstudio.com/)
- [MinGW](http://www.mingw.org/)

### Configure

[MinGW](http://www.mingw.org/) must be part of your PATH. Edit the c_cpp_properties.json and launch.json to reflect the location of GCC and GDB.

### Debug

Use the shortcut F5 in the source code opened in [Visual Studio Code](https://code.visualstudio.com/).

### Build

Use the shortcut CTRL+SHIFT+B in the source code opened in [Visual Studio Code](https://code.visualstudio.com/). 

### Sample Board

The repository provides example sudoku files for 2x2, 3x3, and 4x4. You can write a sudoku using this format.

```
2◦⏎
0◦2◦4◦0◦⏎
1◦0◦0◦3◦⏎
4◦0◦0◦2◦⏎
0◦1◦3◦0◦⏎
```
## Usage

Using a compiled version of the code, you can run the solver.

```
bytesudoku FILENAME [OUTPUT]
```