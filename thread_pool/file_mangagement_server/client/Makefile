Edit := main
CFLAGS := -g -lpthread
Path_src := ./src/#
Path_obj := ./bin/#
Src := $(wildcard $(Path_src)*.c) 
Obj := $(patsubst $(Path_src)%.c, $(Path_obj)%.o, $(Src))


$(Edit):$(Obj)
	gcc -o $@ $^
$(Obj):$(Src)
	@$(foreach i,$(Src),\
		gcc -o $(patsubst $(Path_src)%.c,$(Path_obj)%.o,$(i)) -c $(i);\
	)


.PHONY:clean
clean:
	-rm $(Edit) $(Obj)

