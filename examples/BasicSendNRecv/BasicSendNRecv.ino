#include <RFDO.h>

#ifdef ARDUINO_ESP32C3_DEV
	#define PIN_CE 3
	#define PIN_SS 7
#endif

/*
Octet values to avoid:
'0xaa' - '0b10101010'
'0x55' - '0b01010101'
'0x2a' - '0b00101010'
'0x15' - '0b00010101'

Nibble values to avoid:
'0x0a' - '0b00001010'
'0x05' - '0b00000101'
'0x02' - '0b00000010'
'0x01' - '0b00000001'

And all zeros or all ones (0xFFFFFFFFFF) and 0x000FFFFFFF

*/
// SEED is a uint64_t variable but MAX address size is 5 bytes(40 bit)
constexpr uint64_t SEED PROGMEM = 0x0000221133;

enum ADDR_POS : uint8_t{
	device1,
	device2
};

// If you upload the example in other device, change the device
// to avoid repeat the address
uint8_t myAddrPos = ADDR_POS::device1;
uint8_t otherAddrPos = ADDR_POS::device2;

int data = 10;

RFDO<int> rf(PIN_CE, PIN_SS, &SEED, &data); // The variable type of 'data' must be the same as RFDO<GS>

void setup(){
	Serial.begin(115200);

	// if the other device is an a long distance use
	// rf.init(myAddrPos, rf24_pa_dbm_e::RF24_PA_HIGH);
	// or RF24_PA_MAX if it's really far away
	// default is RF24_PA_LOW
	rf.init(&myAddrPos);
}

void loop(){
	if( rf.RF24::available() ){
		int input = 0;
		rf.RF24::read(&input, sizeof(int));
		Serial.printf("Recvived: %d from the other device.\n", input);
	}

	if(Serial.available() > 0){
		int data2 = 25;

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
				if(rf.sendT(&data2, otherAddrPos)){
					Serial.println("Success");
				}else{
					Serial.println("Fail");
				}
			break;

			case 'A':
				std::vector<uint8_t> d;
				d.push_back(otherAddrPos);
				data = 11;
				rf.sendTToAll(&d);
				if(d.empty()){
					Serial.println("Success");
				}else{
					Serial.println("Fail");
				}
			break;
		}
	}
}