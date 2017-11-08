#------------------------------------------------------------------------------------------------
#
# Makefile to build liblocisfem.a
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

# FILE EXTENSIONS
EXT_CPP := cpp
EXT_C := c
EXT_OBJ := o
EXT_LIB := a
EXT_HEADER := h

# DEFAULT CONFIGURATION
ifeq ($(config),)
	config=debug
endif

#ROOT
LOCIS_ROOT = ../../

# OUTPUT PATHS
PATH_LOCIS_FEM_OUT =  $(LOCIS_ROOT)/Output
PATH_LOCIS_FEM_TEMP = $(LOCIS_ROOT)/Temp/LOCISFem

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_FEM_OUT)/$(config)\
			    $(PATH_LOCIS_FEM_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = liblocisfem.a 

# ALL INCLUDE PATHS
PATH_LOCIS_FEM_INCLUDE := include
PATH_KINSOL_INCLUDE :=	 $(LOCIS_ROOT)/Solvers/KINSOL/srcdir/include
PATH_SUNDIALS_INCLUDE := $(LOCIS_ROOT)/Solvers/KINSOL/builddir/include
PATH_IDA_INCLUDE :=		 $(LOCIS_ROOT)/Solvers/IDA/srcdir/include
PATH_PYTHON_INCLUDE :=	 /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	 /home/$(USER)/.local/lib/python2.7/site-packages/numpy/core/include/numpy
PATH_LOCIS_INCLUDE := $(LOCIS_ROOT)/LOCISFrameWork/LOCISCore/include

PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_FEM_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)\
					-I$(PATH_LOCIS_INCLUDE)

# SOURCE PATHS
PATH_LOCIS_FEM_SRC = src

# SOURCE/INCLUDE/OBJ NAMES
LOCIS_FEM_INC = $(wildcard $(PATH_LOCIS_FEM_INCLUDE)/*.$(EXT_HEADER))
LOCIS_FEM_SRC = $(notdir $(wildcard $(PATH_LOCIS_FEM_SRC)/*.$(EXT_CPP)))
LOCIS_FEM_SRC_C = $(notdir $(wildcard $(PATH_LOCIS_FEM_SRC)/*.$(EXT_C)))
LOCIS_FEM_OBJ = $(addprefix $(PATH_LOCIS_FEM_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_FEM_SRC)))
LOCIS_FEM_OBJ_C = $(addprefix $(PATH_LOCIS_FEM_TEMP)/$(config)/,$(patsubst %.$(EXT_C),%.$(EXT_OBJ),$(LOCIS_FEM_SRC_C)))

# COMPILER AND FLAGS
AR := ar
CXX := g++
CC := gcc
ARFLAGS = rcs
ifeq ($(config),debug)
	CXXFLAGS = -Wall -c -std=c++11 -g -fpermissive
	CCFLAGS = -Wall -c -g
else
	CXXFLAGS = -Wall -c -std=c++11 -O3 -fpermissive
	CCFLAGS = -Wall -c 
endif

# FUNCTIONS
define cleanOutputs	
	@rm -f $(PATH_LOCIS_FEM_TEMP)/$1/*.$(EXT_OBJ);		
	@rm -f $(PATH_LOCIS_FEM_OUT)/$1/$(NAME_PROGRAM);	
endef

# TARGETS
$(PATH_LOCIS_FEM_OUT)/$(config)/$(NAME_PROGRAM): $(LOCIS_FEM_OBJ) $(LOCIS_FEM_OBJ_C)
	@$(AR) $(ARFLAGS) $@ $^   

$(LOCIS_FEM_OBJ):
$(PATH_LOCIS_FEM_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_FEM_SRC)/%.$(EXT_CPP) $(LOCIS_FEM_INC)
	@echo "Building $@"
	@$(CXX) $(CXXFLAGS) $(PATH_ALL_INCLUDE) $< -o $@ 

$(LOCIS_FEM_OBJ_C):
$(PATH_LOCIS_FEM_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_FEM_SRC)/%.$(EXT_C) $(LOCIS_FEM_INC)
	@echo "Building $@"
	@$(CC) $(CCFLAGS) $(PATH_ALL_INCLUDE) $< -o $@ 

.PHONY : cleand
cleand :
	@$(call cleanOutputs,debug)
	
.PHONY : cleanr
cleanr :
	@$(call cleanOutputs,release)

.PHONY : clean
clean : cleand cleanr
