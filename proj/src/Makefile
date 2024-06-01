# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/controllers
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/data_structures
# source code files to be compiled
SRCS = proj.c utils.c timer.c keyboard.c mouse.c graphics.c game.c menu.c maze.c stack.c linked_list.c rtc.c serial.c queue.c info.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
