########################################################
#           PRIMAVERA Mattia                           #
#           Universite Pierre & Marie Curie            #
#           Un client POP graphique                    #
########################################################
OBJDIR := /tmp/temporaire

l=X11
CC=cc -Wall

B=main-pop
C=clicable-pop
T=textuel-pop
G=graphique-pop
R=requests-pop
E=events-pop
P=pop

A=AnalyseEntetes

TEST1 = "USER moi\nPASS a\nLIST\nQUIT\n"
TEST2 = "USER moi\nPASS a\nTOP 1 0\nRETR 1\nQUIT\n"
TEST3 = "USER moi\nPASS a\nRETR 2\nQUIT\n"

HOST=localhost
PORT=6020

ALLC=$(B).c $(C).c $(E).c  $(A).c $(G).c $(T).c $(R).c
ALLO=$(OBJDIR)/$(C).o \
     $(OBJDIR)/$(E).o \
     $(OBJDIR)/$(A).o \
     $(OBJDIR)/$(G).o \
     $(OBJDIR)/$(T).o \
     $(OBJDIR)/$(R).o \

#compilation
$(OBJDIR)/$(B):	$(B).c $(ALLO) $(P).h
	$(CC) $< $(ALLO) -l$l -o $@

$(OBJDIR)/%.o:  %.c $(P).h
	$(CC) -c $< -o $@

$(ALLO): $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#tests
t-test-1: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	echo ${TEST1} | $(OBJDIR)/$(B) $(HOST) $(PORT) -t

t-test-2: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	echo ${TEST2} | $(OBJDIR)/$(B) $(HOST) $(PORT) -t

t-test-3: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	echo ${TEST3} | $(OBJDIR)/$(B) $(HOST) $(PORT) -t

t-test: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	$(OBJDIR)/$(B) $(HOST) $(PORT) -t

c-test: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	$(OBJDIR)/$(B) $(HOST) $(PORT) -c

g-test: $(OBJDIR)/$(B)
	python pop-server.py $(HOST):$(PORT) messagesDir &
	sleep 2
	$(OBJDIR)/$(B) $(HOST) $(PORT) -g

#clean the project dir
cleanall:
	@rm -f *.o $(B) 1.txt 2.asc 3.txt
	@rm -r $(OBJDIR)
	@rm -r messagesDir
	#@kill $(lsof -t -i:5054)
