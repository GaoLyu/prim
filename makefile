tester:graph.c minheap.c graph_algos.c graph_tester.c
	gcc -Wall -Werror graph.c minheap.c graph_algos.c graph_tester.c -o tester
.PHONY:run
run:tester
	./tester sample_input.txt