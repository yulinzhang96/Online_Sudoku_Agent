# ======================================================================
# FILE:        Makefile
#
# AUTHOR:      Abdullah Younis
#
# DESCRIPTION: This file contains useful commands for this project. You
#              may use the following commands:
#
#              - make            - compiles the project and places
#                                  the executable in the bin folder
#
#              - make submission - creates the the submission, you will
#                                  submit.
#
#              - Don't make changes to this file.
# ======================================================================

RAW_SOURCES = \
	BTSolver.cpp\
	Constraint.cpp\
	ConstraintNetwork.cpp\
	Domain.cpp\
	Main.cpp\
	SudokuBoard.cpp\
	Trail.cpp\
	Variable.cpp

SOURCE_DIR = src
BIN_DIR = bin
SOURCES = $(foreach s, $(RAW_SOURCES), $(SOURCE_DIR)/$(s))

all: $(SOURCES)
	@rm -rf $(BIN_DIR)
	@mkdir -p $(BIN_DIR)
	@g++ -std=c++0x $(SOURCES) -o $(BIN_DIR)/Sudoku

submission: all
	@rm -f *.zip
	@echo ""
	@read -p "Enter Team Name (No spaces, '_', '/', '*'): " teamName; \
	 echo ""; \
	 zip -rqq s_$${teamName}.zip $(SOURCE_DIR) $(BIN_DIR)
