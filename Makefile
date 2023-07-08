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

_OBJ = call_class.o switched_thread.o tp_callable.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))





threadpool_test: $(OBJ) $(patsubst %,$(ODIR)/%,threadpool_test.o)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@.run $^

threadpool_test_nowrap: $(OBJ) $(patsubst %,$(ODIR)/%,threadpool_test_nowrap.o)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@.run $^

return_value_test: $(OBJ) $(patsubst %,$(ODIR)/%,return_value_test.o)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@.run $^

res_queue_test: $(OBJ) $(patsubst %,$(ODIR)/%,res_queue_test.o)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@.run $^

$(ODIR)/%.o: %.cc $(DEPS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f *.run
