# ----------------------------- #
#
# This Makefile is only used to simplify COMMON commands (for building, cleaning...).
# Actual Makefile is generated with cmake and is located in ./$(ODIR) directory.
#
# ----------------------------- #

EXE=graphics
CMAKE_OPTIONS=-DMINIFB_BUILD_EXAMPLES=OFF
ODIR=build

all:
	cd ./$(ODIR) && cmake $(CMAKE_OPTIONS) ..
	cd ./$(ODIR) && make

clean:
	cd ./$(ODIR) && make clean
	rm -rf ./$(ODIR)/*

run:
	cd ./$(ODIR) && make
	./$(ODIR)/$(EXE)
