//RadioFrecuency Data Object
#ifndef RFDO_h
#define RFDO_h

/**
 * Copyright 2026 Lautaro Ezequiel Rodriguez Base
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <vector>

#define RFDO_VERSION 2 // v0.0.2

template <typename GS>
class RFDO: public RF24{
    private:
        const uint64_t* addresses;
        /// MAX 255
        uint8_t numberAddresses;
        GS* myData;
        uint8_t typenameTSize;
        /// object address position
        uint8_t* myAddrPos;

        void stopLisNstartWri(uint8_t mod);

    public:
        RFDO(rf24_gpio_pin_t _cepin, rf24_gpio_pin_t _cspin, const uint64_t* addresses, uint8_t numAddresses, GS& data);
        void init(uint8_t& myAP, rf24_pa_dbm_e pow = RF24_PA_LOW);

        /**
         * Sends the GS to the indicate device.
         */
        bool sendT(GS& d, uint8_t device);
        bool sendT(uint8_t device);

        /**
         * It sends an action to all necessary devices, avoiding the opening and closing of pipes.
         * When an action is successfully submitted, it is deleted from the vector.
         * @param devices is the pointer to the vector that contains all the address positions to which we need to send the action.
         * @param secToTry son los segundos que intentará seguir enviando la acción.
         * @note you must check if the vector is empty.
         */
        void sendTToAll(std::vector<uint8_t>* devices, uint8_t secToTry = 2);

        void startReading();

        /**
         * @return a pointer to myData.
         */
        GS* getMyData();
};

#include "RFDO.hpp"

#endif
