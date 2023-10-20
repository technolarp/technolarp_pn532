#include "technolarp_pn532.h"

// init pn532
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

M_pn532::M_pn532()
{
  Serial.println("init RFID");
  delay(500);
  nfc.begin();
  Serial.println("NDEF Ready");
  
  // ACTIVATE / DEACTIVATE reading
  previousMillisActivateReading=millis();
  intervalActivateReading=500;

  previousMillisDeactivateReading=previousMillisActivateReading;
  intervalDeactivateReading=50;

  previousMillisTagPresent=previousMillisActivateReading;
  intervalTagPresent=2000;
}


bool M_pn532::readUID(uint16_t timeout)
{
	if (readingRFID)
	{
		if (nfc.tagPresent(timeout))
		{
			ESP.wdtDisable();
			NfcTag tag = nfc.read();
			ESP.wdtEnable(2000);
			
			Serial.print("uid length: ");
			Serial.println(tag.getUidLength());
			
			tagPresent=true;
			previousMillisTagPresent=millis();
			
			// copy uid
			uidLength=tag.getUidLength();
			tag.getUid(uid, uidLength);
			
			return(true);
		}
	}
	
	return(false);
}

void M_pn532::updateRFID()
{
  // DEACTIVATE RFID READING
  if(millis() - previousMillisDeactivateReading > intervalDeactivateReading)
  {
    previousMillisDeactivateReading = millis();
    
    if (readingRFID)
    {
      readingRFID=false;
      //Serial.println("DE-activate RFID reading");
    }
  }

  // ACTIVATE RFID READING
  if(millis() - previousMillisActivateReading > intervalActivateReading)
  {
    previousMillisActivateReading = millis();
    previousMillisDeactivateReading = previousMillisActivateReading;
    
    if (!tagPresent)
    {
      if (!readingRFID)
      {
        readingRFID=true;
        //Serial.println("activate RFID reading");
      }
    }
    
  }

  // Delay after a tag was red
  if(millis() - previousMillisTagPresent > intervalTagPresent)
  {
    previousMillisTagPresent = millis();
    tagPresent=false;
  }
}
