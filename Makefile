########################################################
#    PRIMAVERA Mattia                                  #
#    Universite Pierre & Marie Curie                   #
#    Un client POP graphique                           #
########################################################
OBJDIR := build

l=X11
CC = cc -Wall

B=main-pop
C=clickable-pop
T=textuel-pop
G=graphical-pop
P=peroraison
I=InitConnexion
A=AnalyseEntetes
E=events

TEST1 = "USER moi\nPASS a\nLIST\nQUIT\n"
TEST2 = "USER moi\nPASS a\nTOP 1 0\nRETR 1"

#POP Requests
R1=user_req
#user_req -> USER, PASS and QUIT requests
R2=list_req
R3=top_req
R4=retr_req

HOST=localhost
PORT=5003

ALLC=$(B).c $(C).c $(P).c $(E).c $(I).c $(A).c $(G).c $(T).c $(R1).c $(R2).c $(R3).c $(R4).c
#ALLO=$P.o $(C).o $(E).o $(I).o $(X).o $(A).o $(R1).o $(R2).o $(R3).o $(R4).o
ALLO= $(OBJDIR)/$P.o \
	$(OBJDIR)/$(C).o \
	$(OBJDIR)/$(E).o \
	$(OBJDIR)/$(I).o \
	$(OBJDIR)/$(A).o \
	$(OBJDIR)/$(G).o \
	$(OBJDIR)/$(T).o \
	$(OBJDIR)/$(R1).o \
	$(OBJDIR)/$(R2).o \
	$(OBJDIR)/$(R3).o \
	$(OBJDIR)/$(R4).o

#compilation
$(OBJDIR)/$(B):  $(B).c $(ALLO) $(P).h
	$(CC) $< $(ALLO) -l$l -o $@

$(OBJDIR)/%.o:  %.c $(P).h
	$(CC) -c $< -o $@

$(ALLO): $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#tests
t-test-1: $(OBJDIR)/$(B)
	echo ${TEST1} | ./$(OBJDIR)/$(B) $(HOST) $(PORT) -t

t-test-2: $(OBJDIR)/$(B)
	echo ${TEST2} | ./$(OBJDIR)/$(B) $(HOST) $(PORT) -t

t-test: $(OBJDIR)/$(B)
	./$(OBJDIR)/$(B) $(HOST) $(PORT) -t

c-test: $(OBJDIR)/$(B)
	./$(OBJDIR)/$(B) $(HOST) $(PORT) -c

g-test: $(OBJDIR)/$(B)
	./$(OBJDIR)/$(B) $(HOST) $(PORT) -g

#clean the project dir
raz:
	@rm -f *.o $(B)
	@rm -r build
