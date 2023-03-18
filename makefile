tester:graph.c minheap.c graph_algos.c graph_tester.c
	gcc -Wall -Werror graph.c minheap.c graph_algos.c graph_tester.c -o tester
.PHONY:run
run:tester
	./tester sample_input.txt

.PHONY: gdb
gdb:tester
	gcc -g -Wall -Werror graph.c minheap.c graph_algos.c graph_tester.c -o tester ;\
	lldb ./tester sample_input.txt