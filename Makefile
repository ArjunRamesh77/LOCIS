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
DIR_LOCIS_SOLVE = LOCISFrameWork/solve

ALL_DIRS = $(DIR_LOCIS_API)\
		   $(DIR_LOCIS_CMD_APP)\
		   $(DIR_LOCIS_CORE)\
		   $(DIR_LOCIS_FEM)\
		   $(DIR_LOCIS_PERF)\
		   $(DIR_LOCIS_SOLVE)

# INCLUDE PATHS
PATH_LOCIS_API_INCLUDE :=  $(CURDIR)/LOCISFrameWork/api/include
PATH_LOCIS_CMD_INCLUDE :=  $(CURDIR)/LOCISFrameWork/cmdapp/include
PATH_LOCIS_INCLUDE :=	   $(CURDIR)/LOCISFrameWork/core/include
PATH_KINSOL_INCLUDE :=	   $(CURDIR)/Solvers/KINSOL-3.1.0/srcdir/include
PATH_SUNDIALS_INCLUDE :=   $(CURDIR)/Solvers/KINSOL-3.1.0/builddir/include
PATH_IDA_INCLUDE :=		   $(CURDIR)/Solvers/IDA-3.1.0/srcdir/include
PATH_PYTHON_INCLUDE :=	   /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	   /home/$(USER)/.local/lib/python2.7/site-packages/numpy/core/include/numpy
PATH_LOCIS_FEM_INCLUDE :=  $(CURDIR)/LOCISFrameWork/fem/include
PATH_LOCIS_PERF_INCLUDE := $(CURDIR)/LOCISFrameWork/perf/include
PATH_LOCIS_SOLVE_INCLUDE := $(CURDIR)/LOCISFrameWork/solve/include
PATH_LOCIS_EXTERNAL_INCLUDE := $(CURDIR)/ExternalFrameworks

export PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_API_INCLUDE)\
					-I$(PATH_LOCIS_CMD_INCLUDE)\
					-I$(PATH_LOCIS_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)\
					-I$(PATH_LOCIS_FEM_INCLUDE)\
					-I$(PATH_LOCIS_PERF_INCLUDE)\
					-I$(PATH_LOCIS_SOLVE_INCLUDE)\
					-I$(PATH_LOCIS_EXTERNAL_INCLUDE)

# LIB PATHS
PATH_KINSOL_LIB := 		$(CURDIR)/Solvers/KINSOL-3.1.0/builddir/src/kinsol
PATH_IDA_LIB :=  		$(CURDIR)/Solvers/IDA-3.1.0/builddir/src/ida
PATH_NVEC_SERIAL_LIB := $(CURDIR)/Solvers/IDA-3.1.0/builddir/src/nvec_ser
PATH_IDA_SUNLINSOL_DENSE := $(CURDIR)/Solvers/IDA-3.1.0/builddir/src/sunlinsol_dense
PATH_IDA_SUNMAT_DENSE := $(CURDIR)/Solvers/IDA-3.1.0/builddir/src/sunmat_dense
PATH_IDA_SUNMAT_SPARSE := $(CURDIR)/Solvers/IDA-3.1.0/builddir/src/sunmat_sparse
PATH_PYTHON_LIB := 		/usr/lib/python2.7/config-x86_64-linux-gnu
PATH_LOCIS_LIBS := 		$(CURDIR)/Output/$(config)

export PATH_ALL_LIB =-L$(PATH_NVEC_SERIAL_LIB)\
					-L$(PATH_IDA_SUNLINSOL_DENSE)\
					-L$(PATH_IDA_SUNMAT_DENSE)\
					-L$(PATH_IDA_SUNMAT_SPARSE)\
					-L$(PATH_KINSOL_LIB)\
					-L$(PATH_IDA_LIB)\
					-L$(PATH_LOCIS_LIBS)
# LIBS
#LIB_LOCIS :=    -llocis
#LIB_LOCIS_FEM := -llocisfem
LIB_KINSOL := 	-lsundials_kinsol
LIB_IDA := 		-lsundials_ida
LIB_NVEC :=     -lsundials_nvecserial
LIB_SUNLINSOL := -lsundials_sunlinsoldense
LIB_SUNMAT_DENSE := -lsundials_sunmatrixdense
LIB_SUMMAT_SPARSE := -lsundials_sunmatrixsparse
#LIB_PYTHON := 	-lpython2.7
LIB_PERF :=     -llocisperf
LIB_SOLVE :=    -llocissolve
     
export LIB_ALL :=$(LIB_SOLVE)\
				$(LIB_PERF)\
				$(LIB_KINSOL)\
				$(LIB_IDA)\
				$(LIB_NVEC)\
				$(LIB_SUNMAT_DENSE)\
				$(LIB_SUNMAT_SPARSE)\
				$(LIB_SUNLINSOL)
				
				# COMPILER AND FLAGS
export CC := gcc
export CXX := g++
export AR := ar
export ARFLAGS = rcs
ifeq ($(config),debug)
	CXXFLAGS = -Wall -fPIC -c -std=c++11 -g -fpermissive
	CCFLAGS = -Wall -fPIC -c -g
else
	CXXFLAGS = -Wall -fPIC -c -std=c++11 -O3 -fpermissive
	CCFLAGS = -Wall -fPIC -c 
endif

# MAKE
export config
export CXXFLAGS
export CCFLAGS

# RULES
.PHONY : lociscmdapp
lociscmdapp : locisperf locissolve
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

.PHONY : locissolve
locissolve :
	$(MAKE) -C $(DIR_LOCIS_SOLVE)

.PHONY : all
all : locis locisfem locisperf locissolve lociscmdapp locisapi

.PHONY : cleand
cleand :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)
	
.PHONY : cleanr
cleanr :
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)

.PHONY : clean
clean : 
	$(foreach DIR, $(ALL_DIRS), $(MAKE) -C $(DIR) $@;)