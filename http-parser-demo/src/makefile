cc=gcc
bin_dir=../bin
bin=$(bin_dir)/http-parser-demo
LIB=../lib
INC=../lib
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o) 
$(shell mkdir -p $(bin_dir))
    
$(bin): $(obj)
	$(cc) -O3 -o $(bin) $(obj) -lhttp_parser
%.o: %.cpp $(deps)
	$(cc) -c $< -o -O3 $@

clean:
	find . -name '*.o' -exec rm -rf  '{}' \;
	rm  $(bin)

