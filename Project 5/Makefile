CC=gcc
CFLAGS=-Wall -pedantic -g
OBJECTS=step1.o step2.o step3.o step4.o step5.o step6.o step7.o step8.o step9.o logger.o

all: step1 step2 step3 step4 step5 step6 step7 step8 step9 logger
step1: step1.o 
	$(CC) $(CFLAGS) -o step1 step1.o	
step2: step2.o 
	$(CC) $(CFLAGS) -o step2 step2.o
step3: step3.o 
	$(CC) $(CFLAGS) -o step3 step3.o
step4: step4.o 
	$(CC) $(CFLAGS) -o step4 step4.o
step5: step5.o 
	$(CC) $(CFLAGS) -o step5 step5.o
step6: step6.o 
	$(CC) $(CFLAGS) -o step6 step6.o
step7: step7.o 
	$(CC) $(CFLAGS) -o step7 step7.o
step8: step8.o 
	$(CC) $(CFLAGS) -o step8 step8.o
step9: step9.o 
	$(CC) $(CFLAGS) -o step9 step9.o
logger: logger.o 
	$(CC) $(CFLAGS) -o logger logger.o
step1.o: step1.c
	$(CC) $(CFLAGS) -c step1.c
step2.o: step2.c
	$(CC) $(CFLAGS) -c step2.c
step3.o: step3.c
	$(CC) $(CFLAGS) -c step3.c
step4.o: step4.c
	$(CC) $(CFLAGS) -c step4.c
step5.o: step5.c
	$(CC) $(CFLAGS) -c step5.c
step6.o: step6.c
	$(CC) $(CFLAGS) -c step6.c
step7.o: step7.c
	$(CC) $(CFLAGS) -c step7.c
step8.o: step8.c
	$(CC) $(CFLAGS) -c step8.c
step9.o: step9.c
	$(CC) $(CFLAGS) -c step9.c
logger.o: logger.c
	$(CC) $(CFLAGS) -c logger.c
.PHONY: clean
clean:
	rm step1 step2 step3 step4 step5 step6 step7 step8 step9 logger
submission: 
	tar czvf prog5.tgz ./