#------------------------------------------------------------------------------------------------
#
# Makefile to build locis
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

# FILE EXTENSIONS
EXT_CPP := cpp
EXT_OBJ := o
EXT_LIB := a

# PROGRAM
NAME_PROGRAM = locis 

# INCLUDE PATHS
PATH_ROOT = $(CURDIR)
PATH_LOCIS_INCLUDE := 				LOCISFrameWork/LOCISFrameWork/include/
PATH_SUNDIALS_NVECTOR_INCLUDE := 	Solvers/KINSOL/kinsol_src/srcdir/include/nvector
PATH_KINSOL_INCLUDE := 				Solvers/KINSOL/kinsol_src/srcdir/include/kinsol
PATH_IDA_INCLUDE := 				Solvers/IDA/IDA_src/srcdir/include/ida
PATH_PYTHON_INCLUDE := 				/usr/include/python2.7

PATH_ALL_INCLUDE =  $(PATH_LOCIS_INCLUDE)\
					$(PATH_SUNDIALS_NVECTOR_INCLUDE)\
					$(PATH_KINSOL_INCLUDE)\
					$(PATH_IDA_INCLUDE)\
					$(PATH_PYTHON_INCLUDE)

# OUTPUT PATHS
PATH_LOCIS_OUT = Output/LOCISFrameWork

# TEMP PATHS
PATH_LOCIS_TEMP = Temp/LOCISFrameWork

# SOURCE PATHS
PATH_LOCIS_SRC = LOCISFrameWork/LOCISFrameWork/src

# LIB PATHS
PATH_KINSOL_LIB := 	Solvers/KINSOL/kinsol_src/builddir/src/kinsol
PATH_IDA_LIB :=  	Solvers/IDA/IDA_src/builddir/src/ida
PATH_PYTHON_LIB := 	/usr/lib/python2.7/config-x86_64-linux-gnu

PATH_ALL_LIB = 		$(PATH_KINSOL_LIB)\
					$(PATH_IDA_LIB)\
					$(PATH_PYTHON_LIB)
# LIBS
LIB_KINSOL := 	-lsundials_kinsol
LIB_IDA := 		-lsundials_ida
LIB_PYTHON := 	-lpython2.7

LIB_ALL := 		$(LIB_KINSOL)\
				$(LIB_IDA)\
				$(LIB_PYTHON)

# SOURCE/INCLUDE NAMES
LOCIS_INC = ""
LOCIS_SRC = ""
LOCIS_OBJ = ""

include $(PATH_LOCIS_TEMP)/locisInc.mk

include $(PATH_LOCIS_TEMP)/locisSrc.mk

LOCIS_OBJ = $(subst .cpp,.o,$(LOCIS_SRC))

# TRANSFORM 
LOCIS_INC_SUBS = $(subst ~,$(PATH_LOCIS_INCLUDE)/,$(LOCIS_INC))
LOCIS_SRC_SUBS = $(subst ~,$(PATH_LOCIS_SRC)/,$(LOCIS_SRC))
LOCIS_OBJ_SUBS = $(subst ~,$(PATH_LOCIS_TEMP)/,$(LOCIS_OBJ))

# COMPILER AND FLAGS
CXX := g++
COMPILE_INCLUDE_PATHS = -I$(PATH_ALL_INCLUDE)
COMPILE_LIB_PATHS = -L$(PATH_ALL_LIB)
COMPILE_OPTIONS = -Wall -c 

# FUNCTIONS
				  
# TARGETS
# default goal
$(PATH_LOCIS_OUT)/$(NAME_PROGRAM): $(LOCIS_OBJ_SUBS) $(LIB_ALL) $(LOCIS_INC_SUBS)
	$(CXX) $(COMPILE_LIB_PATHS) $^ -o $@

$(PATH_LOCIS_TEMP)/%.o : $(PATH_LOCIS_SRC)/%.cpp
	$(CXX) $(COMPILE_OPTIONS) $(COMPILE_INCLUDE_PATHS) $< -o $(PATH_LOCIS_TEMP)/$@

# Get all include files
$(PATH_LOCIS_TEMP)/locisInc.mk :
	ls $(PATH_LOCIS_INCLUDE) | sed 's/^/~/g' | sed 's/\.h/\.h\\/g' | sed '1 s/^/LOCIS_INC = /g' > $(PATH_LOCIS_TEMP)/locisInc.mk

# Get all source files
$(PATH_LOCIS_TEMP)/locisSrc.mk :
	ls $(PATH_LOCIS_SRC) | sed 's/^/~/g' | sed 's/\.cpp/\.cpp\\/g' | sed '1 s/^/LOCIS_SRC = /g' > $(PATH_LOCIS_TEMP)/locisSrc.mk

.PHONY : clean
clean :
	rm -f $(PATH_LOCIS_TEMP)/*; \
	rm -f $(PATH_LOCIS_OUT)/*
