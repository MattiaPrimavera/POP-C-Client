#       Universite Pierre & Marie Curie 
#       Master d'informatique 2010-2011
#       Architecture des Clients Internet Interactifs
#       Un client Subversion utilisant Webdav



XPDIR=~/Development/expat-2.1.0

# Expat doesn't actually link itself into a library, so we
# have to refer to the individual objects that make up expat

XP_OBJ = $(XPDIR)/lib/xmltok.o \
         $(XPDIR)/lib/xmlrole.o \
         $(XPDIR)/lib/xmlparse.o

XP_INC =  -I$(XPDIR)/lib

################
# Adds support for checking lexical aspects of namespaces
XP_NS = -DXML_NS

################
# Adds support for processing DTDs
XP_DTD = -DXML_DTD

################
# Byte order macro
#This is for little endian machines

XP_BO = -DXML_BYTE_ORDER=12

# and this one is for big endian machines

#XP_BO = -DXML_BYTE_ORDER=21


################
# Define XML_Char as unsigned short
#XP_UNI = -DXML_UNICODE

################
# Define XML_Char as wchar_t
#XP_UNI_WC = -DXML_UNICODE_WCHAR_T

################
# Uncomment XP_MM if your system has a bcopy function but not memmove
#XP_MM = -D"memmove(d,s,l)=bcopy(s,d,l)"


XP_DEFINES = $(XP_NS) $(XP_DTD) $(XP_BO) $(XP_UNI) $(XP_UNI_WC) $(XP_MM)

################
# To get debugging, set this to -g
OPT = -O
#I=-I/usr/include

CC = cc -Wall 
#$(OPT) $(XP_INC) $(XP_DEFINES) $I

B=subversif
P=peroraison
C=InitConnexion
A=AnalyseEntetes

T=entries.xml

ALLC=$(B).c $(P).c $(C).c user_req.c list_req.c $(A).c
ALLO=$P.o $(C).o user_req.o list_req.o $(A).o


$(B):	$(B).c $(ALLO) $(P).h
	$(CC) $< $(ALLO) -o $@

%.o:    %.c $(P).h
	$(CC) -c $< -o $@ 
	

raz:
	@rm -f *.o $(B) tm.log tm.aux
