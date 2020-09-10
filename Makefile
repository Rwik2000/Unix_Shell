all:
	@echo "Use 'build' to compile, 'run' to execute and 'clean' to remove"
build:
	gcc -o shell.out shell.c
run:
	./shell.out
clean:
	rm shell.out