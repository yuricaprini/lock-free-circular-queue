all: clean main.cpp Makefile
	$(CXX) main.cpp -o main

run: all
	./main

test: run
	diff consumer.txt producer.txt

clean: 
	rm -f main *.txt