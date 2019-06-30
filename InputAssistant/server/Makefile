Edit := main
#CFLAGS := -g -lpthread -I /usr/include/mysql/ -lmysqlclient -lcrypt -lcrypto -lssl
CFLAGS := -std=c++11 -g -lpthread
Path_src := ./src/#
Path_obj := ./bin/#
Src := $(wildcard $(Path_src)*.cc) 
Obj := $(patsubst $(Path_src)%.cc, $(Path_obj)%.o, $(Src))


$(Edit):$(Obj)
	g++ -o $@ $^ $(CFLAGS)
$(Obj):$(Src)
	@$(foreach i,$(Src),\
		g++ -o $(patsubst $(Path_src)%.cc,$(Path_obj)%.o,$(i)) -c $(i) $(CFLAGS);\
	)


.PHONY:clean
clean:
	-rm $(Edit) $(Obj)



