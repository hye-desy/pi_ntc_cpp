#include <atomic>
#include <thread>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <wiringPi.h>
#include <wiringPiSPI.h>

using std::uint32_t;
// default spi1, sc 0,1
const std::vector<uint32_t> selectChip{0,1};

#define SLEEPTIME  5

#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0


float VoltageCal(int value){
    float V=float(value)/1024.0*3.3;
    return V;
}

float ResisCal(int value){
    float V=VoltageCal(value);
    float R;
    if(V>0.1&&V<3.2)
	R=12000*(3.3-V)/V;
    else
	R=1;
    return R;
}

float ThermistorCal(int value){
    float A=0.9017477480e-3;
    float B=2.489190310e-4;
    float C=2.043213857e-7;

    float R=ResisCal(value);

    float T=1.0/(A+B*log(R)+C*pow(log(R),3))-273;

    return T;
}

void spiSetup (int spiChannel){
    int myFd=0;
    if ((myFd = wiringPiSPISetup (spiChannel, 1000000)) < 0){
	std::cout<<"SPI Setup Error"<<std::endl;
	exit (EXIT_FAILURE) ;
    }
}

int read_adc(int spiChannel,int channelConfig,int analogChannel){
    if(analogChannel<0 || analogChannel>7)
	return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}


void read_ntc(std::atomic_uchar &run){

    run = 1;

    wiringPiSetup () ;

    uint32_t value;
    float volt,resi,temp;
    int channelConfig=CHAN_CONFIG_SINGLE;

    uint32_t i = 0;  
    uint32_t ntot=0;

    time_t timev;

    while(run){

	spiSetup(selectChip[i]);

	//  read ntc
	for (int chan=0; chan<8; chan++) {

	    time(&timev);
	    value = read_adc(selectChip[i],channelConfig,chan);

	    volt=VoltageCal(value);
	    resi=ResisCal(value);
	    temp=ThermistorCal(value);

	    if(value>0) 
		std::cout<<ctime(&timev)<<" sc: "<<selectChip[i]<<", ch: "<<chan<<", adc: "<<value<<", cal: "<<volt<<", "<<resi<<", "<<temp<<std::endl;

	}
	i++;
	if(i == selectChip.size()){ 
	    std::this_thread::sleep_for (std::chrono::seconds(SLEEPTIME));
	    i=0;
	    ntot++;
	}

    }

}


int main (){

    std::atomic_uchar thread_run;
    std::thread reader(read_ntc, std::ref(thread_run));

    (void) getchar();
    thread_run = 0;
    reader.join();

    return 0;
}
