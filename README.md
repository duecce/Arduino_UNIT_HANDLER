# Arduino_UNIT_HANDLER
Unit Handler is a library that could help you to set a frequency for analog to digital converter processing. You can handle more than one function and implement them separatly.
## AD8232.ino 
This file is an example that use Unit_Handler and RealTime_IIR library to do the analog to digital converting and a real-time filtering of the signal read from AD8232. AD8232 is a single-lead, heart rate monitor front-end.
## Usage example:
Arduino_UNIT_HANDLER uses singleton design pattern, so to get an instance of this class you have to call the static member getInstance();
```
...
unitHandler = UNIT_HANDLER::getInstance( ); 
...
```
Now unitHandler is an instance to the Arduino_UNIT_HANDLER class. You can get the same instance in every part of the code with no risk to overwrite or change the object. 
After that, you can connect a function to unitHandler and sey to it: 'execute it every 10ms, or 1s, or another sample time'. 
```
// Connect printRawECG to unitHandler and execute it every 1/100 seconds
unitHandler->add(ECG_FS, printRawECG);  
// Connect leadsOffDetection to unitHandler and execute it every 1/1 second
unitHandler->add(LEADS_OFF_FS, leadsOffDetection);  
```
To allow unitHandler to do its job you have to write this in loop function:
```
void loop( ) {
    ...
    unitHandler->_check( );
    ...
}
```
In AD8232.ino you can view an example of this implementation.
