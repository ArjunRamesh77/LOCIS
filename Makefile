#------------------------------------------------------------------------------------------------
#
# Makefile to build locis
# Created 09/17/217
# NOTES:
#	works only on linux for now
#
#------------------------------------------------------------------------------------------------

# FILE EXTENSIONS
EXT_CPP := cpp
EXT_OBJ := o
EXT_LIB := a

VPATH = Solvers/KINSOL/kinsol_src/srcdir/include/sundials

# PROGRAM
NAME_PROGRAM = locis 

# INCLUDE PATHS
PATH_ROOT = $(CURDIR)
PATH_LOCIS_INCLUDE :=	 LOCISFrameWork/LOCISFrameWork/include
PATH_KINSOL_INCLUDE :=	 Solvers/KINSOL/kinsol_src/srcdir/include
PATH_SUNDIALS_INCLUDE := Solvers/KINSOL/kinsol_src/builddir/include
PATH_IDA_INCLUDE :=		 Solvers/IDA/IDA_src/srcdir/include
PATH_PYTHON_INCLUDE :=	 /usr/include/python2.7
PATH_NUMPY_INCLUDE :=	 /home/arjun/.local/lib/python2.7/site-packages/numpy/core/include/numpy

PATH_ALL_INCLUDE =  -I$(PATH_LOCIS_INCLUDE)\
					-I$(PATH_SUNDIALS_INCLUDE)\
					-I$(PATH_KINSOL_INCLUDE)\
					-I$(PATH_IDA_INCLUDE)\
					-I$(PATH_PYTHON_INCLUDE)\
				    -I$(PATH_NUMPY_INCLUDE)

# OUTPUT PATHS
PATH_LOCIS_OUT = Output/LOCISFrameWork

# TEMP PATHS
PATH_LOCIS_TEMP = Temp/LOCISFrameWork

# SOURCE PATHS
PATH_LOCIS_SRC = LOCISFrameWork/LOCISFrameWork/src

# LIB PATHS
PATH_KINSOL_LIB := 		Solvers/KINSOL/kinsol_src/builddir/src/kinsol
PATH_IDA_LIB :=  		Solvers/IDA/IDA_src/builddir/src/ida
PATH_NVEC_SERIAL_LIB := Solvers/IDA/IDA_src/builddir/src/nvec_ser
PATH_PYTHON_LIB := 		/usr/lib/python2.7/config-x86_64-linux-gnu

PATH_ALL_LIB = 		-L$(PATH_KINSOL_LIB)\
					-L$(PATH_IDA_LIB)\
					-L$(PATH_NVEC_SERIAL_LIB)\
					-L$(PATH_PYTHON_LIB)
# LIBS
LIB_KINSOL := 	-lsundials_kinsol
LIB_IDA := 		-lsundials_ida
LIB_NVEC :=     -lsundials_nvecserial
LIB_PYTHON := 	-lpython2.7

LIB_ALL := 		$(LIB_KINSOL)\
				$(LIB_IDA)\
				$(LIB_NVEC)\
				$(LIB_PYTHON)

# SOURCE/INCLUDE NAMES
LOCIS_INC = ""
LOCIS_SRC = ""
LOCIS_OBJ = ""

include $(PATH_LOCIS_TEMP)/locisInc.mk

include $(PATH_LOCIS_TEMP)/locisSrc.mk

LOCIS_OBJ = $(subst .cpp,.o,$(LOCIS_SRC))

# TRANSFORM 
LOCIS_INC_SUBS = $(subst ~,$(PATH_LOCIS_INCLUDE)/,$(LOCIS_INC))
LOCIS_SRC_SUBS = $(subst ~,$(PATH_LOCIS_SRC)/,$(LOCIS_SRC))
LOCIS_OBJ_SUBS = $(subst ~,$(PATH_LOCIS_TEMP)/,$(LOCIS_OBJ))

# COMPILER AND FLAGS
CXX := g++
COMPILE_INCLUDE_PATHS = $(PATH_ALL_INCLUDE)
COMPILE_LIB_PATHS = $(PATH_ALL_LIB)
COMPILE_OPTIONS = -Wall -c -fpermissive

# FUNCTIONS
				  
# TARGETS
# default goal
$(PATH_LOCIS_OUT)/$(NAME_PROGRAM): $(LOCIS_OBJ_SUBS) 
	g++ -fpermissive -Wall $^ $(PATH_ALL_LIB) $(LIB_ALL) -o $@ 

$(PATH_LOCIS_TEMP)/%.o : $(PATH_LOCIS_SRC)/%.cpp $(LOCIS_INC_SUBS)
	$(CXX) $(COMPILE_OPTIONS) $(COMPILE_INCLUDE_PATHS) $< -o $@

# Get all include files
$(PATH_LOCIS_TEMP)/locisInc.mk :
	ls $(PATH_LOCIS_INCLUDE) | sed 's/^/~/g' | sed 's/\.h/\.h\\/g' | sed '1 s/^/LOCIS_INC = /g' > $(PATH_LOCIS_TEMP)/locisInc.mk

# Get all source files
$(PATH_LOCIS_TEMP)/locisSrc.mk :
	ls $(PATH_LOCIS_SRC) | sed 's/^/~/g' | sed 's/\.cpp/\.cpp\\/g' | sed '1 s/^/LOCIS_SRC = /g' > $(PATH_LOCIS_TEMP)/locisSrc.mk

.PHONY : clean
clean :
	rm -f $(PATH_LOCIS_TEMP)/*; \
	rm -f $(PATH_LOCIS_OUT)/*
