.PHONY: run clean

# source object target
SRC := $(wildcard      ./*.c)
LIB := $(wildcard ../lib/*.c)
OBJ := $(SRC:.c=.o) $(LIB:.c=.o)
APP := a.out

# 编译参数
CC      := gcc
CFLAGS  :=
DEFINES :=
LDFLAGS := -L../lib
LIB_INC := -I../lib
HDR_INC := -I../lib

# 链接
$(APP):$(OBJ)
	@ $(CC) $(LIB_INC) -o $@ $^

# 编译
%.o : %.c
	@ $(CC) $(HDR_INC) -o $@ $(CFLAGS) $(DEFINES) -c $^

# 运行
run : $(APP)
	@ ./$(APP)

# 清理
clean:
	@ $(RM) -f $(OBJ)
	@ $(RM) -f $(APP)
