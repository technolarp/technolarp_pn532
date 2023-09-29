#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


class M_pn532
{
  private:
	bool readingRFID = true;
	bool tagPresent = true;

	uint8_t success;
	

	uint32_t previousMillisActivateReading;
	uint32_t intervalActivateReading;

	uint32_t previousMillisDeactivateReading;
	uint32_t intervalDeactivateReading;

	uint32_t previousMillisTagPresent;
	uint32_t intervalTagPresent;

	
  public:
	M_pn532();
	
	bool readUID(uint16_t timeout);
	void updateRFID();
	
	uint8_t uid[7] = { 0, 0, 0, 0, 0, 0, 0 };
	uint8_t uidLength;
};
