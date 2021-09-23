build_stat: dist/mainfano.o dist/libmylib_stat.a
	gcc -lm -o compressor dist/mainfano.o dist/libmylib_stat.a -I.

dist/mainfano.o: src/mainfano.c
	gcc -c -o dist/mainfano.o src/mainfano.c

dist/io.o: src/io.c src/io.h
	gcc -c -o dist/io.o src/io.c

dist/newfano.o: src/newfano.c src/newfano.h
	gcc -c -o dist/newfano.o src/newfano.c

dist/libmylib_stat.a: dist/io.o dist/newfano.o
	ar rc dist/libmylib_stat.a dist/io.o dist/newfano.o

libs: dist/libmylib_stat.a

clean: 
	rm -f dist/*.o dist/*.a compressor