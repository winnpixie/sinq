all:
	@mkdir -p bin
	cc -O2 -x c -o bin/sinq src/*.c

clean:
	@rm -rf ./bin/