
EXE = waveM
CXX = g++
SYSTEM = x86-64_linux
LIBFORMAT = static_pic
OBJECT = ../build

CXXOPTS = -m64 -O3 -fopenmp -fPIC -fno-strict-aliasing -fexceptions -fpermissive -DNDEBUG -DIL_STD -Wno-ctor-dtor-privacy
CFLAGS = -std=c++11 -static-libstdc++ -O3 -fopenmp -fpermissive -DNDEBUG -DIL_STD -Wno-ctor-dtor-privacy $(LIBINCLUDE)

LIBINCLUDE = -I$(SRCHASH)/include -I$(BOOST)/include  


LINKCFLAGS = $(CFLAGS) -static-libstdc++ -lX11 -ldl -lm -lpthread#$(LIBGOMP)

BOOST   = ../lib/boost
SRCHASH = ../lib/sparsehash
LIBGOMP = -L../lib/gomp/libgomp.a -static


SRC = 	$(OBJECT)/main.o \
		$(OBJECT)/bitmap.o \
		$(OBJECT)/router.o \
		$(OBJECT)/server.o \
		$(OBJECT)/simulator.o \
		$(OBJECT)/graph.o 



$(EXE): $(SRC)
	@echo " "
	@echo "Linking..."
	@echo " "
	$(CXX) $(CXXOPTS) $^ -o $@ $(LINKCFLAGS)
	@echo " "
	@echo "Linking is Completed !!"
	@echo " "
	@echo "Tagging..."
	@echo " "
	ctags -R
	@echo " "
	@echo "Finished..!"
	@echo " "
	@echo "GNU GCC/G++ compiler has been used as"
	@g++ --version


$(OBJECT)/main.o: main.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

#$(OBJECT)/bitmap.o: bitmap.cpp bitmap.h
#	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/router.o: router.cpp router.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/graph.o: graph.cpp graph.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/server.o: server.cpp server.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/simulator.o: simulator.cpp simulator.h
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	/bin/rm -f ./objects/*.o 2> /dev/null;  /bin/rm -f $(EXE) 2> /dev/null; /bin/rm -f defreader defwriter lefreader lefwriter 2> /dev/null; /bin/rm -f *.log;









