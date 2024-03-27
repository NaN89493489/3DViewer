FLAGS = -lm -lsubunit -lrt -lpthread 
CFLAGS =gcc

ifeq ($(shell uname -s), Linux)
	LFLAG += -lpthread -lrt -lm -lsubunit
endif

all: function_C.a

function_C.a:
	$(CFLAGS) -c -g ./lib_c/s21_backend.c
	ar rc s21_backend.a s21_backend.o
	ranlib s21_backend.a

test: clean function_C.a
	$(CFLAGS) -g ./Tests/*.c -o test s21_backend.a -lcheck $(LFLAG)
	./test

leaks: test
	valgrind -s --leak-check=full --track-origins=yes  ./test


gcov_report: clean

	$(CFLAGS) lib_c/*.c ./Tests/*.c -lcheck -o gcov_report --coverage $(LFLAG)
	./gcov_report
	rm -rf *test*.gc* 
	gcov *.c 
	lcov -d . -o gcov_report.info -c
	genhtml -o report gcov_report.info
	open report/index.html

style:
	clang-format -style=Google -i ./lib_c/*.c ./lib_c/*.h
	clang-format -style=Google -n ./lib_c/*.c ./lib_c/*.h
	clang-format -style=Google -i ./Tests/*.c ./Tests/*.h
	clang-format -style=Google -n ./Tests/*.c ./Tests/*.h
	clang-format -style=Google -i ./Qt_file/*.cpp ./Qt_file/*.h
	clang-format -style=Google -n ./Qt_file/*.cpp ./Qt_file/*.h

rebuild: clean all


install: clean
	mkdir Viewer || true
	cd Qt_file && qmake && make &&make clean &&rm Makefile &&  cd .. 
	mv Qt_file/3D_Viewer Viewer 

uninstall:
	rm -rf Viewer


dvi: clean
	cd doc && latex readme.tex && evince readme.dvi || (echo "Can't open preview, needs to install an app"; exit 1)



dist: install
	mkdir -p Viewer-1.0/src
	cp -r Configs Viewer-1.0/src
	cp -r doc Viewer-1.0/src
	cp -r lib_c Viewer-1.0/src
	cp Makefile Viewer-1.0/src
	cp -r Obj_files Viewer-1.0/src
	cp -r Qt_file Viewer-1.0/src
	cp -r Tests Viewer-1.0/src
	cp -r Viewer Viewer-1.0/src
	cp -r doc Viewer-1.0/src
	tar cvzf Viewer-1.0.tgz Viewer-1.0 > /dev/null
	rm -rf Viewer-1.0 


clean:
	rm -f *.gcno *.gcda *.info *.gcov *.cpp *.h
	rm -f *.o *.a
	rm -rf report *.dSYM
	rm -f *.out
	rm -rf test
	rm -f gcov_report
	rm -rf Viewer-1.0.tgz
	rm -rf calc
	rm -rf doc/readme.aux doc/readme.dvi doc/readme.log


rename_to_obj:
	mv ./Obj_files/Car.txt ./Obj_files/Car.obj
	mv ./Obj_files/cube.txt ./Obj_files/cube.obj
	mv ./Obj_files/Dog.txt ./Obj_files/Dog.obj
	mv ./Obj_files/empty.txt ./Obj_files/empty.obj
	mv ./Obj_files/MapleTree.txt ./Obj_files/MapleTree.obj
	mv ./Obj_files/octahedron.txt ./Obj_files/octahedron.obj
		mv ./Obj_files/Guitar.txt ./Obj_files/Guitar.obj

rename_to_txt:
	mv ./Obj_files/Car.obj ./Obj_files/Car.txt
	mv ./Obj_files/cube.obj ./Obj_files/cube.txt
	mv ./Obj_files/Dog.obj ./Obj_files/Dog.txt
	mv ./Obj_files/empty.obj ./Obj_files/empty.txt
	mv ./Obj_files/MapleTree.obj ./Obj_files/MapleTree.txt
	mv ./Obj_files/octahedron.obj ./Obj_files/octahedron.txt
	mv ./Obj_files/Guitar.obj ./Obj_files/Guitar.txt