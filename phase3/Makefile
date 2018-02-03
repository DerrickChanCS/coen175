CXX		= g++
CXXFLAGS	= -g -Wall
EXTRAS		= lexer.cpp
OBJS		= lexer.o parser.o type.o
PROG		= scc


all:		$(PROG)

$(PROG):	$(EXTRAS) $(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core *.o

clobber:;	$(RM) $(EXTRAS) $(PROG) core *.o

lexer.cpp:	lexer.l
		$(LEX) $(LFLAGS) -t lexer.l > lexer.cpp

lextest:	lextest.cpp lexer.o
		$(CXX) -o lextest lextest.cpp lexer.o

symbtest:	parser.cpp lexer.o type.cpp
		$(CXX) -o symbtest parser.cpp lexer.o type.cpp

typetest:	type.cpp typeTest.cpp
		$(CXX) -o typetest type.cpp typeTest.cpp

symboltest:	type.cpp symbolTest.cpp symbol.cpp
		$(CXX) -o symboltest type.cpp symbolTest.cpp symbol.cpp

scopetest:	type.cpp scopeTest.cpp scope.cpp symbol.cpp
		$(CXX) -o scopetest type.cpp scopeTest.cpp symbol.cpp scope.cpp
