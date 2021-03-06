//
// UHD only receiver, because gnuradio is not needed and is actually just an added
// complicated dependency
//
// Juha Vierinen, 2017
//

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <complex>
#include <stdio.h>
#include "sgo_beacon_opts.h"
#include "sgo_beacon_sink.h"
namespace po = boost::program_options;

int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    //variables to be set by po
    std::string args;
    size_t total_num_samps=0;
    double rate;
    double seconds_in_future=1.5;
    int which = 0;                       // specify which USRP board
    //usrp_subdev_spec spec1(0,0);         // specify the d'board side
    //usrp_subdev_spec spec2(0,1);         // specify the d'board side
    int decim = 64;                      // set the decimation rate
    double freq1 = 150e6;                // set the frequency
    double freq2 = 400e6;                // set the frequency
    double offset = 0.0;                 // sample clock offset
    double offset2 = 0.0;                 // sample clock offset
    int useDriverFreq = 0;
    float gain1 = 1;                    // set the gain; -1 will set the mid-point gain
    float gain2 = 1;                    // set the gain; -1 will set the mid-point gain
    char *dirname;                       // output directory name
    long refclock = 100000000;
    std::string outputdir = ".";
    std::string addr = "recv_frame_size=4096,num_recv_frames=4096";
    std::string subdev = "A:RX1 A:RX2";
    std::string sync = "pps";
    std::string channel_list="0,1";
    
    po::options_description cmdconfig("Program options: usrp_text_rx [options] filename");
    cmdconfig.add_options()
      ("help,h", "produce help message")
      ("which,W", po::value<int>(&which), "select which USRP board")
      ("subdev-spec,r", po::value<std::string>(), "Subdev spec (e.g., A:RX1 A:RX2, or B:0 A:0)")
      ("addr,a", po::value<std::string>(), "Device address string (e.g., recv_frame_size=4096,num_recv_frames=4096, or addr0=192.168.10.2,recv_buff_size=100000000")
      ("decim,d", po::value<int>(&decim), "set fgpa decimation rate to DECIM")
      ("driverFreq,u", po::value<int>(&useDriverFreq), "use driver frequency.")
      ("freq1,f", po::value<double>(), "set frequency to FREQ1")
      ("freq2,F", po::value<double>(), "set frequency to FREQ2")
      ("sync,S", po::value<double>(), "sync (now,pps)")
      ("gain1,g", po::value<float>(), "set gain 1 in dB (default is midpoint)")
      ("gain2,G", po::value<float>(), "set gain 2 in dB (default is midpoint)")
      ("refclock,e", po::value<long>(), "reference clock frequency (default 64 MHz)")
      ("dirname,o", po::value<std::string>(), "Ouput directory name (default=.)")
      ("offset,s", po::value<double>(), "Frequency offset at 150 MHz (default=0.0)")
      ("offset2,t", po::value<double>(), "Frequency offset at 400 MHz (default=0.0)")
      ;
    
    po::options_description fileconfig("Input file options");
    fileconfig.add_options()
      ("filename", po::value<std::string>(), "input file")
      ;
    
    po::positional_options_description inputfile;
    inputfile.add("filename", -1);
    
    po::options_description config;
    config.add(cmdconfig).add(fileconfig);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
	      options(config).positional(inputfile).run(), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
      std::cout << cmdconfig << "\n";
      return 1;
    }
    
    if(vm.count("dirname")) {
      outputdir = vm["dirname"].as<std::string>();
    }
    
    if(vm.count("freq1")) {
      freq1 = vm["freq1"].as<double>();
    }
    if(vm.count("freq2")) {
      freq2 = vm["freq2"].as<double>();
    }
    
    if(vm.count("gain1")) {
      gain1 = vm["gain1"].as<float>();
    }
    if(vm.count("gain2")) {
      gain2 = vm["gain2"].as<float>();
    }
    
    if(vm.count("refclock")) {
      refclock = vm["refclock"].as<long>();
    }
    
    if(vm.count("offset")) {
      offset = vm["offset"].as<double>();
    }
    if(vm.count("offset2")) {
      offset2 = vm["offset2"].as<double>();
    }
    
    if(vm.count("subdev-spec")) {
      subdev = vm["subdev-spec"].as<std::string>();
    }
    if(vm.count("addr")) {
      addr = vm["addr"].as<std::string>();
    }
    
    std::cout << "which:   " << which << std::endl;
    std::cout << "addr:   " << addr << std::endl;
    std::cout << "subdev:   " << subdev << std::endl;
    std::cout << "decim:   " << decim << std::endl;
    std::cout << "freq1:    " << freq1 << std::endl;
    std::cout << "freq2:    " << freq2 << std::endl;
    std::cout << "gain1:    " << gain1 << std::endl;
    std::cout << "gain2:    " << gain2 << std::endl;
    std::cout << "outputdir:    " << outputdir << std::endl;
    
    
    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the usrp device with: %s...") % args << std::endl;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(addr);

    //always select the subdevice first, the channel mapping affects the other settings
    if (vm.count("subdev"))
      std::cout << subdev << std::endl;
      usrp->set_rx_subdev_spec(subdev); //sets across all mboards

    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

    printf("number of channels %d\n",(int)usrp->get_rx_num_channels());
    
    //set the rx sample rate (sets across all channels)
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (refclock/decim/1e6) << std::endl;
    usrp->set_rx_rate(refclock/decim);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate()/1e6) << std::endl << std::endl;

    usrp->set_rx_freq(freq1,0);
    usrp->set_rx_freq(freq2,1);
    usrp->set_rx_gain(gain1,0);
    usrp->set_rx_gain(gain2,1);
    double actual_freq1, actual_freq2;
    printf("Using custom freq %1.2f %1.2f\n",offset,offset2);
    printf("%1.6f %1.6f\n",usrp->get_rx_freq(0),usrp->get_rx_freq(1));
    usrp->set_rx_freq(freq2,1);
    
    actual_freq1 = freq1 + offset;
    actual_freq2 = freq2 + offset2;
    

    sgo_beacon_options *o=new sgo_beacon_options(actual_freq1, 
						 actual_freq2, 
						 decim, 
						 3600,
						 1.0,
						 1.0,
						 refclock,
						 outputdir);

    sgo_beacon_sink *s = new sgo_beacon_sink(o);
    
    std::cout << boost::format("Setting device timestamp to 0...") << std::endl;
    if (sync == "now"){
        //This is not a true time lock, the devices will be off by a few RTT.
        //Rather, this is just to allow for demonstration of the code below.
        usrp->set_time_now(uhd::time_spec_t(0.0));
    }
    else if (sync == "pps"){
      printf("syncing to external clock");
      usrp->set_time_source("gpsdo");
      usrp->set_clock_source("gpsdo");
      usrp->set_time_unknown_pps(uhd::time_spec_t(0.0));
      boost::this_thread::sleep(boost::posix_time::seconds(1)); //wait for pps sync pulse
    }

    //detect which channels to use
    std::vector<std::string> channel_strings;
    std::vector<size_t> channel_nums;
    boost::split(channel_strings, channel_list, boost::is_any_of("\"',"));
    for(size_t ch = 0; ch < channel_strings.size(); ch++){
      printf("%d/%d\n",(int)ch,(int)channel_strings.size());
      std::cout << std::endl << channel_strings[ch] << std::endl << std::endl;
      size_t chan = boost::lexical_cast<int>(channel_strings[ch]);
      if(chan >= usrp->get_rx_num_channels()){
	throw std::runtime_error("Invalid channel(s) specified.");
      }
      else channel_nums.push_back(boost::lexical_cast<int>(channel_strings[ch]));
    }
    printf("starting\n");
    //create a receive streamer
    //linearly map channels (index0 = channel0, index1 = channel1, ...)
    uhd::stream_args_t stream_args("fc32"); //complex floats
    stream_args.channels = channel_nums;
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    //setup streaming
    std::cout << std::endl;
    std::cout << boost::format(
        "Begin streaming %u samples, %f seconds in the future..."
    ) % total_num_samps % seconds_in_future << std::endl;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.num_samps = total_num_samps;
    stream_cmd.stream_now = false;
    stream_cmd.time_spec = uhd::time_spec_t(seconds_in_future);
    rx_stream->issue_stream_cmd(stream_cmd); //tells all channels to stream

    //meta-data will be filled in by recv()
    uhd::rx_metadata_t md;

    //allocate buffers to receive with samples (one buffer per channel)
    const size_t samps_per_buff = rx_stream->get_max_num_samps();
    std::vector<std::vector<std::complex<float> > > buffs(
        usrp->get_rx_num_channels(), std::vector<std::complex<float> >(samps_per_buff)
    );

    //create a vector of pointers to point to each of the channel buffers
    std::vector<std::complex<float> *> buff_ptrs;
    for (size_t i = 0; i < buffs.size(); i++) buff_ptrs.push_back(&buffs[i].front());

    //the first call to recv() will block this many seconds before receiving
    double timeout = seconds_in_future + 0.1; //timeout (delay before receive + padding)

    size_t num_acc_samps = 0; //number of accumulated samples
    FILE *out=fopen("bin0.bin","w");
    printf("starting\n");
    while(1)
    {
      //receive a single packet
      size_t num_rx_samps = rx_stream->recv(
					    buff_ptrs, samps_per_buff, md, timeout
					    );
      s->work(num_rx_samps,
	      (float *)&buffs[0].front(),
	      (float *)&buffs[1].front());
      
      //use a small timeout for subsequent packets
      timeout = 0.1;
      
      //handle the error code
      if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) break;
      if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
	throw std::runtime_error(str(boost::format(
						   "Receiver error %s"
						   ) % md.strerror()));
      }
      
      num_acc_samps += num_rx_samps;
    }
    
    if (num_acc_samps < total_num_samps) std::cerr << "Receive timeout before all samples received..." << std::endl;

    //finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
