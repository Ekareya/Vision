PROG = Vision

OBJS = Image.o Masque.o
CXX = g++
CXXFLAGS = -Wall -Wextra -g

.PHONY : all clean mrproper check

all : $(PROG) 
$(PROG) : $(OBJS) $(PROG).o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $@.o

%.t : %.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $<

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<


#Le programme est déjà compilé
check :
	./tests/tests.sh

clean :
	-rm -f *.o

mrproper :
	-rm -f *.o $(PROG) $(TESTS)

