Edit := main
#CFLAGS := -g -lpthread -I /usr/include/mysql/ -lmysqlclient -lcrypt -lcrypto -lssl
CFLAGS := -std=c++11 -g
Path_src := ./src/#
Path_src_reactor := ./src/reactor/#
Path_obj := ./bin/#
Src := $(wildcard $(Path_src)*.cc) 
Src += $(wildcard $(Path_src_reactor)*.cc)
Obj := $(patsubst $(Path_src)%.cc, $(Path_obj)%.o, $(Src))
Obj += $(patsubst $(Path_src_reactor)%.cc, $(Path_obj)%.o, $(Src))


$(Edit):$(Obj)
	g++ -o $@ $^ $(CFLAGS)
$(Obj):$(Src)
	@$(foreach i,$(Src),\
		g++ -o $(patsubst $(Path_src)%.cc,$(Path_obj)%.o,$(i)) -c $(i) $(CFLAGS);\
	)


.PHONY:clean
clean:
	-rm $(Edit) $(Obj)



