########################################################
#		PRIMAVERA Mattia							   #
#       Universite Pierre & Marie Curie                #
#       Un client POP graphique						   #
########################################################
OBJDIR := build

l=X11
CC = cc -Wall 

B=main-pop
C=clicable-pop
T=textuel-pop
G=graphique-pop
R=requests-pop
E=events-pop
P=pop

A=AnalyseEntetes


TEST1 = "USER moi\nPASS a\nLIST\nQUIT\n"
TEST2 = "USER moi\nPASS a\nTOP 1 0\nRETR 1"


HOST=localhost
PORT=5000

ALLC=$(B).c $(C).c $(E).c  $(A).c $(G).c $(T).c $(R).c  
ALLO= $(OBJDIR)/$(C).o \
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