#------------------------------------------------------------------------------------------------
#
# Makefile to build liblocis.a
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
PATH_LOCIS_OUT = $(LOCIS_ROOT)/Output
PATH_LOCIS_TEMP = $(LOCIS_ROOT)/Temp/core

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_OUT)/$(config)\
			    $(PATH_LOCIS_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = liblocis.a 

# SOURCE PATHS
PATH_LOCIS_SRC = src

# SOURCE/INCLUDE/OBJ NAMES
LOCIS_INC = $(wildcard $(PATH_LOCIS_INCLUDE)/*.$(EXT_HEADER))
LOCIS_SRC = $(notdir $(wildcard $(PATH_LOCIS_SRC)/*.$(EXT_CPP)))
LOCIS_OBJ = $(addprefix $(PATH_LOCIS_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_SRC)))

# FUNCTIONS
define cleanOutputs	
	@rm -f $(PATH_LOCIS_TEMP)/$1/*;		
	@rm -f $(PATH_LOCIS_OUT)/$1/$(NAME_PROGRAM);	
endef

# TARGETS
$(PATH_LOCIS_OUT)/$(config)/$(NAME_PROGRAM): $(LOCIS_OBJ)
	@$(AR) $(ARFLAGS) $@ $^   

$(PATH_LOCIS_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_SRC)/%.$(EXT_CPP) $(LOCIS_INC)
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
