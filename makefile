# Makefile for LDFroot to convert raw data (L003) to ROOT file

CPP      = g++
CPPFLAGS = -Wall -g

# ROOT
ROOTINC  = `root-config --cflags`
ROOTLIB  = `root-config --libs`

LDFroot: LDFroot.h LDFroot.cpp LDFDataReader.o
	$(CPP) $(ROOTINC) $(ROOTLIB) $(CPPFLAGS) \
        -o LDFroot LDFroot.cpp LDFDataReader.o

LDFDataReader.o: LDFDataReader.h LDFDataReader.cpp
	$(CPP) -c $(CPPFLAGS) $(ROOTLIB) $(ROOTINC) LDFDataReader.cpp

clean:
	rm -f *.o LDFroot
