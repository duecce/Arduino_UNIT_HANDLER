#include "RealTime_IIR.hpp"
#include "Unit_Handler.hpp"

#define ECG_FS            500 // [Hz]
#define LEADS_OFF_FS      1   // [Hz]

#define ECG_PIN           A0
#define LO_P_PIN          10  // leads off detection LO + Pin
#define LO_M_PIN          11  // leads off detection LO - Pin
#define ECG_BASELINE      0

bool leadsOff;
float input;

// Butterworth IIR filter coefficient 
// [frequency sample = 100Hz, second-order, low-cut = 0.5 Hz, high-cut = 40 Hz, band-pass]
// Matlab command:
// fs=100; [b, a] = butter(2, [0.5, 40]/(fs/2), 'bandpass');
float a[] = {
  1,  
  -0.814719572580634,  
  -0.861224468755414,  
  0.283142071794629, 
  0.395292960066178
};

float b[] = {
    0.624447671548746,  
    0, 
    -1.24889534309749, 
    0, 
    0.624447671548746
  };

RT_IIR<float> iir = RT_IIR<float>(7, 7, b, a);
UNIT_HANDLER *unitHandler;

void setup( ) {
  Serial.begin(2000000);
  // Setup pin for leads off detection LO +
  pinMode(LO_P_PIN, INPUT); 
  // Setup pin for leads off detection LO -
  pinMode(LO_M_PIN, INPUT); 
  // UNIT_HANDLER uses singleton design pattern, only 1 instance can be created
  unitHandler = UNIT_HANDLER::getInstance( ); 
  // Add printRawECG to unitHandler with frequency ECG_FS
  unitHandler->add(ECG_FS, printRawECG);      
  // Add leadsOffDetection to unitHandler with frequency LEADS_OFF_FS
  unitHandler->add(LEADS_OFF_FS, leadsOffDetection);  
  leadsOff = false;
}

// Check leads off+ and leads off-
// Frequency sample: 1 Hz 
// Note: elapsed value is the real elapsed time from an execution to another
void leadsOffDetection ( unsigned int elapsed ) {
  if ((digitalRead(LO_P_PIN) == 1) || (digitalRead(LO_M_PIN) == 1)) {
    leadsOff = true;
  } else {
    leadsOff = false;
  }
}

// Read ECG analog value from ECG_PIN
// Frequency sample: 100 Hz
// Note: elapsed value is the real elapsed time from an execution to another
void printRawECG ( unsigned int elapsed ) {
    if(leadsOff == false) {
      input = iir.add(analogRead(ECG_PIN) - ECG_BASELINE);
    }
    // Serial.println is very slow but this is only an example
    // you should to store the values and view them after
    Serial.println(String(input));
}

void loop ( ) {
  unitHandler->_check( );
}
