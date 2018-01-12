#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include "rflib.h"

#define RFLIB_MAX_CHANNEL 125
#define RFLIB_MAX_DELAY 15
#define RFLIB_MAX_RETRANSMITS 15

int rflib_init(RF24 **radio, uint16_t cepin, uint16_t cspin, uint8_t channel,
	       uint8_t delay, uint8_t retransmits)
{
	if (channel > 125 || delay > 15 || retransmits > 15) {
		return -1;
	}

	RF24 *r = new RF24(cepin, cspin);
	if (!r->begin()) {
		//FIXME: Object should be freed but destructor is missing...
		//delete r;
		return -2;
	}

	if (r->getPayloadSize() != sizeof(struct rflib_msg_t)) {
		//FIXME: Object should be freed but destructor is missing...
		//delete r;
		return -3;
	}

	r->setChannel(channel);
	r->enableDynamicPayloads();
	r->enableAckPayload();
	r->setRetries(delay, retransmits);
	*radio = r;

	return 0;
}
