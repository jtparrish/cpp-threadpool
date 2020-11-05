IDIR = library_header

CXX = g++
CXXFLAGS= -g -Wall -I$(IDIR)

ODIR = build
TDIR = test_code
SDIR = library_code

VPATH = $(SDIR):$(TDIR)

LIBS= -pthread

_DEPS = $(shell ls library_header)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = threadpool.o call_class.o switched_thread.o tp_callable.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))





threadpool_test: $(OBJ) $(patsubst %,$(ODIR)/%,threadpool_test.o)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: %.cc $(DEPS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o