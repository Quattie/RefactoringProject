compile:
	clang++ -std=c++17 -fsanitize=thread -o simulator simulator.cpp

test:
	chmod +x test.sh
	./test.sh > output.txt

	




