CC = gcc
CFlags = -g -Wall -pedantic
CFlagsLib = -c -g -Wall -pedantic
LDFLAGS = -lcunit
COVERAGE_FLAG = --coverage

SRC = ./src/
LIB = ./src/lib/
TEST = ./test/
DOC = ./doc/

Valgrind = valgrind --leak-check=full
LCOV = lcov -c -d ./ -o $@.info
GENHTML = genhtml $@.info -o $@_lcov

STD = $(CC) $(CFlags) $^ -o $@.out $(LDFLAGS)

define Coverage
$(CC) --coverage $(CFlags) $^ -o $@.out $(LDFLAGS)
./$@.out
$(LCOV)
$(GENHTML)
endef

.PHONY: doxygen clear valgrind_freq

tests: test_environment #add more test files as they are created
run_tests: test_environment
	./$<.out

run_%: %
	./$<.out

refmem: $(SRC)refmem.o environment.o
	$(STD)

environment: environment.o hash_table.lib.o linked_list.lib.o
	$(CC) $(CFlagsLib) $< -o $@

test_environment: $(TEST)environment_test.o environment.o hash_table.lib.o linked_list.lib.o
	$(STD)

valgrind_%: %
	$(Valgrind) ./$<.out

%.lib.o: $(LIB)%.c
	$(CC) $(CFlagsLib) $< -o $@

%.o: $(SRC)%.c
	$(CC) $(CFlagsLib) $<

%.cov.o: %.c
	$(CC) $(CFlagsLib) $(COVERAGE_FLAG) $< -o $@

doxygen_firefox: doxygen
	firefox $(DOC)doxygen/output/html/index.html

doxygen_chrome: doxygen
	google-chrome $(DOC)doxygen/output/html/index.html

doxygen:
	rm -rf $(DOC)doxygen
	mkdir  $(DOC)doxygen
	mkdir  $(DOC)doxygen/output
	doxygen

clean:	clean_src clean_lib clean_test clean_doc
	rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/
clean_src:
	cd $(SRC) && rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	cd $(SRC) && rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/

clean_lib:
	cd $(LIB) && rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	cd $(LIB) && rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/

clean_test:
	cd $(TEST) && rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	cd $(TEST) && rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/

clean_doc:
	cd $(DOC) && rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	cd $(DOC) && rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/
