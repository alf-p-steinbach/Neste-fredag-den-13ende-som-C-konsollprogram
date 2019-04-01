## Noter for bygging av C++ konsollprogrammet.

Denne C++koden bruker [*cpp-core*-mikrobiblioteket](https://github.com/alf-p-steinbach/cppx-core), som i sin tur bruker et enda mer mikroskopisk lite bibliotek kalt [*C header wrappers*](https://github.com/alf-p-steinbach/C-header-wrappers). Begge disse er rene header-biblioteker, som betyr at de ikke behøver å bygges, men *cpp-core* krever C++17 eller senere. Med g++ betyr det å bruke en kommando à la

> ***`g++ -std=c++ -I. main.cpp -o f13`***


mens med Visual C++ er det mer krøkkete å få kompilatoren standard-konform:

> [E:\root\writings\articles\technical\norwegian\kode-24\fredag-13\app\c++]  
> \> ***`cl /I. main.cpp`***  
> main.cpp  
> c:\my\dev\include\cppx-core/assert-c++17-or-later.hpp(3): error C2338: Requires C++17 or later. For MSVC use e.g. '/std:c++17 /Zc:__cplusplus /utf-8'.  
> c:\my\dev\include\cppx-core/assert-c++17-or-later.hpp(7): fatal error C1083: Cannot open include file: 'C++17-or-later-is-required': No such file or directory  
>  
> [E:\root\writings\articles\technical\norwegian\kode-24\fredag-13\app\c++]  
> `>` ***`cl /std:c++17 /Zc:__cplusplus /utf-8 /I. main.cpp /Fe"f13"`***  
> main.cpp
>  
> [E:\root\writings\articles\technical\norwegian\kode-24\fredag-13\app\c++]  
> \> ***`f13`***  
> 165 dager etter 1. april 2019 kommer fredag 13. september... :-o
