CC = gcc
CFlags = -g -Wall -pedantic
CFlagsLib = -c -g -Wall -pedantic
LDFLAGS = -lcunit
COVERAGE_FLAG = --coverage

SRC = ./src/
LIB = ./src/lib/
TEST = ./test/
DOC = ./doc/

Valgrind = valgrind --leak-check=full  # --track-origins=yes -v
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

tests: test_environment test_refmem #add more test files as they are created
run_tests: test_environment test_refmem
	./$<.out
	./test_refmem.out #TODO:

run_%: %
	./$<.out

refmem: $(SRC)refmem.o $(SRC)environment.o
	$(CC) $(CFlagsLib) $< -o $@

environment: $(SRC)environment.o $(LIB)hash_table.o $(LIB)linked_list.o
	$(STD)

test_environment: $(TEST)environment_test.o $(SRC)environment.o $(LIB)hash_table.o $(LIB)linked_list.o
	$(STD)

test_refmem: $(TEST)refmem_test.o  $(SRC)refmem.o $(SRC)environment.o $(LIB)hash_table.o $(LIB)linked_list.o
	$(STD)

coverage_test_environment: $(TEST)environment_test.cov.o $(SRC)environment.cov.o $(LIB)hash_table.cov.o $(LIB)linked_list.cov.o
	$(COVERAGE)

valgrind_%: %
	$(Valgrind) ./$<.out

%.lib.o: $(LIB)%.c
	$(CC) $(CFlagsLib) $< -o $@

%.o: $(SRC)%.c
	$(CC) $(CFlagsLib) $<

%.cov.o: %.c
	$(CC) $(CFlagsLib) $(COVERAGE_FLAG) $< -o $@

%.cov.o: $(SRC)%.c
	$(CC) $(CFlagsLib) $(COVERAGE_FLAG) $< -o $@

%.cov.o: $(LIB)%.c
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
	#cd $(DOC) && rm -f -r ./*.o ./*.out ./*.info ./*.gcda ./*.gcno ./*.data ./*.data.old ./*.h.gch ./*.cov ./*.out.*
	#cd $(DOC) && rm -f -r ./*.dSYM ./*.out.dSYM ./*_lcov/
