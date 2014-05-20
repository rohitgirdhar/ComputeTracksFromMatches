CXX += -std=c++0x 
CXXFLAGS += -O2
INC += -I${BOOST_ROOT}
LIBS += -L/usr/local/lib/
LDFLAGS += -lboost_filesystem -lboost_system

SRCS=computeTracks.cpp
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
EXECS=computeTracks

all: $(EXECS)

depend: .depend

.depend: $(SRCS)
	rm -rf ./.depend
	$(CXX) -std=c++0x $(CXXFLAGS) -MM $^ -MF ./.depend

include .depend

%: %.o
	$(LINK.cc) $(INC) -std=c++0x $< -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	@- $(RM) $(EXECS)
	@- $(RM) $(OBJS)
