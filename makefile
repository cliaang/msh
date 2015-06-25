mshell: mshell.h mshell.c print_prompt.c read_command.c builtin_command.c parse.c
	gcc mshell.c print_prompt.c read_command.c builtin_command.c parse.c -o msh -g
test: mshell.h test.c read_command.c parse.c
	gcc test.c read_command.c parse.c -g -o test
