test:
	cd tests; cmake .;make ; ./personal_test_suite

build:
	cd tests; cmake .; make ;

valgrind:
	cd tests; cmake .; make ; valgrind ./personal_test_suite

clean:
	cd tests; make clean;