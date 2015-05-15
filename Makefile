########################################################
#		PRIMAVERA Mattia							   #
#       Universite Pierre & Marie Curie                #
#       Un client POP graphique						   #
########################################################
OBJDIR := build

l=X11
CC = cc -Wall 

B=main-pop
C=clickable-pop
P=peroraison
I=InitConnexion
A=AnalyseEntetes
E=events
X=xtable

#POP Requests
R1=user_req
#user_req -> USER, PASS and QUIT requests  
R2=list_req
R3=top_req
R4=retr_req

HOST=localhost
PORT=5002

ALLC=$(B).c $(C).c $(P).c $(E).c $(I).c $(X).c $(A).c $(R1).c $(R2).c $(R3).c $(R4).c 
#ALLO=$P.o $(C).o $(E).o $(I).o $(X).o $(A).o $(R1).o $(R2).o $(R3).o $(R4).o 
ALLO= $(OBJDIR)/$P.o \
	  $(OBJDIR)/$(C).o \
	  $(OBJDIR)/$(E).o \
	  $(OBJDIR)/$(I).o \
	  $(OBJDIR)/$(X).o \
	  $(OBJDIR)/$(A).o \
	  $(OBJDIR)/$(R1).o \
	  $(OBJDIR)/$(R2).o \
	  $(OBJDIR)/$(R3).o \
	  $(OBJDIR)/$(R4).o 

#compilation

$(OBJDIR)/$(B):	$(B).c $(ALLO) $(P).h
	$(CC) $< $(ALLO) -l$l -o $@

$(OBJDIR)/%.o:  %.c $(P).h
	$(CC) -c $< -o $@ 

$(OBJDIR):
	mkdir -p $(OBJDIR)

#tests	
t-test: $(B) 
	./$(B) $(HOST) $(PORT) -t

c-test: $(B) 
	./$(B) $(HOST) $(PORT) -c

g-test: $(B) 
	./$(B) $(HOST) $(PORT) -g

#clean the project dir
raz:
	@rm -f *.o $(B)
	@rm -r build