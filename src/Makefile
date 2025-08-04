all: clean viewer

viewer: clean
	mkdir -p 3DViewer/build
	cp 3DViewer/settings.json ./3DViewer/build
	cd 3DViewer/build && cmake .. && $(MAKE)
	@clear
	@echo Use '"make run"' to start the program.

run:
	./3DViewer/build/3DViewer || ./install/3DViewer

install: all
	@mkdir -p install
	@cp 3DViewer/settings.json ./install
	@mv 3DViewer/build/3DViewer install

uninstall:
	@rm -rf install

dist: clean
	mkdir -p dist
	tar cvzf dist/3DViewer.tgz 3DViewer obj_models tests Makefile mainpage.dox Doxyfile

dvi:
	@doxygen Doxyfile
	@clear
	@echo "Done."
	@echo "Please open dvi/html/index.html manually."

clean:
	rm -rf 3DViewer/build/* 3DViewer/build/.[!.]*
	rm -rf tests/lib_build/.qt tests/lib_build/CMakeFiles tests/test_build/lib_build
	rm -f tests/lib_build/cmake_install.cmake tests/lib_build/CMakeCache.txt tests/lib_build/lib3DViewerBackend.a tests/lib_build/Makefile *.obj 
	rm -rf tests/test_build/.qt tests/test_build/CMakeFiles dist
	rm -f tests/test_build/cmake_install.cmake tests/test_build/CMakeCache.txt tests/test_build/lib3DViewerBackend.a tests/test_build/Makefile tests/test_build/3DViewerTests

tests: clean build_lib build_tests
	./tests/test_build/3DViewerTests
	@rm *.obj

valgrind: tests
	valgrind --tool=memcheck --leak-check=yes ./tests/test_build/3DViewerTests
	@rm *.obj

build_lib:
	cd tests/lib_build && cmake . && $(MAKE)

build_tests: build_lib
	cd tests/test_build && cmake . && $(MAKE)

clang:
	cp ../materials/linters/.clang-format ./
	clang-format -n */*/*.h */*/*.cpp
	clang-format -i */*/*.h */*/*.cpp

# # sudo apt install texlive-full
# # sudo apt-get install doxygen
