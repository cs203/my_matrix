cc = clang++-21 -std=c++23 -g
file=/usr/lib/llvm-21/share/libc++/v1/std.cppm

test: matrix-support.o matrix-slice.o matrix.o  matrix-trait.o matrix-base.o main.o
	$(cc) -stdlib=libc++ $^ -o $@ 

matrix-base.o: matrix-base.pcm
	$(cc) $< -fprebuilt-module-path=. -c -o $@

matrix-base.pcm: matrix-base.cppm std.pcm matrix-slice.pcm
	$(cc) $< --precompile -fprebuilt-module-path=. -o $@

matrix-support.o: matrix-support.pcm 
	$(cc) $< -fprebuilt-module-path=. -c -o $@

matrix-support.pcm: matrix-support.cppm std.pcm matrix-trait.pcm
	$(cc) $< --precompile -fprebuilt-module-path=. -o $@

matrix-slice.o: matrix-slice.pcm 
	$(cc) $< -fprebuilt-module-path=. -c -o $@

matrix-slice.pcm: matrix-slice.cppm std.pcm matrix-support.pcm
	$(cc) $< --precompile -fprebuilt-module-path=. -o $@

matrix.o: matrix.pcm
		$(cc) $< -fprebuilt-module-path=. -c -o $@ 

matrix.pcm: matrix.cppm std.pcm matrix-base.pcm matrix-slice.pcm 
	$(cc) $<  --precompile -fprebuilt-module-path=. -o $@ 

matrix-trait.o: matrix-trait.pcm
	$(cc) $< -fprebuilt-module-path=. -c -o $@

matrix-trait.pcm: matrix-trait.cppm
	$(cc) $< --precompile -fprebuilt-module-path=. -o $@

std.pcm: $(file)
	$(cc) -stdlib=libc++ -Wno-reserved-module-identifier --precompile $(file) -o std.pcm 

main.o: main.cpp matrix.pcm 
	$(cc) main.cpp -fprebuilt-module-path=. -c -o main.o

run:
	./test

clean:
	rm -f *.o test matrix.pcm matrix-slice.pcm std.pcm

.PHONY: run clean
