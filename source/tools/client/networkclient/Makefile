#获取当前路径
PWD_DIR = ${shell pwd}

#输入的头文件路径
INCLUDE := -I. \
         -I${PWD_DIR}/../include/hv \
         -I${PWD_DIR}/../../include/hv \
         -I${PWD_DIR}/../../../include/hv \
         -I${PWD_DIR}/../../../include/hv/portable

#使用静态库
LIBS_STATIC =    -static\
                 -lpthread \
                 -lhv \
                 -lftp
             
#默认使用动态库
LIBS_SHARED = -lhv \
                -lpthread \
                -lftp

#库查找目录 
LIB_INC = -L. \
         -L${PWD_DIR}/../lib \
	 -L${PWD_DIR}/../../lib \
	 -L${PWD_DIR}/../../../lib

NC = nc
NCC = nc.c

FTP = ftp
FTPC = ftp.c

TOTAL = ${FTP} ${NC}
 
all : ${TOTAL} 

${FTP} : ${FTPC}
	${CC} ${INCLUDE} ${LIB_INC} ${LIBS_SHARED} -o $@ $?
	cp ../../../lib/libftp.so .
	#cp ../../../lib/libftp.a /usr/lib/

${NC} : ${NCC}
	#链接静态库
	#${CC}  ${INCLUDE} ${LIB_INC}  ${LIBS_STATIC} -o $@  $? 
	#默认链接动态库
	${CC} ${INCLUDE} ${LIB_INC} ${LIBS_SHARED} -o $@  $?
	cp ../../../lib/libhv.so .
	#cp ../../../lib/libhv.a /usr/lib/
	#cd .. ; mkdir bin
	#cp $@ ../bin


.PHONY : clean
clean : 
	${RM} -rf ${TOTAL}  *.so *.a
