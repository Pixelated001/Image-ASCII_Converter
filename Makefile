# Source based on Eike Anderson starts here; originates from Lab 8 task 5 compiles the program including SDL headers
ASCII_Converter.exe: main.c
	clang main.c -l SDL2 -l SDL2main -l SDL2_image -l SDL2_ttf -l shell32 -o ASCII_Converter.exe -Xlinker /subsystem:console 
# Source from Eike Anderson ends here