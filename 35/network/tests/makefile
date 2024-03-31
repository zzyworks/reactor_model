include_path=-I /home/zzy0/vim_workspace/reactor_model/35/include
lib_path=-L /home/zzy0/vim_workspace/reactor_model/35/lib

lib1=-l network

# demo1=test_enum
# demo2=test_smart_ptr
# demo3=test_thread_pool
# demo4=test_shared_ptr
# demo5=test_unique_ptr
# demo6=test_event_fd
# demo7=test_pipe
# demo8=server
# demo9=client
# demo10=test_time_stamp
demo11=test_buffer

all:$(demo11)

$(demo11):
	g++ -o ./bin/$(demo11) test_buffer.cpp $(include_path) $(lib_path) $(lib1) -g

clean:
	rm ./bin/*

# $(demo1):
# 	g++ -o ./bin/$(demo1) test_enum.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo10):
# 	g++ -o ./bin/$(demo10) test_time_stamp.cpp $(include_path) $(lib_path) $(lib1)
# $(demo2):
# 	g++ -o ./bin/$(demo2) test_smart_ptr.cpp $(include_path) $(lib_path) $(lib1)
# 
# # $(demo3):
# # 	g++ -o ./bin/$(demo3) test_thread_pool.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo4):
# 	g++ -o ./bin/$(demo4) test_shared_ptr.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo5):
# 	g++ -o ./bin/$(demo5) test_unique_ptr.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo6):
# 	g++ -o ./bin/$(demo6) test_event_fd.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo7):
# 	g++ -o ./bin/$(demo7) test_pipe.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo8):
# 	g++ -o ./bin/$(demo8) test_server.cpp $(include_path) $(lib_path) $(lib1)
# 
# $(demo9):
# 	g++ -o ./bin/$(demo9) test_client.cpp $(include_path) $(lib_path) $(lib1)
