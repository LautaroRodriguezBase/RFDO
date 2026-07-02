#include <RFDO.h>

// ESP32 C3 pinout
#define CE_PIN 10
#define CSN_PIN 7

// ADDR is a uint64_t variable
#define ADDR_nRF1  0xFEDCBA9876543210
#define ADDR_nRF2  0x1

enum ADDR_POS : uint8_t{
	device1,
	device2
};

constexpr uint64_t addresses[] PROGMEM = {
	ADDR_nRF1,
	ADDR_nRF2
};

#define ADDRS_SIZE (sizeof(addresses)/sizeof(addresses[0]))

// If you upload the example in other device, change the device
// to avoid repeat the address
uint8_t myAddrPos = ADDR_POS::device1;
uint8_t otherAddrPos = ADDR_POS::device2;

int data = 10;
RFDO<int> rf(CE_PIN, CSN_PIN, addresses, ADDRS_SIZE, data);

void setup(){
	Serial.begin(115200);

	// if the other device is an a long distance use
	// rf.init(myAddrPos, RF24_PA_HIGH);
	// or RF24_PA_MAX if it's really far away
	rf.init(myAddrPos);
}

void loop(){
	if( rf.RF24::available() ){
		int input = 0;
		rf.RF24::read(&input, sizeof(int));
		Serial.printf("Recvived: %d from the other device.\n", input);
	}

	if(Serial.available() > 0){
		char read = Serial.read();
		if(read == 'S'){
			Serial.println("Sending...");
			// Send 'data'
			if(rf.sendT(otherAddrPos)){
				Serial.println("Success");
			}else{
				Serial.println("Fail");
			}
		}else if(read == 'D'){
			Serial.println("Sending other data...");
			// Send other data
			int data2 = 25;
			if(rf.sendT(data2, otherAddrPos)){
				Serial.println("Success");
			}else{
				Serial.println("Fail");
			}
		}
	}
}