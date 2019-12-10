CFLAGS = -pedantic -Wall -g -DASSERT_BOUNDS
GCOVFLAGS = --coverage
GPROFFLAGS = -pg


freq_count: freq_count.c hash_table.o linked_list.o
	gcc $(CFLAGS) freq_count.c hash_table.o linked_list.o -o freq_count

freq_count_gprof: freq_count.c hash_table_gprof.o linked_list_gprof.o
	gcc $(CFLAGS) $(GPROFFLAGS) freq_count.c hash_table_gprof.o linked_list_gprof.o -o freq_count_gprof


libs: hash_table.o linked_list.o


test: hash_table_tests linked_list_tests
	valgrind --leak-check=full ./hash_table_tests
	valgrind --leak-check=full ./linked_list_tests

hash_table_tests: hash_table.o hash_table_tests.c linked_list.o
	gcc $(CFLAGS) -lcunit hash_table_tests.c hash_table.o linked_list.o -o hash_table_tests

linked_list_tests: linked_list.o linked_list_tests.c
	gcc $(CFLAGS) -lcunit linked_list_tests.c linked_list.o -o linked_list_tests



test_gcov: hash_table_tests_gcov linked_list_tests_gcov
	valgrind --leak-check=full ./hash_table_tests_gcov
	valgrind --leak-check=full ./linked_list_tests_gcov
	gcov -b -c hash_table_gcov.o
	gcov -b -c linked_list_gcov.o

hash_table_tests_gcov: hash_table_gcov.o linked_list_gcov.o hash_table_tests.c
	gcc $(CFLAGS) $(GCOVFLAGS) -lcunit hash_table_tests.c hash_table_gcov.o linked_list_gcov.o -o hash_table_tests_gcov

linked_list_tests_gcov: linked_list_gcov.o linked_list_tests.c
	gcc $(CFLAGS) $(GCOVFLAGS) -lcunit linked_list_tests.c linked_list_gcov.o -o linked_list_tests_gcov



hash_table.o: hash_table.c
	gcc $(CFLAGS) -c hash_table.c

linked_list.o: linked_list.c
	gcc $(CFLAGS) -c linked_list.c



hash_table_gcov.o: hash_table.c
	gcc $(CFLAGS) $(GCOVFLAGS) -c hash_table.c -o hash_table_gcov.o

linked_list_gcov.o: linked_list.c
	gcc $(CFLAGS) $(GCOVFLAGS) -c linked_list.c -o linked_list_gcov.o



hash_table_gprof.o: hash_table.c
	gcc $(CFLAGS) $(GPROFFLAGS) -c hash_table.c -o hash_table_gprof.o

linked_list_gprof.o: linked_list.c
	gcc $(CFLAGS) $(GPROFFLAGS) -c linked_list.c -o linked_list_gprof.o



clean:
	rm -f *.o *.gcda *.gcov *.gcno gmon.out hash_table_tests linked_list_tests hash_table_tests_gcov linked_list_tests_gcov freq_count freq_count_gprof
