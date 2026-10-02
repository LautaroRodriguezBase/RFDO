template <typename GS>
RFDO<GS>::RFDO(
    rf24_gpio_pin_t _cepin,
    rf24_gpio_pin_t _cspin,
    const uint64_t* seed,
    GS* data
) :
    RF24(_cepin, _cspin),
    seed(seed),
    myData(data),
    typenameTSize(sizeof(GS))
{
//
}

template <typename GS>
void RFDO<GS>::init(uint8_t* myAP, rf24_pa_dbm_e pow){
    // Probar si myAddrPos almacena solo el valor o esta ligado a la direccion
	this->myAddrPos = myAP;
    this->RF24::begin();

    this->RF24::setPALevel(pow);
    this->RF24::setPayloadSize(this->typenameTSize);

    this->startReading();
}

template <typename GS>
void RFDO<GS>::stopLisNstartWri(uint8_t devPos){
    this->RF24::stopListening();
	// It is not the same as ( (*(this->seed)) * (devPos + 1) ) because if devPos is 255, the SEED will be multiplied by 0(zero)
    this->RF24::openWritingPipe((*(this->seed)) + ( (*(this->seed)) * devPos));
    this->RF24::setRetries((devPos + 3) % 15, 15);
}

template <typename GS>
bool RFDO<GS>::sendT(GS* d, uint8_t mod){
    this->stopLisNstartWri(mod);
    bool sent = this->RF24::write(d, this->typenameTSize);

    this->startReading();

    return sent;
}

template <typename GS>
bool RFDO<GS>::sendT(uint8_t mod){
    return this->sendT(this->myData, mod);
}

template <typename GS>
GS* RFDO<GS>::getMyData(){
    return this->myData;
}

template <typename GS>
void RFDO<GS>::sendTToAll(std::vector<uint8_t>* devices, uint8_t secToTry){
    this->RF24::stopListening();
    this->RF24::setRetries(millis()%15, 15);

    uint64_t startTime = millis();
    uint8_t pos = 0;

    while( !devices->empty() && ((millis() - startTime) <= (secToTry * 1000)) ){

		// It is not the same as ( (*(this->seed)) * (devices->at(pos) + 1) ) because if devices->at(pos) is 255, the SEED will be multiplied by 0(zero)
        this->RF24::openWritingPipe( (*(this->seed)) + ( (*(this->seed)) * devices->at(pos)) );

        if( this->RF24::write(this->myData, this->typenameTSize) ){
            devices->erase(devices->begin()+pos);
            pos = (pos < (devices->size() - 1) ? (pos) : 0);

        }else{
            pos = (pos < (devices->size() - 1) ? (pos + 1) : 0);
        }
    }

    this->startReading();
}

template <typename GS>
void RFDO<GS>::startReading(){
	// It is not the same as ( (*(this->seed)) * ((*(this->myAddrPos)) + 1) ) because if (*(this->myAddrPos)) is 255, the SEED will be multiplied by 0(zero)
    this->RF24::openReadingPipe(1, (*(this->seed)) + ( (*(this->seed)) * (*(this->myAddrPos))));
    this->RF24::startListening();
}