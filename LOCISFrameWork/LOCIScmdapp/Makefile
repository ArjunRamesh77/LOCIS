#------------------------------------------------------------------------------------------------
#
# Makefile to build locis (command line version)
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
PATH_LOCIS_CMD_OUT = $(LOCIS_ROOT)/Output
PATH_LOCIS_CMD_TEMP = $(LOCIS_ROOT)/Temp/LOCIScmdapp

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_CMD_OUT)/$(config)\
			    $(PATH_LOCIS_CMD_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = locis 

# INCLUDE PATHS
PATH_LOCIS_CMD_INCLUDE := include
PATH_LOCIS_INCLUDE :=	 $(LOCIS_ROOT)/LOCISFrameWork/LOCISCore/include
PATH_KINSOL_INCLUDE :=	 $(LOCIS_ROOT)/Solvers/KINSOL/srcdir/include
PATH_SUNDIALS_INCLUDE := $(LOCIS_ROOT)/Solvers/KINSOL/builddir/include
PATH_IDA_INCLUDE :=		 $(LOCIS_ROOT)/Solvers/IDA/srcdir/include
PATH_PYTHON_INCLUDE :=	 /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	 /home/$(USER)/.local/lib/python2.7/site-packages/numpy/core/include/numpy
PATH_LOCIS_FEM_INCLUDE := $(LOCIS_ROOT)/LOCISFrameWork/LocisFEM/include
PATH_LOCIS_PERF_INCLUDE := $(LOCIS_ROOT)/LOCISFrameWork/LOCISperf/include

PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_CMD_INCLUDE)\
					-I$(PATH_LOCIS_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)\
					-I$(PATH_LOCIS_FEM_INCLUDE)\
					-I$(PATH_LOCIS_PERF_INCLUDE)

# SOURCE PATHS
PATH_LOCIS_CMD_SRC = src

# LIB PATHS
PATH_KINSOL_LIB := 		$(LOCIS_ROOT)/Solvers/KINSOL/builddir/src/kinsol
PATH_IDA_LIB :=  		$(LOCIS_ROOT)/Solvers/IDA/builddir/src/ida
PATH_NVEC_SERIAL_LIB := $(LOCIS_ROOT)/Solvers/IDA/builddir/src/nvec_ser
PATH_PYTHON_LIB := 		/usr/lib/python2.7/config-x86_64-linux-gnu
PATH_LOCIS_LIBS := 		$(LOCIS_ROOT)/Output/$(config)

PATH_ALL_LIB = 		-L$(PATH_KINSOL_LIB)\
					-L$(PATH_IDA_LIB)\
					-L$(PATH_NVEC_SERIAL_LIB)\
					-L$(PATH_PYTHON_LIB)\
					-L$(PATH_LOCIS_LIBS)
# LIBS
LIB_LOCIS :=    -llocis
LIB_LOCIS_FEM := -llocisfem
LIB_KINSOL := 	-lsundials_kinsol
LIB_IDA := 		-lsundials_ida
LIB_NVEC :=     -lsundials_nvecserial
LIB_PYTHON := 	-lpython2.7
LIB_PERF :=     -llocisperf
     
LIB_ALL :=		$(LIB_LOCIS)\
				$(LIB_LOCIS_FEM)\
				$(LIB_KINSOL)\
				$(LIB_IDA)\
				$(LIB_NVEC)\
				$(LIB_PYTHON)\
				$(LIB_PERF)


# SOURCE/INCLUDE/OBJ NAMES
LOCIS_CMD_INC = $(wildcard $(PATH_LOCIS_CMD_INCLUDE)/*.$(EXT_HEADER))
LOCIS_CMD_SRC = $(notdir $(wildcard $(PATH_LOCIS_CMD_SRC)/*.$(EXT_CPP)))
LOCIS_CMD_OBJ = $(addprefix $(PATH_LOCIS_CMD_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_CMD_SRC)))

# COMPILER AND FLAGS
CXX := g++
ifeq ($(config),debug)
	CXXFLAGS = -Wall -c -std=c++11 -g -fpermissive
else
	CXXFLAGS = -Wall -c -std=c++11 -O3 -fpermissive
endif

# FUNCTIONS
define cleanOutputs	
	@rm -f $(PATH_LOCIS_CMD_TEMP)/$1/*.$(EXT_OBJ);		
	@rm -f $(PATH_LOCIS_CMD_OUT)/$1/$(NAME_PROGRAM);	
endef

# TARGETS
$(PATH_LOCIS_CMD_OUT)/$(config)/$(NAME_PROGRAM): $(LOCIS_CMD_OBJ) 
	@$(CXX) $^ -o $@ $(PATH_ALL_LIB) $(LIB_ALL)    

$(PATH_LOCIS_CMD_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_CMD_SRC)/%.$(EXT_CPP) $(LOCIS_CMD_INC)
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
