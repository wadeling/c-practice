cc=gcc
bin_dir=../bin
bin=$(bin_dir)/llhttp-demo
LIB=../lib
INC=../lib
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o) 
$(shell mkdir -p $(bin_dir))
    
$(bin): $(obj)
	$(cc)  -o $(bin) $(obj)
%.o: %.cpp $(deps)
	$(cc)  -c $< -o $@

clean:
	find . -name '*.o' -exec rm -rf  '{}' \;
	rm  $(bin)

