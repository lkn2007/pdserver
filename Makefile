all: server

server:
	gcc main.c check_black_list.c find_upserver.c fill_dns_body.c -o pdserver

clean:
	rm pdserver