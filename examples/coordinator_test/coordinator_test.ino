#include <rflib.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

const uint64_t pipes[1] = { 0xF0F0F0F0E1LL };

static struct rflib_msg_t ackmsg = {
  9, "BAM OIDA"
};

void setup(void)
{
  Serial.begin(57600);
  printf_begin();

  // Give some time 
  delay(100);
  if (rflib_coordinator_init(9, 10, 0, pipes, 1, 15, 15) < 0) {
    printf("Init failed :(\n\r");
    abort();
  } 
}

static bool ackmsg_loaded = false;
static void toggle_ackmsg(void)
{
  printf("toggling\n\r");
  if (ackmsg_loaded) {
    rflib_coordinator_clear_reply();
    ackmsg_loaded = false;
  } else {
    rflib_coordinator_set_reply(0, &ackmsg);
    ackmsg_loaded = true;
  }
}

static rflib_msg_t msg;
static uint8_t runcnt = 0;
void loop(void)
{

  if (runcnt % 5 == 0) {
    toggle_ackmsg();
  }
  runcnt++;

  if (rflib_coordinator_available() == 0) {
    rflib_coordinator_read(&msg);
    printf("Received :), msglen = %d, msg = \"%s\"\n\r", msg.size, msg.data);
  }
  
  delay(1000);
}
