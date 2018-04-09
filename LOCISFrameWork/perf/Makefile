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
PATH_LOCIS_PERF_TEMP = $(LOCIS_ROOT)/Temp/perf

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_PERF_OUT)/$(config)\
			    $(PATH_LOCIS_PERF_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = liblocisperf.a 

# SOURCE PATHS
PATH_LOCIS_PERF_SRC = src

# SOURCE/INCLUDE/OBJ NAMES
LOCIS_PERF_INC = $(wildcard $(PATH_LOCIS_PERF_INCLUDE)/*.$(EXT_HEADER))
LOCIS_PERF_SRC = $(notdir $(wildcard $(PATH_LOCIS_PERF_SRC)/*.$(EXT_CPP)))
LOCIS_PERF_OBJ = $(addprefix $(PATH_LOCIS_PERF_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_PERF_SRC)))

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