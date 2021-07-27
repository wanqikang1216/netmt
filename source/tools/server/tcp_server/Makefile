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
                 -lftp \
                 -lbase

#默认使用动态库
LIBS_SHARED = -lhv \
                -lpthread \
                -lftp \
                -lbase

#库查找目录 
LIB_INC = -L. \
         -L${PWD_DIR}/../lib \
         -L${PWD_DIR}/../../lib \
         -L${PWD_DIR}/../../../lib

ECHO = tcp_echo_server
ECHOC = tcp_echo_server.c

CHAT = tcp_chat_server
CHATC = tcp_chat_server.c


TOTAL = ${ECHO} ${CHAT}

all : ${TOTAL}

${ECHO} : ${ECHOC}
        #链接静态库
        #${CC}  ${INCLUDE} ${LIB_INC} ${LIBS_STATIC} -o $@  $?
        #默认链接动态库
	${CC} ${INCLUDE} ${LIB_INC} ${LIBS_SHARED} -o $@  $?
	cp ../../../lib/libhv.so .
	cp ../../../lib/libftp.so .
	#cd .. ; mkdir bin
        #cp ${OBJ} ../bin

${CHAT} : ${CHATC}
	${CC} ${INCLUDE} ${LIB_INC} ${LIBS_SHARED} -o $@ $?
	cp ../../../lib/libbase.so .

.PHONY : clean
clean : 
	${RM} -rf ${TOTAL} *.so *.a


