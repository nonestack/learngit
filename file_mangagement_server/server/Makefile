Edit := main
CFLAGS := -g -lpthread -I /usr/include/mysql/ -lmysqlclient -lcrypt -lcrypto -lssl
Path_src := ./src/#
Path_obj := ./bin/#
Src := $(wildcard $(Path_src)*.c) 
Obj := $(patsubst $(Path_src)%.c, $(Path_obj)%.o, $(Src))


$(Edit):$(Obj)
	gcc -o $@ $^ $(CFLAGS)
$(Obj):$(Src)
	@$(foreach i,$(Src),\
		gcc -o $(patsubst $(Path_src)%.c,$(Path_obj)%.o,$(i)) -c $(i) $(CFLAGS);\
	)


.PHONY:clean
clean:
	-rm $(Edit) $(Obj)



