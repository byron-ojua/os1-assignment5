all:
	gcc --std=gnu99 -o line_processor main.c util.c

clean: 
	rm line_processor

test:
	clear
	make
	./line_processor

leak-test:
	clear
	make
	valgrind --leak-check=yes ./line_processor

gdb-test:
	clear
	echo Run GDB with 'run'
	make
	gdb ./line_processor

processes:
	ps -ef | grep niceb

kill-all:
	ps -ef | grep niceb | awk '{print $2}' | xargs kill -9 {} \;
