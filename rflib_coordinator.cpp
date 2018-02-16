#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include "rflib.h"

static RF24 *radio;
static uint8_t n_rx_addresses;

extern int rflib_init(RF24 **radio, uint16_t cepin, uint16_t cspin, uint8_t channel,
		      uint8_t delay, uint8_t retransmits);

int rflib_coordinator_init(uint16_t cepin, uint16_t cspin, uint8_t channel,
			   const uint64_t *addresses, uint8_t n_addresses,
			   uint8_t delay, uint8_t retransmits)
{
	int ret = rflib_init(&radio, cepin, cspin, channel, delay, retransmits);
	if (ret != 0) {
		return ret;
	}

	if (n_addresses > RFLIB_MAX_RX_ADDRESSES) {
		return -4;
	}

	n_rx_addresses = n_addresses;

	int i;
	for (i = 0; i < n_addresses; i++) {
		radio->openReadingPipe(i + 1, addresses[i]);
	}

	radio->startListening();

	return 0;
}

int rflib_coordinator_available(void)
{
	if (radio->available()) {
		return 0;
	}

	return -1;
}

void rflib_coordinator_read(struct rflib_msg_t *msg)
{
	msg->size = 0;
	memset(msg->data, 0, RFLIB_MAX_MSGSIZE);
	radio->read(msg, sizeof(struct rflib_msg_t));
	msg->size &= 0x1F;
}

int rflib_coordinator_set_reply(uint8_t address_idx, struct rflib_msg_t *msg)
{
	if (address_idx >= n_rx_addresses) {
		return -1;
	}

	uint8_t msg_size = msg->size & 0x1F;
	/* TODO: check if these are persistent */
	radio->flush_tx();
	radio->writeAckPayload(address_idx + 1, msg, msg_size + 1);

	return 0;
}

void rflib_coordinator_clear_reply(void)
{
	radio->flush_tx();
}
