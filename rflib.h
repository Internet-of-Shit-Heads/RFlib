#ifndef __RFLIB_H__
#define __RFLIB_H__

#include <stdint.h>

#define RFLIB_MAX_MSGSIZE 31
struct rflib_msg_t {
	uint8_t size;
	uint8_t data[RFLIB_MAX_MSGSIZE];
} __attribute__((packed));

/* Limited by the number of ACK payloads that can be pending at the same time. */
#define RFLIB_MAX_RX_ADDRESSES 3

int rflib_sensor_init(uint16_t cepin, uint16_t cspin, uint8_t channel,
		      uint64_t address, uint8_t delay, uint8_t retransmits);
void rflib_sensor_tx_pre(void);
int rflib_sensor_tx(const struct rflib_msg_t *msg, struct rflib_msg_t *ackmsg);
void rflib_sensor_tx_post(void);

int rflib_coordinator_init(uint16_t cepin, uint16_t cspin, uint8_t channel,
			   const uint64_t *addresses, uint8_t n_addresses,
			   uint8_t delay, uint8_t retransmits);
int rflib_coordinator_available(void);
void rflib_coordinator_read(struct rflib_msg_t *msg);
int rflib_coordinator_set_reply(uint8_t address_idx, struct rflib_msg_t *msg);
void rflib_coordinator_clear_reply(void);

#endif /* __RFLIB_H__ */
