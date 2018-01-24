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
DIR_LOCIS_API = LOCISFrameWork/LOCISapi
DIR_LOCIS_CMD_APP = LOCISFrameWork/LOCIScmdapp
DIR_LOCIS_CORE = LOCISFrameWork/LOCISCore
DIR_LOCIS_FEM = LOCISFrameWork/LocisFEM
DIR_LOCIS_PERF = LOCISFrameWork/LOCISperf

ALL_DIRS = $(DIR_LOCIS_API)\
		   $(DIR_LOCIS_CMD_APP)\
		   $(DIR_LOCIS_CORE)\
		   $(DIR_LOCIS_FEM)\
		   $(DIR_LOCIS_PERF)

# MAKE
export config
export shared

# RULES
.PHONY : locisapi
lociscmdapp : locis locisfem locisperf
	$(MAKE) -C $(DIR_LOCIS_API) 

.PHONY : lociscmdapp
lociscmdapp : locis locisfem locisperf
	$(MAKE) -C $(DIR_LOCIS_CMD_APP) 

.PHONY : locis
locis : 
	$(MAKE) -C $(DIR_LOCIS_CORE) 

.PHONY : locisfem
locisfem : 
	$(MAKE) -C $(DIR_LOCIS_FEM)

.PHONY : locisperf
locisperf :
	$(MAKE) -C $(DIR_LOCIS_PERF)

.PHONY : all
all : locis locisfem locisperf lociscmdapp

.PHONY : cleand
cleand :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)
	
.PHONY : cleanr
cleanr :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)

.PHONY : clean
clean : 
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)