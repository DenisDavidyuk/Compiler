const char * prolog = "\
push ebp\n\
mov ebp, esp\n\
";

const char * epilog = "\n\
mov esp, ebp\n\
pop ebp\n\
ret 0\n\
";

const char * tpl = "\
.686\n\
.model flat\n\
includelib msvcrt.lib\n\
extrn __imp__printf: proc\n\
extrn __imp__scanf: proc\n\
extrn __imp__getchar: proc\n\
extrn __imp__malloc: proc\n\
printf EQU dword ptr __imp__printf\n\
scanf EQU dword ptr __imp__scanf\n\
getchar EQU dword ptr __imp__getchar\n\
malloc EQU dword ptr __imp__malloc\n\
";
