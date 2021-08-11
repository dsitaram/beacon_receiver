
# g++ -O3 -I/usr/local/include -I/usr/local/include/gnuradio    -o beacon receiver2.cc sgo_beacon_sink.cc flights.cc sgo_beacon_opts.cc -L/usr/lib -L/usr/local/lib -lboost_program_options  -lboost_system -lboost_thread -lm -luhd -lgnuradio-uhd -lgnuradio-pmt -lgnuradio-core -lgruel

INCS = -I/usr/include -I/usr/local/include 
LIBS =  -L/usr/lib -L/usr/local/lib -lboost_program_options  -lboost_system -lboost_thread -lm -luhd 
CXXFLAGS=-O3 $(INCS) 
CFLAGS=-O3 -lm 

all:
	g++ $(CXXFLAGS) -o beacon rx_beacon.cc sgo_beacon_opts.cc sgo_beacon_sink.cc flights.cc $(LIBS)

#LIBS =  -lstdc++ -lboost_thread-gcc43-mt-1_35 -lrt -lboost_date_time-gcc43-1_35 -lgnuradio-core -lgruel -lfftw3f -lgsl -lusrp -lgslcblas -lm -lgromnithread -lboost_program_options -lgnuradio-usrp
