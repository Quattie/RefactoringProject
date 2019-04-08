compile:
	clang++ -std=c++17 -fsanitize=thread -o simulator simulator.cpp bank.cpp store.cpp

test:
	chmod +x test.sh
	./test.sh > output.txt

	




