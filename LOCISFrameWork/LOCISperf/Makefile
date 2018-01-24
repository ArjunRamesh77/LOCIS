#------------------------------------------------------------------------------------------------
#
# Makefile to build liblocisperf.a
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

# FILE EXTENSIONS
EXT_CPP := cpp
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
PATH_LOCIS_PERF_OUT = $(LOCIS_ROOT)/Output
PATH_LOCIS_PERF_TEMP = $(LOCIS_ROOT)/Temp/LOCISperf

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_PERF_OUT)/$(config)\
			    $(PATH_LOCIS_PERF_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = liblocisperf.a 

# ALL INCLUDE PATHS
PATH_LOCIS_API_INCLUDE :=	$(LOCIS_ROOT)/LOCISFrameWork/LOCISapi/include
PATH_LOCIS_INCLUDE :=	 $(LOCIS_ROOT)/LOCISFrameWork/LOCISCore/include
PATH_KINSOL_INCLUDE :=	 $(LOCIS_ROOT)/Solvers/KINSOL/srcdir/include
PATH_SUNDIALS_INCLUDE := $(LOCIS_ROOT)/Solvers/KINSOL/builddir/include
PATH_IDA_INCLUDE :=		 $(LOCIS_ROOT)/Solvers/IDA/srcdir/include
PATH_PYTHON_INCLUDE :=	 /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	 /home/$(USER)/.local/lib/python2.7/site-packages/numpy/core/include/numpy
PATH_LOCIS_FEM_INCLUDE := $(LOCIS_ROOT)/LOCISFrameWork/LocisFEM/include
PATH_LCOIS_PERF_INCLUDE := include

PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_API_INCLUDE)\
					-I$(PATH_LOCIS_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)\
					-I$(PATH_LOCIS_FEM_INCLUDE)\
					-I$(PATH_LCOIS_PERF_INCLUDE)

# SOURCE PATHS
PATH_LOCIS_PERF_SRC = src

# SOURCE/INCLUDE/OBJ NAMES
LOCIS_PERF_INC = $(wildcard $(PATH_LOCIS_PERF_INCLUDE)/*.$(EXT_HEADER))
LOCIS_PERF_SRC = $(notdir $(wildcard $(PATH_LOCIS_PERF_SRC)/*.$(EXT_CPP)))
LOCIS_PERF_OBJ = $(addprefix $(PATH_LOCIS_PERF_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_PERF_SRC)))

# COMPILER AND FLAGS
AR := ar
CXX := g++
ARFLAGS = rcs

ifeq ($(config),debug)
	CXXFLAGS = -Wall $(shared) -c -std=c++11 -g -fpermissive
else
	CXXFLAGS = -Wall $(shared) -c -std=c++11 -O3 -fpermissive
endif

# FUNCTIONS
define cleanOutputs	
	@rm -f $(PATH_LOCIS_PERF_TEMP)/$1/*;		
	@rm -f $(PATH_LOCIS_PERF_OUT)/$1/$(NAME_PROGRAM);	
endef

# TARGETS
$(PATH_LOCIS_PERF_OUT)/$(config)/$(NAME_PROGRAM): $(LOCIS_PERF_OBJ)
	@$(AR) $(ARFLAGS) $@ $^   

$(PATH_LOCIS_PERF_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_PERF_SRC)/%.$(EXT_CPP) $(LOCIS_PERF_INC)
	@echo "Building $@"
	@$(CXX) $(CXXFLAGS) $(PATH_ALL_INCLUDE) $< -o $@ 

.PHONY : cleand
cleand :
	@$(call cleanOutputs,debug)
	
.PHONY : cleanr
cleanr :
	@$(call cleanOutputs,release)

.PHONY : clean
clean : cleand cleanr