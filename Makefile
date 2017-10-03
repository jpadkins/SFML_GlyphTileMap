.SILENT:

CMAKE_DIR = build
EXEC = bin/sfmlproject

all: $(EXEC)

run: $(EXEC)
	$^

$(EXEC): $(wildcard src/*.cpp)
	cd $(CMAKE_DIR) && $(MAKE)

cmake: CMakeLists.txt
	cd $(CMAKE_DIR) && cmake ..

check: $(EXEC)
	valgrind --leak-check=full --suppressions=etc/valgrind/suppressions.supp $^

clean:
	rm -rf $(EXEC)
