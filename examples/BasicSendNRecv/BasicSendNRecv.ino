#include <RFDO.h>

// ESP32 C3 pinout
#define CE_PIN 10
#define CSN_PIN 7

// SEED is a uint64_t variable
#define SEED 0x00221133

enum ADDR_POS : uint8_t{
	device1,
	device2
};

// If you upload the example in other device, change the device
// to avoid repeat the address
uint8_t myAddrPos = ADDR_POS::device1;
uint8_t otherAddrPos = ADDR_POS::device2;

int data = 10;
RFDO<int> rf(CE_PIN, CSN_PIN, SEED, data); // The variable type of 'data' must be the same as RFDO<GS>

void setup(){
	Serial.begin(115200);

	// if the other device is an a long distance use
	// rf.init(myAddrPos, rf24_pa_dbm_e::RF24_PA_HIGH);
	// or RF24_PA_MAX if it's really far away
	rf.init(&myAddrPos);
}

void loop(){
	if( rf.RF24::available() ){
		int input = 0;
		rf.RF24::read(&input, sizeof(int));
		Serial.printf("Recvived: %d from the other device.\n", input);
	}

	if(Serial.available() > 0){
		char read = Serial.read();
		switch (read){
			case 'S':
				Serial.println("Sending...");
				// Send 'data'
				if(rf.sendT(otherAddrPos)){
					Serial.println("Success");
				}else{
					Serial.println("Fail");
				}
			break;

			case 'D':
				Serial.println("Sending other data...");
				// Send other data
				int data2 = 25;
				if(rf.sendT(data2, otherAddrPos)){
					Serial.println("Success");
				}else{
					Serial.println("Fail");
				}
			break;

			case 'A':
				std::vector<uint8_t> d;
				d.push_back(otherAddrPos);
				data = 11;
			break;
		}
	}
}