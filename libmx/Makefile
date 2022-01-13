all: libmx.a

libmx.a: obj
	@ar rcs libmx.a obj/*.o

obj: src/*.c inc/*.h
	@mkdir -p obj
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic -c src/*.c -I inc
	@mv *.o obj

clean:
	@rm -rf obj

uninstall: clean
	@rm -f libmx.a

reinstall: uninstall all


