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
DIR_LOCIS_API = LOCISFrameWork/api
DIR_LOCIS_CMD_APP = LOCISFrameWork/cmdapp
DIR_LOCIS_CORE = LOCISFrameWork/core
DIR_LOCIS_FEM = LOCISFrameWork/fem
DIR_LOCIS_PERF = LOCISFrameWork/perf

ALL_DIRS = $(DIR_LOCIS_API)\
		   $(DIR_LOCIS_CMD_APP)\
		   $(DIR_LOCIS_CORE)\
		   $(DIR_LOCIS_FEM)\
		   $(DIR_LOCIS_PERF)

# INCLUDE PATHS
PATH_LOCIS_API_INCLUDE :=  $(CURDIR)/LOCISFrameWork/api/include
PATH_LOCIS_CMD_INCLUDE :=  $(CURDIR)/LOCISFrameWork/cmdapp/include
PATH_LOCIS_INCLUDE :=	   $(CURDIR)/LOCISFrameWork/core/include
PATH_KINSOL_INCLUDE :=	   $(CURDIR)/Solvers/KINSOL/srcdir/include
PATH_SUNDIALS_INCLUDE :=   $(CURDIR)/Solvers/KINSOL/builddir/include
PATH_IDA_INCLUDE :=		   $(CURDIR)/Solvers/IDA/srcdir/include
PATH_PYTHON_INCLUDE :=	   /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	   /home/$(USER)/.local/lib/python2.7/site-packages/numpy/core/include/numpy
PATH_LOCIS_FEM_INCLUDE :=  $(CURDIR)/LOCISFrameWork/fem/include
PATH_LOCIS_PERF_INCLUDE := $(CURDIR)/LOCISFrameWork/perf/include

export PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_API_INCLUDE)\
					-I$(PATH_LOCIS_CMD_INCLUDE)\
					-I$(PATH_LOCIS_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)\
					-I$(PATH_LOCIS_FEM_INCLUDE)\
					-I$(PATH_LOCIS_PERF_INCLUDE)

# LIB PATHS
PATH_KINSOL_LIB := 		$(CURDIR)/Solvers/KINSOL/builddir/src/kinsol
PATH_IDA_LIB :=  		$(CURDIR)/Solvers/IDA/builddir/src/ida
PATH_NVEC_SERIAL_LIB := $(CURDIR)/Solvers/IDA/builddir/src/nvec_ser
PATH_PYTHON_LIB := 		/usr/lib/python2.7/config-x86_64-linux-gnu
PATH_LOCIS_LIBS := 		$(CURDIR)/Output/$(config)

export PATH_ALL_LIB = -L$(PATH_KINSOL_LIB)\
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
				$(LIB_PERF)\
				$(LIB_PYTHON)\

# MAKE
export config
export shared

# RULES
.PHONY : locisapi
locisapi : locis locisfem locisperf
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
all : locis locisfem locisperf lociscmdapp locisapi

.PHONY : cleand
cleand :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)
	
.PHONY : cleanr
cleanr :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)

.PHONY : clean
clean : 
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)