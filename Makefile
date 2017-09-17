#------------------------------------------------------------------------------------------------
#
# Makefile to build locis
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

# file extensions
EXT_CPP := cpp
EXT_OBJ := o
EXT_LIB := a

# include paths
PATH_LOCIS_INCLUDE = LOCISFrameWork\include
PATH_KINSOL_INCUDE = 
PATH_IDA_INCLUDE = 
PATH_PYTHON_INCLUDE = 

# src paths
PATH_LOCIS_SOURCE = LOCISFrameWork\src

# src/include
LOCIS_INC = ""
LOCIS_SRC = ""
include locisInc.mk
include locisSrc.mk

# lib paths
PATH_KINSOL_LIB = 
PATH_IDA_LIB = 
PATH_PYTHON_LIB = 

# libs
KINSOL_LIB = 
IDA_LIB = 
PYTHON_LIB = 

# compiler and flags
CXX := g++
ARCH := x86
COMPILE_OPTIONS_CPP = -Wall -c $(ARCH)
COMPILE_CPP = $(CXX) $(COMPILE_OPTIONS)
COMPILED_CPP = $(CXX) -g $(COMPILE_OPTIONS)

#FUNCTIONS
# Updates the include files with locis src file names
define getAllLocisCode
	@echo "Loading all include files"
	@ls $(PATH_LOCIS_INCLUDE) *.h | sed 's/\.h/\.h\\/g' | sed '1 s/^/LOCIS_INC = /g' > locisInc.mk

	@echo "Loading all src files"
	@ls $(PATH_LOCIS_INCLUDE) *.cpp | sed 's/\.cpp/\.cpp\\/g' | sed '1 s/^/LOCIS_SRC = /g' > locisSrc.mk
endef
				  
# RULES
release : 	

debug :

clean :

# Compile file
$(LOCIS_SRC) : %.$(EXT_OBJ) : %.$(EXT_CPP)



locisSrc.mk : 
	$(getAllLocisCode)





 