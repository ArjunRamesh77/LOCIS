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
PATH_LOCIS_CMD_OUT = $(LOCIS_ROOT)/Output
PATH_LOCIS_CMD_TEMP = $(LOCIS_ROOT)/Temp/cmdapp

# REQUIRED FOLDERS
REQUIRED_DIRS = $(PATH_LOCIS_CMD_OUT)/$(config)\
			    $(PATH_LOCIS_CMD_TEMP)/$(config)

_makeRequiredFolders := $(shell for d in $(REQUIRED_DIRS); \
						do								   \
	  					[[ -d $$d ]] || mkdir -p $$d;	   	   \
						done)

# PROGRAM
NAME_PROGRAM = locis 

# SOURCE PATHS
PATH_LOCIS_CMD_SRC = src

# SOURCE/INCLUDE/OBJ NAMES
LOCIS_CMD_INC = $(wildcard $(PATH_LOCIS_CMD_INCLUDE)/*.$(EXT_HEADER))
LOCIS_CMD_SRC = $(notdir $(wildcard $(PATH_LOCIS_CMD_SRC)/*.$(EXT_CPP)))
LOCIS_CMD_SRC_C = $(notdir $(wildcard $(PATH_LOCIS_CMD_SRC)/*.$(EXT_C)))
LOCIS_CMD_OBJ = $(addprefix $(PATH_LOCIS_CMD_TEMP)/$(config)/,$(patsubst %.$(EXT_CPP),%.$(EXT_OBJ),$(LOCIS_CMD_SRC)))
LOCIS_CMD_OBJ_C = $(addprefix $(PATH_LOCIS_CMD_TEMP)/$(config)/,$(patsubst %.$(EXT_C),%.$(EXT_OBJ),$(LOCIS_CMD_SRC_C)))

# FUNCTIONS
define cleanOutputs	
	@rm -f $(PATH_LOCIS_CMD_TEMP)/$1/*.$(EXT_OBJ);		
	@rm -f $(PATH_LOCIS_CMD_OUT)/$1/$(NAME_PROGRAM);	
endef

# TARGETS
$(PATH_LOCIS_CMD_OUT)/$(config)/$(NAME_PROGRAM): $(LOCIS_CMD_OBJ) $(LOCIS_CMD_OBJ_C)
	@$(CXX) $^ -o $@ $(PATH_ALL_LIB) $(LIB_ALL)

$(LOCIS_CMD_OBJ) :
$(PATH_LOCIS_CMD_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_CMD_SRC)/%.$(EXT_CPP) $(LOCIS_CMD_INC)
	@echo "Building $@"
	@$(CXX) $(CXXFLAGS) $(PATH_ALL_INCLUDE) $< -o $@ 

$(LOCIS_CMD_OBJ_C):
$(PATH_LOCIS_CMD_TEMP)/$(config)/%.$(EXT_OBJ) : $(PATH_LOCIS_CMD_SRC)/%.$(EXT_C) $(LOCIS_CMD_INC)
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

LIB_KINSOL := 	-lsundials_kinsol
LIB_IDA := 		-lsundials_ida
LIB_NVEC :=     -lsundials_nvecserial