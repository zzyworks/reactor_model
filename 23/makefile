INCLUDE_DIR=-I /home/zzy0/vim_workspace/reactor_model/network_server_2/23
LIB_DIR=-L /home/zzy0/vim_workspace/reactor_model/network_server_2/23

lib1=-l inet_address
lib2=-l my_socket
lib3=-l my_epoll
lib4=-l channel
lib5=-l event_loop
lib6=-l tcp_server
lib7=-l acceptor
lib8=-l connection
lib9=-l buffer
lib10=-l echo_server

s1=libinet_address.a
s2=libmy_socket.a
s3=libmy_epoll.a
s4=libchannel.a
s5=libevent_loop.a
s6=libtcp_server.a
s7=libacceptor.a
s8=libconnection.a
s9=libbuffer.a
s10=libecho_server.a

all:$(s1) $(s2) $(s3) $(s4) $(s5) $(s6) $(s7) $(s8) $(s9) $(s10)\
		client server

$(s1):inet_address.h inet_address.cpp
	g++ -c -o $(s1) inet_address.cpp

$(s2):my_socket.h my_socket.cpp
	g++ -c -o $(s2) my_socket.cpp

$(s3):my_epoll.h my_epoll.cpp
	g++ -c -o $(s3) my_epoll.cpp

$(s4):channel.h channel.cpp
	g++ -c -o $(s4) channel.cpp

$(s5):event_loop.h event_loop.cpp
	g++ -c -o $(s5) event_loop.cpp

$(s6):tcp_server.h tcp_server.cpp
	g++ -c -o $(s6) tcp_server.cpp

$(s7):acceptor.h acceptor.cpp
	g++ -c -o $(s7) acceptor.cpp

$(s8):connection.h connection.cpp
	g++ -c -o $(s8) connection.cpp

$(s9):buffer.h buffer.cpp
	g++ -c -o $(s9) buffer.cpp

$(s10):echo_server.h echo_server.cpp
	g++ -c -o $(s10) echo_server.cpp

client:client.cpp
	g++ -o client client.cpp -g $(INCLUDE_DIR) $(LIB_DIR) $(lib1)

server:server.cpp
	g++ -o server server.cpp -g $(INCLUDE_DIR) $(LIB_DIR) $(lib1) $(lib2) \
	$(lib3) $(lib4) $(lib5) $(lib6) $(lib7) $(lib8) $(lib9) $(lib10)

clean:
	rm -f server client $(s1) $(s2) $(s3) $(s4) $(s5) $(s6) $(s7) $(s8) $(s9) $(s10)
