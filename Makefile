#------------------------------------------------------------------------------------------------
#
# Makefile to build all locis subprojects
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

ifeq ($(config),)
	config = debug
endif

# OUT
OUT = Output/$(config)

# DIRS
DIR_LOCIS_CMD_APP = LOCISFrameWork/LOCIScmdapp
DIR_LOCIS_CORE = LOCISFrameWork/LOCISCore
DIR_LOCIS_FEM = LOCISFrameWork/LocisFEM

ALL_DIRS = $(DIR_LOCIS_CMD_APP)\
		   $(DIR_LOCIS_CORE)\
		   $(DIR_LOCIS_FEM)

# MAKE
export config

# RULES
.PHONY : lociscmdapp
lociscmdapp : locis locisfem
	$(MAKE) -C $(DIR_LOCIS_CMD_APP) 

.PHONY : locis
locis : 
	$(MAKE) -C $(DIR_LOCIS_CORE) 

.PHONY : locisfem
locisfem : 
	$(MAKE) -C $(DIR_LOCIS_FEM) 

.PHONY : all
all : locis locisfem lociscmdapp

.PHONY : cleand
cleand :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)
	
.PHONY : cleanr
cleanr :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)

.PHONY : clean
clean : 
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)