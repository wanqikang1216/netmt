#获取当前路径
PWD_DIR = ${shell pwd}

#输入的头文件路径
INCLUDE := -I. \
         -I${PWD_DIR}/../include/hv \
         -I${PWD_DIR}/../../include/hv \
         -I${PWD_DIR}/../../../include/hv \
         -I${PWD_DIR}/../../../include/hv/portable \
         -I${PWD_DIR}/../../../include/hv/base

#使用静态库
LIBS_STATIC =    -static\
                 -lpthread \
                 -lhv \
                 #-lftp \
                 -lbase

#默认使用动态库
LIBS_SHARED = -lhv \
                -lpthread \
                #-lftp \
                -lbase

#库查找目录 
LIB_INC = -L. \
         -L${PWD_DIR}/../lib \
         -L${PWD_DIR}/../../lib \
         -L${PWD_DIR}/../../../lib

UDP = udp_echo_server
UDPC = udp_echo_server.c

TOTAL = ${UDP}

all : ${TOTAL} 

${UDP} : ${UDP}
	#链接静态库
	#${CC}  ${INCLUDE} ${LIB_INC} ${LIBS_STATIC} -o ${OBJ}  ${OBJO} 
	#默认链接动态库
	${CC} ${INCLUDE} ${LIB_INC} ${LIBS_SHARED} -o ${UDP}  ${UDPC}
	cp ../../../lib/libhv.so .
	#cd .. ; mkdir bin
	#cp ${OBJ} ../bin

.PHONY : clean
clean : 
	${RM} -rf ${TOTAL} *.so *.a
