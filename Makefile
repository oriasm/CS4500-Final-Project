test:
	cd tests; ./personal_test_suite

build:
	cd tests; cmake .; make ;

valgrind:
	cd tests; valgrind ./personal_test_suite

clean:
	cd tests; make clean;