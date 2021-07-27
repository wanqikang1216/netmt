
# 通常编译一个新的工程时，只需亚修改PATH_INCLUDE的内容，其他的都不需要修改
# 如果需要在这里链接某个输入库，则需要修改并打开PATH_LIBARAY和NAME_LIBARAY

TOPTMP               = $(shell pwd)
TOPDIR               = $(shell echo ${TOPTMP} |sed -r 's/(.*\/source).*/\1/g')
include $(TOPDIR)/config.mk

#输入的头文件路径
INCLUDE :=  -I/usr/include/ -I./ -I$(TOPDIR)/pubinc -I$(TOPDIR)/proc  \
             -I$(TOPDIR)/dpdk18/out/include               \
             -I$(TOPDIR)/wtf/include                     \
              -I/usr/local/include/libxml2/             \
            -I/usr/include/libxml2/
            
            
LIBS      = 
DYNAFILE  = libvtycmd.so#End
				

LIB_INC   = -L$(TOPDIR)/.
DYNADIR   =$(TOPDIR)/../out/bin/
EXCSRC	  =   
LIBCPPOBJS	 =  $(patsubst %.cpp,%.o,$(filter-out $(EXCSRC),$(wildcard *.cpp)))
LIBCOBJS     =  $(patsubst %.c,%.o,$(filter-out $(EXCSRC),$(wildcard *.c)))

DYNALIBRARY	 = $(DYNADIR)/$(DYNAFILE)

all: $(DYNALIBRARY)

clean:
	$(OS_RM) $(DYNALIBRARY) $(LIBCOBJS) $(LIBCPPOBJS)

$(DYNALIBRARY): .depend $(LIBCOBJS) $(LIBCPPOBJS)
	$(OS_CC) $(OS_CCDEBUGFLAGS) $(LDFLAGS) $(LIB_INC) $(LIBS) -o $@ $(LIBCOBJS) $(LIBCPPOBJS)

.depend: makefile $(LIBCOBJS:.o=.c) $(LIBCPPOBJS:.o=.cpp)
	$(OS_CC) -MM $(OS_CCFLAGS) $(OS_CCDYNAFLAGS) $(OS_CCDEBUGFLAGS) $(INCLUDE) $(LIBCOBJS:.o=.c) $(LIBCPPOBJS:.o=.cpp) > $@

ifeq (.depend, $(wildcard .depend))
include .depend
endif

%.o:%.c
	$(OS_CC)  $(OS_CCFLAGS) $(OS_CCDYNAFLAGS) $(OS_CCDEBUGFLAGS) $(INCLUDE) -c $< -o $@

%.o:%.cpp
	$(OS_CC)  $(OS_CCFLAGS) $(OS_CCDYNAFLAGS) $(OS_CCDEBUGFLAGS) $(INCLUDE) -c $< -o $@

