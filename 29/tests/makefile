include_path=-I /home/zzy0/vim_workspace/reactor_model/29/include
lib_path=-L /home/zzy0/vim_workspace/reactor_model/29/lib

lib1=-l network

demo1=test_enum
demo2=test_smart_ptr
# demo3=test_thread_pool
demo4=test_shared_ptr

all:$(demo1) $(demo2) $(demo3) $(demo4)

$(demo1):
	g++ -o ./bin/$(demo1) test_enum.cpp $(include_path) $(lib_path) $(lib1)

$(demo2):
	g++ -o ./bin/$(demo2) test_smart_ptr.cpp $(include_path) $(lib_path) $(lib1)

# $(demo3):
# 	g++ -o ./bin/$(demo3) test_thread_pool.cpp $(include_path) $(lib_path) $(lib1)

$(demo4):
	g++ -o ./bin/$(demo4) test_shared_ptr.cpp $(include_path) $(lib_path) $(lib1)

clean:
	rm ./bin/*
