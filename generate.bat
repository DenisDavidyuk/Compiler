rem Options from Code::Blocks
rem flex -o$file_dir/$file_name.scanner.c $file
rem bison -v -d $file -o $file_dir/$file_name.parser.c
flex -oc.scanner.c c.l
bison -d c.y -o c.parser.c
pause