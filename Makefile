test:
	cd tests; cmake .; make ; ./personal_test_suite

build:
	cd src; g++ -std=c++11 -Wall -g application/milestone1.cpp

valgrind:
	cd tests; valgrind --leak-check ./personal_test_suite

clean:
	cd tests; make clean;