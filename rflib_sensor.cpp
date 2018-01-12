#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include "rflib.h"

static RF24 *radio;
static uint64_t tx_address;

extern int rflib_init(RF24 **radio, uint16_t cepin, uint16_t cspin, uint8_t channel,
		      uint8_t delay, uint8_t retransmits);

int rflib_sensor_init(uint16_t cepin, uint16_t cspin, uint8_t channel,
		      uint64_t address, uint8_t delay, uint8_t retransmits)
{
	int ret = rflib_init(&radio, cepin, cspin, channel, delay, retransmits);
	tx_address = address;
	return ret;
}

void rflib_sensor_tx_pre(void)
{
	radio->powerUp();
	radio->openWritingPipe(tx_address);
	radio->stopListening();
}

int rflib_sensor_tx(const struct rflib_msg_t *msg, struct rflib_msg_t *ackmsg)
{
	uint8_t msg_size = msg->size & 0x1F;
	if (!radio->write(msg, msg_size + 1)) {
		return -1;
	}

	ackmsg->size = 0;
	memset(ackmsg->data, 0, RFLIB_MAX_MSGSIZE);
	if (radio->isAckPayloadAvailable()) {
		radio->read(ackmsg, sizeof(struct rflib_msg_t));
	}
	ackmsg->size &= 0x1F;

	return 0;
}

void rflib_sensor_tx_post(void)
{
	radio->powerDown();
}
