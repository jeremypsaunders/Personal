#include <UnixTime.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Wire.h>
#include <EEPROM.h>

int testvalue;

//PIN ASSIGNMENTS
const int relay1_out = 13;
const int relay2_out = 12;
const int relay3_out = 27;
const int relay4_out = 33;

const int button1 = A4;
const int button2 = A5;
const int button3 = 5;
const int button4 = 18;

const int relay1_in = A3;
const int relay2_in = A2;
const int relay3_in = A1;
const int relay4_in = A0;

//GLOBAL DATA
unsigned long loop_rate;
UnixTime current_unix_timestamp(0); //"0" is the time zone
unsigned long current_UNIX_time = 0;
tmElements_t rtc_time;
unsigned long rtc_seconds_after_midnight;
unsigned long current_seconds_after_midnight;
unsigned long PreviousMillis;
long signalRSSI;
int signalquality;
bool main_loop_firstcall = true;
bool Blynk_Enter_Connected_Mode_rqst;
bool Blynk_Enter_Connected_Mode;


//PANEL CONTROL
String device_indicator;
String mode_indicator;
String ontime_indicator;
String offtime_indicator;
float mode_indicator_blink;
float ontime_indicator_blink;
float offtime_indicator_blink;

int display_device;
int timer_segment;

int D1_ON_HR;
int D2_ON_HR;
int D3_ON_HR;
int D4_ON_HR;

int D1_ON_MIN;
int D2_ON_MIN;
int D3_ON_MIN;
int D4_ON_MIN;

int D1_OFF_HR;
int D2_OFF_HR;
int D3_OFF_HR;
int D4_OFF_HR;

int D1_OFF_MIN;
int D2_OFF_MIN;
int D3_OFF_MIN;
int D4_OFF_MIN;

bool timed_out;
unsigned long last_buttonpress_time;
bool D1_Mode_edit;
bool D1_Daily_edit;
bool D1_Cycle_edit;
bool DeviceButtonPressed;
bool ModeButtonPressed;
bool SetButtonPressed;
bool IncButtonPressed;
bool DecButtonPressed;
bool ConfirmButtonPressed;

bool D1_ModeCmd_Ind;
bool D2_ModeCmd_Ind;
bool D3_ModeCmd_Ind;
bool D4_ModeCmd_Ind;

bool D1_Daily_Timer_ON_Time_Ind;
bool D2_Daily_Timer_ON_Time_Ind;
bool D3_Daily_Timer_ON_Time_Ind;
bool D4_Daily_Timer_ON_Time_Ind;

bool D1_Daily_Timer_OFF_Time_Ind;
bool D2_Daily_Timer_OFF_Time_Ind;
bool D3_Daily_Timer_OFF_Time_Ind;
bool D4_Daily_Timer_OFF_Time_Ind;

bool D1_Cycle_Timer_ON_Length_Ind;
bool D2_Cycle_Timer_ON_Length_Ind;
bool D3_Cycle_Timer_ON_Length_Ind;
bool D4_Cycle_Timer_ON_Length_Ind;

bool D1_Cycle_Timer_OFF_Length_Ind;
bool D2_Cycle_Timer_OFF_Length_Ind;
bool D3_Cycle_Timer_OFF_Length_Ind;
bool D4_Cycle_Timer_OFF_Length_Ind;

//DEVICE SPECIFIC DATA
String D1_Name;
String D2_Name;
String D3_Name;
String D4_Name;

String D1_Name_prev;
String D2_Name_prev;
String D3_Name_prev;
String D4_Name_prev;

int D1_ModeCmdRqst;
int D2_ModeCmdRqst;
int D3_ModeCmdRqst;
int D4_ModeCmdRqst;

int D1_ModeCmdRqst_prev;
int D2_ModeCmdRqst_prev;
int D3_ModeCmdRqst_prev;
int D4_ModeCmdRqst_prev;

int D1_ModeCmd;
int D2_ModeCmd;
int D3_ModeCmd;
int D4_ModeCmd;

int D1_ModeCmd_prev;
int D2_ModeCmd_prev;
int D3_ModeCmd_prev;
int D4_ModeCmd_prev;

bool D1_RelayCmd;
bool D2_RelayCmd;
bool D3_RelayCmd;
bool D4_RelayCmd;

bool D1_RelayCmd_prev;
bool D2_RelayCmd_prev;
bool D3_RelayCmd_prev;
bool D4_RelayCmd_prev;

//Check and send relay coil states
bool D1_Coil_State;
bool D2_Coil_State;
bool D3_Coil_State;
bool D4_Coil_State;

bool D1_Coil_State_prev;
bool D2_Coil_State_prev;
bool D3_Coil_State_prev;
bool D4_Coil_State_prev;

//Check relay faults
bool D1_Relay_Fault;
bool D2_Relay_Fault;
bool D3_Relay_Fault;
bool D4_Relay_Fault;

bool D1_Relay_Fault_prev;
bool D2_Relay_Fault_prev;
bool D3_Relay_Fault_prev;
bool D4_Relay_Fault_prev;

unsigned long Relay1_last_nofault_time;
unsigned long Relay2_last_nofault_time;
unsigned long Relay3_last_nofault_time;
unsigned long Relay4_last_nofault_time;

//Check and send manual toggle button presses
bool D1_Toggle_Button;
bool D2_Toggle_Button;
bool D3_Toggle_Button;
bool D4_Toggle_Button;

bool D1_Toggle_Button_prev;
bool D2_Toggle_Button_prev;
bool D3_Toggle_Button_prev;
bool D4_Toggle_Button_prev;

bool D1_toggled;
bool D2_toggled;
bool D3_toggled;
bool D4_toggled;

//Check Hide Device
bool D1_HideDevice;
bool D2_HideDevice;
bool D3_HideDevice;
bool D4_HideDevice;

bool D1_HideDevice_prev;
bool D2_HideDevice_prev;
bool D3_HideDevice_prev;
bool D4_HideDevice_prev;

//Daily Timer
String D1_Daily_Timer_ON_Time_string;
String D2_Daily_Timer_ON_Time_string;
String D3_Daily_Timer_ON_Time_string;
String D4_Daily_Timer_ON_Time_string;

unsigned long D1_Daily_Timer_ON_Time;
unsigned long D2_Daily_Timer_ON_Time;
unsigned long D3_Daily_Timer_ON_Time;
unsigned long D4_Daily_Timer_ON_Time;

unsigned long D1_Daily_Timer_ON_Time_prev;
unsigned long D2_Daily_Timer_ON_Time_prev;
unsigned long D3_Daily_Timer_ON_Time_prev;
unsigned long D4_Daily_Timer_ON_Time_prev;

String D1_Daily_Timer_OFF_Time_string;
String D2_Daily_Timer_OFF_Time_string;
String D3_Daily_Timer_OFF_Time_string;
String D4_Daily_Timer_OFF_Time_string;

unsigned long D1_Daily_Timer_OFF_Time;
unsigned long D2_Daily_Timer_OFF_Time;
unsigned long D3_Daily_Timer_OFF_Time;
unsigned long D4_Daily_Timer_OFF_Time;

unsigned long D1_Daily_Timer_OFF_Time_prev;
unsigned long D2_Daily_Timer_OFF_Time_prev;
unsigned long D3_Daily_Timer_OFF_Time_prev;
unsigned long D4_Daily_Timer_OFF_Time_prev;

int D1_Daily_Timer_Fault_prev;
int D2_Daily_Timer_Fault_prev;
int D3_Daily_Timer_Fault_prev;
int D4_Daily_Timer_Fault_prev;

//Cycle Timer
bool D1_ResetCycleTimer;
bool D2_ResetCycleTimer;
bool D3_ResetCycleTimer;
bool D4_ResetCycleTimer;

unsigned long D1_Cycle_Timer_ON_Counts;
unsigned long D2_Cycle_Timer_ON_Counts;
unsigned long D3_Cycle_Timer_ON_Counts;
unsigned long D4_Cycle_Timer_ON_Counts;

unsigned long D1_Cycle_Timer_OFF_Counts;
unsigned long D2_Cycle_Timer_OFF_Counts;
unsigned long D3_Cycle_Timer_OFF_Counts;
unsigned long D4_Cycle_Timer_OFF_Counts;

unsigned long D1_Cycle_ON_current_count;
unsigned long D2_Cycle_ON_current_count;
unsigned long D3_Cycle_ON_current_count;
unsigned long D4_Cycle_ON_current_count;

unsigned long D1_Cycle_OFF_current_count;
unsigned long D2_Cycle_OFF_current_count;
unsigned long D3_Cycle_OFF_current_count;
unsigned long D4_Cycle_OFF_current_count;

String D1_Cycle_Timer_ON_Length_string;
String D2_Cycle_Timer_ON_Length_string;
String D3_Cycle_Timer_ON_Length_string;
String D4_Cycle_Timer_ON_Length_string;

unsigned long D1_Cycle_Timer_ON_Length;
unsigned long D2_Cycle_Timer_ON_Length;
unsigned long D3_Cycle_Timer_ON_Length;
unsigned long D4_Cycle_Timer_ON_Length;

unsigned long D1_Cycle_Timer_ON_Length_prev;
unsigned long D2_Cycle_Timer_ON_Length_prev;
unsigned long D3_Cycle_Timer_ON_Length_prev;
unsigned long D4_Cycle_Timer_ON_Length_prev;

String D1_Cycle_Timer_OFF_Length_string;
String D2_Cycle_Timer_OFF_Length_string;
String D3_Cycle_Timer_OFF_Length_string;
String D4_Cycle_Timer_OFF_Length_string;

unsigned long D1_Cycle_Timer_OFF_Length;
unsigned long D2_Cycle_Timer_OFF_Length;
unsigned long D3_Cycle_Timer_OFF_Length;
unsigned long D4_Cycle_Timer_OFF_Length;

unsigned long D1_Cycle_Timer_OFF_Length_prev;
unsigned long D2_Cycle_Timer_OFF_Length_prev;
unsigned long D3_Cycle_Timer_OFF_Length_prev;
unsigned long D4_Cycle_Timer_OFF_Length_prev;

bool D1_Cycle_Timer_Fault_prev;
bool D2_Cycle_Timer_Fault_prev;
bool D3_Cycle_Timer_Fault_prev;
bool D4_Cycle_Timer_Fault_prev;



/////////////////////PRELIMINARY DISPLAYS CODE USING BUTTON PRESSED FUNCTIONS//////////////////////////////////////////
//need to write to display from EEPROM on bootup, then again on first blynk connect
/*
if ( ((millis() - last button pressed time) > 300000) || displays_off == false ) { //300000 = 5min
  displays_off = true;
  
  //Command all indicators to be off here

  //write current display device real values back to indicators
  switch (display_device) {  
    case 0: //Device 1
      mode_indicator = D1_ModeCmd;
      if(D1_ModeCmd == 2) { //DAILY TIMER
        ontime_indicator = D1_Daily_Timer_ON_Time;
        offtime_indicator = D1_Daily_Timer_OFF_Time;      
      }
      if(D1_ModeCmd == 3) { //CYCLE TIMER
        ontime_indicator = D1_Cycle_Timer_ON_Length;
        offtime_indicator = D1_Cycle_Timer_OFF_Length;      
      }  
    case 1: //Device 2
      mode_indicator = D2_ModeCmd;
      if(D2_ModeCmd == 2) { //DAILY TIMER
        ontime_indicator = D2_Daily_Timer_ON_Time;
        offtime_indicator = D2_Daily_Timer_OFF_Time;      
      }
      if(D2_ModeCmd == 3) { //CYCLE TIMER
        ontime_indicator = D2_Cycle_Timer_ON_Length;
        offtime_indicator = D2_Cycle_Timer_OFF_Length;      
      }        
    case 2: //Device 3
      mode_indicator = D3_ModeCmd;
      if(D3_ModeCmd == 2) { //DAILY TIMER
        ontime_indicator = D3_Daily_Timer_ON_Time;
        offtime_indicator = D3_Daily_Timer_OFF_Time;      
      }
      if(D3_ModeCmd == 3) { //CYCLE TIMER
        ontime_indicator = D3_Cycle_Timer_ON_Length;
        offtime_indicator = D3_Cycle_Timer_OFF_Length;      
      }  
    case 3: //Device 4
      mode_indicator = D4_ModeCmd;
      if(D4_ModeCmd == 2) { //DAILY TIMER
        ontime_indicator = D4_Daily_Timer_ON_Time;
        offtime_indicator = D4_Daily_Timer_OFF_Time;      
      }
      if(D4_ModeCmd == 3) { //CYCLE TIMER
        ontime_indicator = D4_Cycle_Timer_ON_Length;
        offtime_indicator = D4_Cycle_Timer_OFF_Length;      
      }         
  }
  
  //turn off all blinks
  mode_indicator_blink = 0;
  ontime_indicator_blink = 0;
  offtime_indicator_blink = 0;
}

void (ConfirmButtonPressed) {
  if (displays_off == false) { //Disable this button if displays are turned off
    last_buttonpress_time = millis();

    if (mode_indicator_blink == 2) {
      mode_indicator_blink == 0;

      //Write all mode cmd requests just for simplicity of code
      D1_ModeCmdRqst = D1_ModeCmd_Ind;
      D2_ModeCmdRqst = D2_ModeCmd_Ind;
      D3_ModeCmdRqst = D3_ModeCmd_Ind;
      D4_ModeCmdRqst = D4_ModeCmd_Ind;

      //write to Blynk server here or in main loop?
    }
    if (ontime_indicator_blink == 2)
  }
}

//When the panel DEVICE button is pressed
void (DeviceButtonPressed) {
  last_buttonpress_time = millis();
  if (displays_off == true) {
    //turn on all indicators command
    display_device = 0;
    displays_off = false;
  }
  else {
    if ( display_device == 3 ) { 
      display_device = 0;
    }
    else {
      display_device++;
    }
  }

  switch (display_device) {
  case 0: //Device 1
    device_indicator = "D1_Name"; //Write D1_Name from EEPROM at bootup in case there is no Blynk server. Also write this to the display at bootup.
    mode_indicator = D1_ModeCmd; //Write D1_ModeCmd_Ind from D1_ModeCmd value stored in EEPROM at bootup. Also write this to the display at bootup.
    switch (D1_ModeCmd) {
    case 0:
      ontime_indicator = "----";
      offtime_indicator = "----";
      break;
    case 1:
      ontime_indicator = "----";
      offtime_indicator = "----";      
      break;
    case 2: 
      ontime_indicator = D1_Daily_Timer_ON_Time;
      offtime_indicator = D1_Daily_Timer_ON_Time;
      break;
    case 3:
      ontime_indicator = D1_Cycle_Timer_ON_Length;
      offtime_indicator = D1_Cycle_Timer_OFF_Length;
      break;     
    }
    break;

  case 1: //Device 2
    device_indicator = "D2_Name"; //Write D2_Name from EEPROM at bootup in case there is no Blynk server. Also write this to the display at bootup.
    mode_indicator = D2_ModeCmd; //Write D2_ModeCmd_Ind from D1_ModeCmd value stored in EEPROM at bootup. Also write this to the display at bootup.
    switch (D2_ModeCmd) {
    case 0:
      ontime_indicator = "----";
      offtime_indicator = "----";
      break;
    case 1:
      ontime_indicator = "----";
      offtime_indicator = "----";      
      break;
    case 2: 
      ontime_indicator = D2_Daily_Timer_ON_Time;
      offtime_indicator = D2_Daily_Timer_ON_Time;
      break;
    case 3:
      ontime_indicator = D2_Cycle_Timer_ON_Length;
      offtime_indicator = D2_Cycle_Timer_OFF_Length;
      break;     
    }
    break;
  case 2: //Device 3
    device_indicator = "D3_Name"; //Write D3_Name from EEPROM at bootup in case there is no Blynk server. Also write this to the display at bootup.
    mode_indicator = D3_ModeCmd; //Write D3_ModeCmd_Ind from D1_ModeCmd value stored in EEPROM at bootup. Also write this to the display at bootup.
    switch (D3_ModeCmd) {
    case 0:
      ontime_indicator = "----";
      offtime_indicator = "----";
      break;
    case 1:
      ontime_indicator = "----";
      offtime_indicator = "----";      
      break;
    case 2: 
      ontime_indicator = D3_Daily_Timer_ON_Time;
      offtime_indicator = D3_Daily_Timer_ON_Time;
      break;
    case 3:
      ontime_indicator = D3_Cycle_Timer_ON_Length;
      offtime_indicator = D3_Cycle_Timer_OFF_Length;
      break;     
    }
    break;
  case 3: //Device 4
    device_indicator = "D4_Name"; //Write D4_Name from EEPROM at bootup in case there is no Blynk server. Also write this to the display at bootup.
    mode_indicator = D4_ModeCmd; //Write D4_ModeCmd_Ind from D1_ModeCmd value stored in EEPROM at bootup. Also write this to the display at bootup.
    switch (D4_ModeCmd) {
    case 0:
      ontime_indicator = "----";
      offtime_indicator = "----";
      break;
    case 1:
      ontime_indicator = "----";
      offtime_indicator = "----";      
      break;
    case 2: 
      ontime_indicator = D4_Daily_Timer_ON_Time;
      offtime_indicator = D4_Daily_Timer_ON_Time;
      break;
    case 3:
      ontime_indicator = D4_Cycle_Timer_ON_Length;
      offtime_indicator = D4_Cycle_Timer_OFF_Length;
      break;     
    }
    break;
  }

  mode_indicator_blink = 0;
  ontime_indicator_blink = 0;
  offtime_indicator_blink = 0;
}

void (ModeButtonPressed) {
  if (displays_off == false) {
    last_buttonpress_time = millis();

    switch (display_device) {
   
    case 0: //DEVICE 1

      switch (D1_ModeCmd_Ind) ( //Cycle through the different modes, also writing time indicators 
        case 0: //OFF-->ON
          D1_ModeCmd_Ind = 1; //write next mode
          ontime_indicator = "----";
          offtime_indicator = "----";
          if (D1_ModeCmd_Ind != D1_ModeCmd) { //if next mode isn't the current actual mode then blink
            mode_indicator_blink = 2; //2 Hz blink to display here
          }
          else {
            mode_indicator_blink = 0; //0 Hz blink to display here
          }
          break;
        case 1: //ON-->DAILY TIMER
          D1_ModeCmd_Ind = 2;
          ontime_indicator = D1_OnTime_Ind;
          offtime_indicator = D1_OffTime_Ind;
          if (D1_ModeCmd_Ind != D1_ModeCmd) { //if next mode isn't the current actual mode then blink
            mode_indicator_blink = 2; //2 Hz blink to display here
          }
          else {
            mode_indicator_blink = 0; //0 Hz blink to display here
          }
          break;
        case 2: //DAILY TIMER-->CYCLE TIMER
          D1_ModeCmd_Ind = 3;
          ontime_indicator = D1_OnTime_Ind;
          offtime_indicator = D1_OffTime_Ind;
          if (D1_ModeCmd_Ind != D1_ModeCmd) { //if next mode isn't the current actual mode then blink
            mode_indicator_blink = 2; //2 Hz blink to display here
          }
          else {
            mode_indicator_blink = 0; //0 Hz blink to display here
          }
          break;
        case 3: //CYCLE TIMER-->OFF
          D1_ModeCmd_Ind = 0;
          ontime_indicator = "----";
          offtime_indicator = "----";
          if (D1_ModeCmd_Ind != D1_ModeCmd) { //if next mode isn't the current actual mode then blink
            mode_indicator_blink = 2; //2 Hz blink to display here
          }
          else {
            mode_indicator_blink = 0; //0 Hz blink to display here
          }
          break;
    
      mode_indicator = D1_ModeCmd_Ind; //write to display here
      break;

      case 1: //DEVICE 2
      //COPY FINAL Device 1 CODE HERE
      mode_indicator = D2_ModeCmd_Ind; //write to display here
      break;

      case 2: //DEVICE 3
      //COPY FINAL Device 1 CODE HERE
      mode_indicator = D3_ModeCmd_Ind; //write to display here
      break;

      case 3: //DEVICE 4
      //COPY FINAL Device 1 CODE HERE      
      mode_indicator = D4_ModeCmd_Ind; //write to display here
      break;
    }
  }
}   
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Call all of these functions with hardware interrupts so the flag sets instantly
void ifDeviceButtonPressed() { 
  DeviceButtonPressed = true;
  ButtonPressed = true;
}
void ifModeButtonPressed() { 
  if (timed_out == false) {
    ModeButtonPressed = true;
    ButtonPressed = true;
  }
}
void ifSetButtonPressed() { 
  if (timed_out == false) {
    SetButtonPressed = true;
    ButtonPressed = true;
  } 
}  
void ifIncButtonPressed() {   
  if (timed_out == false) {
    IncButtonPressed = true;
    ButtonPressed = true;
  }   
}
void ifDecButtonPressed() { 
  if (timed_out == false) {
    DecButtonPressed = true;
    ButtonPressed = true;
  }
}
void ifConfirmButtonPressed() {
  if (timed_out == false) {
    ConfirmButtonPressed = true;
    ButtonPressed = true;
  }
}

void PanelTimeout() {
  if ( ((millis() - last_buttonpress_time) > 300000) && timed_out == false ) { //300000 = 5min. Ignoring the millis() rollover here, DNC if a timeout takes twice as long
    
    //Reset all values back to actuals, just in case. This code is a one-shot so this is DNC.
    D1_ModeCmd_Ind = D1_ModeCmd;
    D2_ModeCmd_Ind = D2_ModeCmd;
    D3_ModeCmd_Ind = D3_ModeCmd;
    D4_ModeCmd_Ind = D4_ModeCmd;

    D1_Daily_Timer_ON_Time_Ind = D1_Daily_Timer_ON_Time;
    D2_Daily_Timer_ON_Time_Ind = D2_Daily_Timer_ON_Time;
    D3_Daily_Timer_ON_Time_Ind = D3_Daily_Timer_ON_Time;
    D4_Daily_Timer_ON_Time_Ind = D4_Daily_Timer_ON_Time;

    D1_Daily_Timer_OFF_Time_Ind = D1_Daily_Timer_OFF_Time;
    D2_Daily_Timer_OFF_Time_Ind = D2_Daily_Timer_OFF_Time;
    D3_Daily_Timer_OFF_Time_Ind = D3_Daily_Timer_OFF_Time;
    D4_Daily_Timer_OFF_Time_Ind = D4_Daily_Timer_OFF_Time;

    D1_Cycle_Timer_ON_Length_Ind = D1_Cycle_Timer_ON_Length;
    D2_Cycle_Timer_ON_Length_Ind = D2_Cycle_Timer_ON_Length;
    D3_Cycle_Timer_ON_Length_Ind = D3_Cycle_Timer_ON_Length;
    D4_Cycle_Timer_ON_Length_Ind = D4_Cycle_Timer_ON_Length;

    D1_Cycle_Timer_OFF_Length_Ind = D1_Cycle_Timer_OFF_Length;
    D2_Cycle_Timer_OFF_Length_Ind = D2_Cycle_Timer_OFF_Length;
    D3_Cycle_Timer_OFF_Length_Ind = D3_Cycle_Timer_OFF_Length;
    D4_Cycle_Timer_OFF_Length_Ind = D4_Cycle_Timer_OFF_Length;

    //turn off all indicators
    device_indicator = "    ";
    mode_indicator = "    ";
    ontime_indicator = "    ";
    offtime_indicator = "    ";

    //turn off all blinks
    mode_indicator_blink = 0;
    ontime_indicator_blink = 0;
    offtime_indicator_blink = 0;
    
    //reset edit flags used to latch editing 
    D1_Mode_edit = false;
    D2_Mode_edit = false;
    D3_Mode_edit = false;
    D4_Mode_edit = false;

    D1_Daily_edit = false;
    D2_Daily_edit = false;
    D3_Daily_edit = false;
    D4_Daily_edit = false;

    D1_Cycle_edit = false;
    D2_Cycle_edit = false;
    D3_Cycle_edit = false;
    D4_Cycle_edit = false;

    timed_out = true;
  }
}

//This function controls the panel indicator and reads panel buttons
void PanelControl() {

  if ( DeviceButtonPressed == true ) {
    last_buttonpress_time = millis();
    if (timed_out == true) {
      display_device = 0;
      timed_out = false;
    }
    else {
      if ( display_device == 3 ) { 
        display_device = 0;
      }
      else {
        display_device++;
      }
    }
    
    //Reset values to actuals if device button pressed. This ensures anything that was changed but not confirmed resets when device is changed.
    D1_ModeCmd_Ind = D1_ModeCmd;
    D2_ModeCmd_Ind = D2_ModeCmd;
    D3_ModeCmd_Ind = D3_ModeCmd;
    D4_ModeCmd_Ind = D4_ModeCmd;

    D1_Daily_Timer_ON_Time_Ind = D1_Daily_Timer_ON_Time;
    D2_Daily_Timer_ON_Time_Ind = D2_Daily_Timer_ON_Time;
    D3_Daily_Timer_ON_Time_Ind = D3_Daily_Timer_ON_Time;
    D4_Daily_Timer_ON_Time_Ind = D4_Daily_Timer_ON_Time;

    D1_Daily_Timer_OFF_Time_Ind = D1_Daily_Timer_OFF_Time;
    D2_Daily_Timer_OFF_Time_Ind = D2_Daily_Timer_OFF_Time;
    D3_Daily_Timer_OFF_Time_Ind = D3_Daily_Timer_OFF_Time;
    D4_Daily_Timer_OFF_Time_Ind = D4_Daily_Timer_OFF_Time;

    D1_Cycle_Timer_ON_Length_Ind = D1_Cycle_Timer_ON_Length;
    D2_Cycle_Timer_ON_Length_Ind = D2_Cycle_Timer_ON_Length;
    D3_Cycle_Timer_ON_Length_Ind = D3_Cycle_Timer_ON_Length;
    D4_Cycle_Timer_ON_Length_Ind = D4_Cycle_Timer_ON_Length;

    D1_Cycle_Timer_OFF_Length_Ind = D1_Cycle_Timer_OFF_Length;
    D2_Cycle_Timer_OFF_Length_Ind = D2_Cycle_Timer_OFF_Length;
    D3_Cycle_Timer_OFF_Length_Ind = D3_Cycle_Timer_OFF_Length;
    D4_Cycle_Timer_OFF_Length_Ind = D4_Cycle_Timer_OFF_Length;
  }

  if ( ModeButtonPressed == true ) {
    //Reset values to actuals if MODE button pressed. This ensures anything that was changed but not confirmed resets when MODE is changed.
    D1_ModeCmd_Ind = D1_ModeCmd;
    D2_ModeCmd_Ind = D2_ModeCmd;
    D3_ModeCmd_Ind = D3_ModeCmd;
    D4_ModeCmd_Ind = D4_ModeCmd;

    D1_Daily_Timer_ON_Time_Ind = D1_Daily_Timer_ON_Time;
    D2_Daily_Timer_ON_Time_Ind = D2_Daily_Timer_ON_Time;
    D3_Daily_Timer_ON_Time_Ind = D3_Daily_Timer_ON_Time;
    D4_Daily_Timer_ON_Time_Ind = D4_Daily_Timer_ON_Time;

    D1_Daily_Timer_OFF_Time_Ind = D1_Daily_Timer_OFF_Time;
    D2_Daily_Timer_OFF_Time_Ind = D2_Daily_Timer_OFF_Time;
    D3_Daily_Timer_OFF_Time_Ind = D3_Daily_Timer_OFF_Time;
    D4_Daily_Timer_OFF_Time_Ind = D4_Daily_Timer_OFF_Time;

    D1_Cycle_Timer_ON_Length_Ind = D1_Cycle_Timer_ON_Length;
    D2_Cycle_Timer_ON_Length_Ind = D2_Cycle_Timer_ON_Length;
    D3_Cycle_Timer_ON_Length_Ind = D3_Cycle_Timer_ON_Length;
    D4_Cycle_Timer_ON_Length_Ind = D4_Cycle_Timer_ON_Length;

    D1_Cycle_Timer_OFF_Length_Ind = D1_Cycle_Timer_OFF_Length;
    D2_Cycle_Timer_OFF_Length_Ind = D2_Cycle_Timer_OFF_Length;
    D3_Cycle_Timer_OFF_Length_Ind = D3_Cycle_Timer_OFF_Length;
    D4_Cycle_Timer_OFF_Length_Ind = D4_Cycle_Timer_OFF_Length;
  }

  switch (display_device) {   //Set display device to 0 at main loop first call #format

    case 0:
      device_indicator = D1_Name;

      if ( ModeButtonPressed == true || D1_Mode_edit == true ) {  //while viewing device 1 latch in blinking mode indicator if MODE button pressed
        if ( ModeButtonPressed == true && D1_Mode_edit == true ) {  //this would be for subsequent MODE button presses while viewing this device
          if (D1_ModeCmd_Ind == 3) { //this if-else increments the mode cmd in a loop that rolls over 0-3
            D1_ModeCmd_Ind = 0;
          }
          else {
            D1_ModeCmd_Ind++;
          }
        }
        D1_Mode_edit = true;
        mode_indicator_blink = 2; //2 Hz
        mode_indicator = D1_ModeCmd_Ind; //#format
      }
      else {
        mode_indicator = D1_ModeCmd; //#format
      	mode_indicator_blink = 0;
        D1_ModeCmd_Ind = D1_ModeCmd;
      }

      switch ( D1_ModeCmd_Ind) {
        case 0: //OFF
          ontime_indicator = "----";
          offtime_indicator = "----";
          break;
        case 1: //ON
          ontime_indicator = "----";
          offtime_indicator = "----";
          break;
        case 2: //DAILY TIMER
          if ( SetButtonPressed == true || D1_Daily_edit == true ) {  //while viewing device 1 latch in blinking mode indicator if MODE button pressed
            if ( SetButtonPressed == true && D1_Daily_edit == true ) { //SET button has already been pressed once (don't increment on first press)
              if (timer_segment == 4) { //this if-else increments the mode cmd in a loop that rolls over 0-4
                timer_segment = 0;
              }
              else {
                timer_segment++;
              }
            }  
	          D1_Daily_edit = true;
	          ontime_indicator_blink = 2; //2 Hz
            offtime_indicator_blink = 2; //2 Hz

            switch ( timer_segment ) { //figure out where to initialize this to ON HOURS segment to 0
              
              case 0: //SET ON HOURS
                D1_ON_HR = D1_Daily_Timer_ON_Time_Ind / 3600; //parse HOUR
                D1_ON_MIN = (D1_Daily_Timer_ON_Time_Ind - D1_ON_HR*3600) / 60; //parse MIN

	              if ( IncButtonPressed == true) {
                  if ( D1_ON_HR < 23 ) {
                    D1_ON_HR++; //add 1 hour
                  }
                  if ( D1_ON_HR == 23 ) {
                    D1_ON_HR = D1_ON_HR - 23; //subract 23 hours to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_ON_HR > 0 ) {
                    D1_ON_HR--; //subtract 1 hour
                  }  
                  if ( D1_ON_HR == 0 ) {
                    D1_ON_HR = D1_ON_HR + 23; //add 23 hours to reset to 23
                  }                  
                }  

                ontime_indicator = D1_ON_HR + "  "; 
                offtime_indicator = "HR";
                D1_Daily_Timer_ON_Time_Ind = D1_ON_HR*3600 + D1_ON_MIN*60; //reassemble time just in case
              break;
		
              case 1: //SET ON MINS 
			          D1_ON_HR = D1_Daily_Timer_ON_Time_Ind / 3600; //parse HOUR
                D1_ON_MIN = (D1_Daily_Timer_ON_Time_Ind - D1_ON_HR*3600) / 60; //parse MIN 

	              if ( IncButtonPressed == true) {
                  if ( D1_ON_MIN < 59 ) {
                    D1_ON_MIN++; //add 1 min
                  }
                  if ( D1_ON_MIN == 59 ) {
                    D1_ON_MIN = D1_ON_MIN - 59; //subract 59 mins to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_ON_MIN > 0 ) {
                    D1_ON_MIN--; //subtract 1 hour
                  }  
                  if ( D1_ON_MIN == 0 ) {
                    D1_ON_MIN = D1_ON_MIN + 59; //add 59 mins to reset to 59
                  }                  
                }  

                ontime_indicator = "  " + D1_ON_MIN; 
                offtime_indicator = "MIN";
                D1_Daily_Timer_ON_Time_Ind = D1_ON_HR*3600 + D1_ON_MIN*60; //reassemble time just in case
              break;

              case 2: //OFF HRS
                D1_OFF_HR = D1_Daily_Timer_OFF_Time_Ind / 3600; //parse HOUR
                D1_OFF_MIN = (D1_Daily_Timer_OFF_Time_Ind - D1_OFF_HR*3600) / 60; //parse MIN

	              if ( IncButtonPressed == true) {
                  if ( D1_OFF_HR < 23 ) {
                    D1_OFF_HR++; //add 1 hour
                  }
                  if ( D1_OFF_HR == 23 ) {
                    D1_OFF_HR = D1_OFF_HR - 23; //subract 23 hours to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_OFF_HR > 0 ) {
                    D1_OFF_HR--; //subtract 1 hour
                  }  
                  if ( D1_OFF_HR == 0 ) {
                    D1_OFF_HR = D1_OFF_HR + 23; //add 23 hours to reset to 23
                  }                  
                }  

                offtime_indicator = D1_OFF_HR + "  "; 
                ontime_indicator = "HR";
                D1_Daily_Timer_OFF_Time_Ind = D1_OFF_HR*3600 + D1_OFF_MIN*60; //reassemble time just in case
              break;

              case 3: //OFF MINS
                D1_OFF_HR = D1_Daily_Timer_OFF_Time_Ind / 3600; //parse HOUR
                D1_OFF_MIN = (D1_Daily_Timer_OFF_Time_Ind - D1_OFF_HR*3600) / 60; //parse MIN 

	              if ( IncButtonPressed == true) {
                  if ( D1_OFF_MIN < 59 ) {
                    D1_OFF_MIN++; //add 1 min
                  }
                  if ( D1_OFF_MIN == 59 ) {
                    D1_OFF_MIN = D1_OFF_MIN - 59; //subract 59 mins to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_OFF_MIN > 0 ) {
                    D1_OFF_MIN--; //subtract 1 hour
                  }  
                  if ( D1_OFF_MIN == 0 ) {
                    D1_OFF_MIN = D1_OFF_MIN + 59; //add 59 mins to reset to 59
                  }                  
                }  

                offtime_indicator = "  " + D1_OFF_MIN; 
                ontime_indicator = "MIN";
                D1_Daily_Timer_OFF_Time_Ind = D1_OFF_HR*3600 + D1_OFF_MIN*60; //reassemble time just in case
              break;

              case 4: //SHOW ALL BLINKING
                D1_ON_HR = D1_Daily_Timer_ON_Time_Ind / 3600; //parse HOUR
                D1_ON_MIN = (D1_Daily_Timer_ON_Time_Ind - D1_ON_HR*3600) / 60; //parse MIN 
                D1_OFF_HR = D1_Daily_Timer_OFF_Time_Ind / 3600; //parse HOUR
                D1_OFF_MIN = (D1_Daily_Timer_OFF_Time_Ind - D1_OFF_HR*3600) / 60; //parse MIN 

                ontime_indicator = D1_ON_HR + ":" + D1_OFF_HR;
                offtime_indicator = D1_OFF_HR + ":" + D1_OFF_MIN;
              break;  
            }  
          }
          else {
            ontime_indicator = D1_Daily_Timer_ON_Time;
            offtime_indicator = D1_Daily_Timer_OFF_Time;     
            ontime_indicator_blink = 0;
	          offtime_indicator_blink = 0;
            D1_Daily_Timer_ON_Time_Ind = D1_Daily_Timer_ON_Time;
            D1_Daily_Timer_OFF_Time_Ind = D1_Daily_Timer_OFF_Time;
	          
            D1_Daily_edit = false;
  	        timer_segment = 0; //initialize this at bootup to 0
          }
        break;

        case 3: //CYCLE
          if ( SetButtonPressed == true || D1_Cycle_edit == true ) {  //while viewing device 1 latch in blinking mode indicator if MODE button pressed
            if ( SetButtonPressed == true && D1_Daily_edit == true ) { //SET button has already been pressed once (don't increment on first press)
              if (timer_segment == 4) { //this if-else increments the mode cmd in a loop that rolls over 0-4
                timer_segment = 0;
              }
              else {
                timer_segment++;
              }
            }  
	          D1_Cycle_edit = true;
	          ontime_indicator_blink = 2; //2 Hz
            offtime_indicator_blink = 2; //2 Hz

            switch ( timer_segment ) { //figure out where to initialize this to ON HOURS segment to 0
              
              case 0: //SET ON HOURS
                D1_ON_HR = D1_Cycle_Timer_ON_Length_Ind / 3600000; //parse HOUR
                D1_ON_MIN = (D1_Cycle_Timer_ON_Length_Ind - D1_ON_HR*3600000) / 60000; //parse MIN

	              if ( IncButtonPressed == true) {
                  if ( D1_ON_HR < 23 ) {
                    D1_ON_HR++; //add 1 hour
                  }
                  if ( D1_ON_HR == 23 ) {
                    D1_ON_HR = D1_ON_HR - 23; //subract 23 hours to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_ON_HR > 0 ) {
                    D1_ON_HR--; //subtract 1 hour
                  }  
                  if ( D1_ON_HR == 0 ) {
                    D1_ON_HR = D1_ON_HR + 23; //add 23 hours to reset to 23
                  }                  
                }  

                ontime_indicator = D1_ON_HR + "  "; 
                offtime_indicator = "HR";
                D1_Cycle_Timer_ON_Length_Ind = D1_ON_HR*3600000 + D1_ON_MIN*60000; //reassemble time just in case
              break;
		
              case 1: //SET ON MINS 
			          D1_ON_HR = D1_Cycle_Timer_ON_Length_Ind / 3600000; //parse HOUR
                D1_ON_MIN = (D1_Cycle_Timer_ON_Length_Ind - D1_ON_HR*3600000) / 60000; //parse MIN 

	              if ( IncButtonPressed == true) {
                  if ( D1_ON_MIN < 59 ) {
                    D1_ON_MIN++; //add 1 min
                  }
                  if ( D1_ON_MIN == 59 ) {
                    D1_ON_MIN = D1_ON_MIN - 59; //subract 59 mins to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_ON_MIN > 0 ) {
                    D1_ON_MIN--; //subtract 1 hour
                  }  
                  if ( D1_ON_MIN == 0 ) {
                    D1_ON_MIN = D1_ON_MIN + 59; //add 59 mins to reset to 59
                  }                  
                }  

                ontime_indicator = "  " + D1_ON_MIN; 
                offtime_indicator = "MIN";
                D1_Cycle_Timer_ON_Length_Ind = D1_ON_HR*3600000 + D1_ON_MIN*60000; //reassemble time just in case
              break;

              case 2: //OFF HRS
                D1_OFF_HR = D1_Cycle_Timer_OFF_Length_Ind / 3600000; //parse HOUR
                D1_OFF_MIN = (D1_Cycle_Timer_OFF_Length_Ind - D1_OFF_HR*3600000) / 60000; //parse MIN

	              if ( IncButtonPressed == true) {
                  if ( D1_OFF_HR < 23 ) {
                    D1_OFF_HR++; //add 1 hour
                  }
                  if ( D1_OFF_HR == 23 ) {
                    D1_OFF_HR = D1_OFF_HR - 23; //subract 23 hours to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_OFF_HR > 0 ) {
                    D1_OFF_HR--; //subtract 1 hour
                  }  
                  if ( D1_OFF_HR == 0 ) {
                    D1_OFF_HR = D1_OFF_HR + 23; //add 23 hours to reset to 23
                  }                  
                }  

                offtime_indicator = D1_OFF_HR + "  "; 
                ontime_indicator = "HR";
                D1_Cycle_Timer_OFF_Length_Ind = D1_OFF_HR*3600000 + D1_OFF_MIN*60000; //reassemble time just in case
              break;

              case 3: //OFF MINS
                D1_OFF_HR = D1_Cycle_Timer_OFF_Length_Ind / 3600000; //parse HOUR
                D1_OFF_MIN = (D1_Cycle_Timer_OFF_Length_Ind - D1_OFF_HR*3600000) / 60000; //parse MIN 

	              if ( IncButtonPressed == true) {
                  if ( D1_OFF_MIN < 59 ) {
                    D1_OFF_MIN++; //add 1 min
                  }
                  if ( D1_OFF_MIN == 59 ) {
                    D1_OFF_MIN = D1_OFF_MIN - 59; //subract 59 mins to reset to 00
                  }
                }
                if ( DecButtonPressed == true ) {
                  if ( D1_OFF_MIN > 0 ) {
                    D1_OFF_MIN--; //subtract 1 hour
                  }  
                  if ( D1_OFF_MIN == 0 ) {
                    D1_OFF_MIN = D1_OFF_MIN + 59; //add 59 mins to reset to 59
                  }                  
                }  

                offtime_indicator = "  " + D1_OFF_MIN; 
                ontime_indicator = "MIN";
                D1_Cycle_Timer_OFF_Length_Ind = D1_OFF_HR*3600000 + D1_OFF_MIN*60000; //reassemble time just in case
              break;

              case 4: //SHOW ALL BLINKING
                D1_ON_HR = D1_Cycle_Timer_ON_Length_Ind / 3600000; //parse HOUR
                D1_ON_MIN = (D1_Cycle_Timer_ON_Length_Ind - D1_ON_HR*3600000) / 60000; //parse MIN 
                D1_OFF_HR = D1_Cycle_Timer_OFF_Length_Ind / 3600000; //parse HOUR
                D1_OFF_MIN = (D1_Cycle_Timer_OFF_Length_Ind - D1_OFF_HR*3600000) / 60000; //parse MIN 

                ontime_indicator = D1_ON_HR + ":" + D1_OFF_HR;
                offtime_indicator = D1_OFF_HR + ":" + D1_OFF_MIN;
              break;  
            }  
          }
          else {
            ontime_indicator = D1_Cycle_Timer_ON_Length;
            offtime_indicator = D1_Cycle_Timer_OFF_Length;     
            ontime_indicator_blink = 0;
	          offtime_indicator_blink = 0;
            D1_Cycle_Timer_ON_Length_Ind = D1_Cycle_Timer_ON_Length;
            D1_Cycle_Timer_OFF_Length_Ind = D1_Cycle_Timer_OFF_Length;
	          
            D1_Cycle_edit = false;
  	        timer_segment = 0; //initialize this at bootup to 0
          }
        break;     
      } 
    break;

    case 1:
      //Copy Device 1 Logic
    break;

    case 2:
      //Copy Device 1 Logic
    break;

    case 3:
      //Copy Device 1 Logic
    break;
  }

  if ( ConfirmButtonPressed == true ) {
    
    //Device 1
    if ( D1_Mode_edit = true ) {
      D1_ModeCmdRqst = D1_ModeCmd_Ind; //write this here or in main loop?
      Blynk.virtualWrite(V0, D1_ModeCmd_Ind);
    }
    
    if ( D1_Daily_edit == true ) {
      D1_Daily_Timer_ON_Time = D1_Daily_Timer_ON_Time_Ind;
      Blynk.virtualWrite(V2, D1_Daily_Timer_ON_Time_Ind); //#format to get up to Blynk?
      D1_Daily_Timer_OFF_Time = D1_Daily_Timer_OFF_Time_Ind;
      Blynk.virtualWrite(V3, D1_Daily_Timer_OFF_Time_Ind);
    }
    
    if ( D1_Cycle_edit == true ) {
      D1_Cycle_Timer_ON_Length = D1_Cycle_Timer_ON_Length_Ind;
      Blynk.virtualWrite(V4, D1_Cycle_Timer_ON_Length_Ind);
      D1_Cycle_Timer_ON_Length = D1_Cycle_Timer_ON_Length_Ind;
      Blynk.virtualWrite(V5, D1_Cycle_Timer_OFF_Length_Ind);
    }
    
    //reset all editing flags
    D1_Mode_edit = false;
    D1_Daily_edit = false;
    D1_Cycle_edit = false;

    //Need to add device 2-4 here. Might as well write them all every time just in case
    /
    /Un-blink all indicators
    mode_indicator_blink = 0;
    ontime_indicator_blink = 0;
    offtime_indicator_blink = 0;

    timer_segment = 0;
  }
  
  //Reset button flags at the end of indicator control loop, do them all just in case
  DeviceButtonPressed = false;
  ModeButtonPressed = false;
  SetButtonPressed = false;
  IncButtonPressed = false;
  DecButtonPressed = false;
  ConfirmButtonPressed = false;
  ButtonPressed = false;

}

//This function sends all of the relay coil states up to the Blynk server
void SendRelayCoils() {    

  //Device1 
  D1_Coil_State = digitalRead(relay1_in);
  if ( (D1_Coil_State_prev != D1_Coil_State) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.virtualWrite( V6, D1_Coil_State );
  }
  D1_Coil_State_prev = D1_Coil_State;

  //Device2
  D2_Coil_State = digitalRead(relay2_in);
  if ( (D2_Coil_State_prev != D2_Coil_State) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.virtualWrite( V16, D2_Coil_State );
  }
  D2_Coil_State_prev = D2_Coil_State;

  //Device3 
  D3_Coil_State = digitalRead(relay3_in);
  if ( (D3_Coil_State_prev != D3_Coil_State) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.virtualWrite( V26, D3_Coil_State );
  }
  D3_Coil_State_prev = D3_Coil_State;

  //Device4 
  D4_Coil_State = digitalRead(relay4_in);
  if ( (D4_Coil_State_prev != D4_Coil_State) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.virtualWrite( V36, D4_Coil_State );
  }
  D4_Coil_State_prev = D4_Coil_State;
}

//This function checks if the relay coil state does not match the command for each device. This is used to set fault events.
void DetectAndSendRelayFaults() {
 
  //Relay 1
  if (D1_Relay_Fault == false) {
    if(digitalRead(relay1_out) != digitalRead(relay1_in)) { 
      if ((millis() - Relay1_last_nofault_time) > 10000) {
        
        //Do all of this when in faulted state:
        D1_Relay_Fault = true;
        Serial.println("BLYNK EVENT");
        Blynk.logEvent("Relay1_FaultSet");

        Blynk.setProperty(V61, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V61, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V61, "offColor", WHITE);  
        Blynk.setProperty(V61, "onColor", WHITE);  
        Blynk.setProperty(V61, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V61, "onBackColor", WARNING_RED);  

        Blynk.setProperty( V7, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty( V7, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty( V7, "offColor", WHITE); 
        Blynk.setProperty( V7, "onColor", WHITE);  
        Blynk.setProperty( V7, "offBackColor", WARNING_RED);  
        Blynk.setProperty( V7, "onBackColor", WARNING_RED);  
      }
    }
    else {
      Relay1_last_nofault_time = millis();
    }
  }
  else {
    Serial.print("RElay fault"); 
    if (digitalRead(relay1_out) == digitalRead(relay1_in)) { 
      
      //Do all of this when in NOT faulted state:
      Relay1_last_nofault_time = millis();
      D1_Relay_Fault = false;
      Blynk.logEvent("Relay1_FaultClear");

      Blynk.setProperty(V61, "offLabel", D1_Name);
      Blynk.setProperty(V61, "onLabel", "D1 ON Label"); //Can substitute data here if we want to see it on app dashboard button press
      Blynk.setProperty(V61, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V61, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V61, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V61, "onBackColor", GARDENTEK_BLUE);  

      Blynk.setProperty( V7, "offLabel", "MODE");
      Blynk.setProperty( V7, "onLabel", "MODE");
      Blynk.setProperty( V7, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty( V7, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty( V7, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty( V7, "onBackColor", GARDENTEK_BLUE);
    }
  }

  //Relay 2
  if (D2_Relay_Fault == false) {
    if(digitalRead(relay2_out) != digitalRead(relay2_in)) { 
      if ((millis() - Relay2_last_nofault_time) > 10000) {
        
        //Do all of this when in faulted state:
        D2_Relay_Fault = true;
        Serial.println("BLYNK EVENT");
        Blynk.logEvent("Relay2_FaultSet");

        Blynk.setProperty(V62, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V62, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V62, "offColor", WHITE);  
        Blynk.setProperty(V62, "onColor", WHITE);  
        Blynk.setProperty(V62, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V62, "onBackColor", WARNING_RED);  

        Blynk.setProperty(V17, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V17, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V17, "offColor", WHITE); 
        Blynk.setProperty(V17, "onColor", WHITE);  
        Blynk.setProperty(V17, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V17, "onBackColor", WARNING_RED);  
      }
    }
    else {
      Relay2_last_nofault_time = millis();
    }
  }
  else { 
    if (digitalRead(relay2_out) == digitalRead(relay2_in)) { 
      
      //Do all of this when in NOT faulted state:
      Relay2_last_nofault_time = millis();
      D2_Relay_Fault = false;
      Blynk.logEvent("Relay2_FaultClear");

      Blynk.setProperty(V62, "offLabel", D2_Name);
      Blynk.setProperty(V62, "onLabel", "D2 ON Label"); //Can substitute data here if we want to see it on app dashboard button press
      Blynk.setProperty(V62, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V62, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V62, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V62, "onBackColor", GARDENTEK_BLUE);  

      Blynk.setProperty(V17, "offLabel", "MODE");
      Blynk.setProperty(V17, "onLabel", "MODE");
      Blynk.setProperty(V17, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V17, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V17, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V17, "onBackColor", GARDENTEK_BLUE);
    }
  }
  
  //Relay3
  if (D3_Relay_Fault == false) {
    if(digitalRead(relay3_out) != digitalRead(relay3_in)) { 
      if ((millis() - Relay3_last_nofault_time) > 10000) {
        
        //Do all of this when in faulted state:
        D3_Relay_Fault = true;
        Serial.println("BLYNK EVENT");
        Blynk.logEvent("Relay3_FaultSet");

        Blynk.setProperty(V63, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V63, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V63, "offColor", WHITE);  
        Blynk.setProperty(V63, "onColor", WHITE);  
        Blynk.setProperty(V63, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V63, "onBackColor", WARNING_RED);  

        Blynk.setProperty(V27, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V27, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V27, "offColor", WHITE); 
        Blynk.setProperty(V27, "onColor", WHITE);  
        Blynk.setProperty(V27, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V27, "onBackColor", WARNING_RED);  
      }
    }
    else {
      Relay3_last_nofault_time = millis();
    }
  }
  else { 
    if (digitalRead(relay3_out) == digitalRead(relay3_in)) { 
      
      //Do all of this when in NOT faulted state:
      Relay3_last_nofault_time = millis();
      D3_Relay_Fault = false;
      Blynk.logEvent("Relay3_FaultClear");

      Blynk.setProperty(V63, "offLabel", D3_Name);
      Blynk.setProperty(V63, "onLabel", "D3 ON Label"); //Can substitute data here if we want to see it on app dashboard button press
      Blynk.setProperty(V63, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V63, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V63, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V63, "onBackColor", GARDENTEK_BLUE);  

      Blynk.setProperty(V27, "offLabel", "MODE");
      Blynk.setProperty(V27, "onLabel", "MODE");
      Blynk.setProperty(V27, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V27, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V27, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V27, "onBackColor", GARDENTEK_BLUE);
    }
  }

  //Relay4
  if (D4_Relay_Fault == false) {
    if(digitalRead(relay4_out) != digitalRead(relay4_in)) { 
      if ((millis() - Relay4_last_nofault_time) > 10000) {
        
        //Do all of this when in faulted state:
        D4_Relay_Fault = true;
        Serial.println("BLYNK EVENT");
        Blynk.logEvent("Relay4_FaultSet");

        Blynk.setProperty(V64, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V64, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V64, "offColor", WHITE);  
        Blynk.setProperty(V64, "onColor", WHITE);  
        Blynk.setProperty(V64, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V64, "onBackColor", WARNING_RED);  

        Blynk.setProperty(V37, "offLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V37, "onLabel", "HARDWARE FAULT!");
        Blynk.setProperty(V37, "offColor", WHITE); 
        Blynk.setProperty(V37, "onColor", WHITE);  
        Blynk.setProperty(V37, "offBackColor", WARNING_RED);  
        Blynk.setProperty(V37, "onBackColor", WARNING_RED);  
      }
    }
    else {
      Relay4_last_nofault_time = millis();
    }
  }
  else { 
    if (digitalRead(relay4_out) == digitalRead(relay4_in)) { 
      
      //Do all of this when in NOT faulted state:
      Relay4_last_nofault_time = millis();
      D4_Relay_Fault = false;
      Blynk.logEvent("Relay4_FaultClear");

      Blynk.setProperty(V64, "offLabel", D4_Name);
      Blynk.setProperty(V64, "onLabel", "D4 ON Label"); //Can substitute data here if we want to see it on app dashboard button press
      Blynk.setProperty(V64, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V64, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V64, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V64, "onBackColor", GARDENTEK_BLUE);  

      Blynk.setProperty(V37, "offLabel", "MODE");
      Blynk.setProperty(V37, "onLabel", "MODE");
      Blynk.setProperty(V37, "offColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V37, "onColor", GARDENTEK_GREEN);  
      Blynk.setProperty(V37, "offBackColor", GARDENTEK_BLUE);  
      Blynk.setProperty(V37, "onBackColor", GARDENTEK_BLUE);
    }
  }
}

//THis function checks if the 'Hide Device X' buttons have been toggled.
void CheckHideDevices() {

  //Device1
  if ( (D1_HideDevice != D1_HideDevice_prev) || Blynk_Enter_Connected_Mode == true ) {
    if ( D1_HideDevice == true ) {
      Blynk.setProperty( V6, "isHidden", true);
      Blynk.setProperty(V61, "isHidden", true);
      Blynk.setProperty(V71, "isHidden", true);
    }
    else {
      Blynk.setProperty( V6, "isHidden", false);
      Blynk.setProperty(V61, "isHidden", false);
      Blynk.setProperty(V71, "isHidden", false);
    }
  }

  D1_HideDevice_prev = D1_HideDevice;

  //Device2
  if ( (D2_HideDevice != D2_HideDevice_prev) || Blynk_Enter_Connected_Mode == true ) {
    if ( D2_HideDevice == true ) {
      Blynk.setProperty(V16, "isHidden", true);
      Blynk.setProperty(V62, "isHidden", true);
      Blynk.setProperty(V72, "isHidden", true);
    }
    else {
      Blynk.setProperty(V16, "isHidden", false);
      Blynk.setProperty(V62, "isHidden", false);
      Blynk.setProperty(V72, "isHidden", false);
    }
  }

  D2_HideDevice_prev = D2_HideDevice;

  //Device3
  if ( (D3_HideDevice != D3_HideDevice_prev) || Blynk_Enter_Connected_Mode == true ) {
    if ( D3_HideDevice == true ) {
      Blynk.setProperty(V26, "isHidden", true);
      Blynk.setProperty(V63, "isHidden", true);
      Blynk.setProperty(V73, "isHidden", true);
    }
    else {
      Blynk.setProperty(V26, "isHidden", false);
      Blynk.setProperty(V63, "isHidden", false);
      Blynk.setProperty(V73, "isHidden", false);
    }
  }

  D3_HideDevice_prev = D3_HideDevice;

  //Device4
  if ( (D4_HideDevice != D4_HideDevice_prev) || Blynk_Enter_Connected_Mode == true ) {
    if ( D4_HideDevice == true ) {
      Blynk.setProperty(V36, "isHidden", true);
      Blynk.setProperty(V64, "isHidden", true);
      Blynk.setProperty(V74, "isHidden", true);
    }
    else {
      Blynk.setProperty(V36, "isHidden", false);
      Blynk.setProperty(V64, "isHidden", false);
      Blynk.setProperty(V74, "isHidden", false);
    }
  }

  D4_HideDevice_prev = D4_HideDevice;

}

//This function checks if the device names have changed (from server) and sends back to server
void CheckDeviceNameChanges() {

  //Device1
  if ( (D1_Name != D1_Name_prev) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.setProperty(V61, "offLabel", D1_Name);
  }
  D1_Name_prev = D1_Name;

  //Device2
  if ( (D2_Name != D2_Name_prev) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.setProperty(V62, "offLabel", D2_Name);
  }
  D2_Name_prev = D2_Name;

  //Device3
  if ( (D3_Name != D3_Name_prev) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.setProperty(V63, "offLabel", D3_Name);
  }
  D3_Name_prev = D3_Name;

  //Device4
  if ( (D4_Name != D4_Name_prev) || Blynk_Enter_Connected_Mode == true ) {
    Blynk.setProperty(V64, "offLabel", D4_Name);
  }
  D4_Name_prev = D4_Name;      
}

//This function checks wifi signal strength.
void wifistrength() {
  signalRSSI = WiFi.RSSI();
  signalquality = map(signalRSSI,-100, -50, 0, 100);
  Blynk.virtualWrite(V80, signalquality);
  if (signalquality >= 85) {
    Blynk.setProperty(V80, "color", BLYNK_GREEN);
  }
  if (signalquality <= 85 && signalquality >= 50) {
    Blynk.setProperty(V80, "color", BLYNK_YELLOW);
  }
  if (signalquality <= 50) {
    Blynk.setProperty(V80, "color", BLYNK_RED);
    Blynk.logEvent("wifi_signal_low");
  }
}

//This function syncs the on-board RTC with the Blynk server time if the device is connected to the Blynk server.  
bool SetRTC() {
  if (Blynk.connected() == true) {
    rtc_time.Hour   = current_unix_timestamp.hour;
    rtc_time.Minute = current_unix_timestamp.minute;
    rtc_time.Second = current_unix_timestamp.second;
    rtc_time.Day    = current_unix_timestamp.day;
    rtc_time.Month  = current_unix_timestamp.month;
    rtc_time.Year   = CalendarYrToTm(current_unix_timestamp.year);
              
    if (RTC.write(rtc_time)) {                                                  
      Serial.println("RTC write successful");
      return true;
    }
    else {
      if (RTC.chipPresent()) {
        Serial.println("Cannot write time to DS1307!");
      }
      else {
        Serial.println("Cannot detect DS1307 present! Check circuitry.");
      }
      return false;
    }
  }
  else {
    Serial.println("Cannot sync RTC because Blynk server disconnected!");
    return false;
  } 
}

//This function selects the server time if available, or RTC time if the server is offline, to use for control logic. Also syncs RTC with server if RTC time is off.
void GetRTC() {  
  if (RTC.read(rtc_time)) {                                   //Read time from on-board RTC
    rtc_seconds_after_midnight = (rtc_time.Second + rtc_time.Minute * 60 + rtc_time.Hour * 60 * 60);
  }
  else {
    if (RTC.chipPresent()) {
      Serial.println("Cannot get time from DS1307!");
    }
    else {
      Serial.println("Cannot detect RTC chip present! Check circuitry.");
    }
  }
}

//This function resets the button alarm displays for relay faults, hides timer alarms, and resets relay fault setting to FALSE to avoid any server hangups. Alarms will set again if conditions met again.
void ReArmAlarms() {
  D1_Relay_Fault = false;
  D2_Relay_Fault = false;
  D3_Relay_Fault = false;
  D4_Relay_Fault = false;

  Relay1_last_nofault_time = millis();
  Relay2_last_nofault_time = millis();
  Relay3_last_nofault_time = millis();
  Relay4_last_nofault_time = millis();

  Blynk.setProperty(V61, "offLabel", D1_Name);
  Blynk.setProperty(V61, "onLabel", "D1 ON Label");
  Blynk.setProperty(V61, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V61, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V61, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty(V61, "onBackColor", GARDENTEK_BLUE);
      
  Blynk.setProperty(V62, "offLabel", D1_Name);
  Blynk.setProperty(V62, "onLabel", "D1 ON Label");
  Blynk.setProperty(V62, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V62, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V62, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty(V62, "onBackColor", GARDENTEK_BLUE);

  Blynk.setProperty(V63, "offLabel", D1_Name);
  Blynk.setProperty(V63, "onLabel", "D1 ON Label");
  Blynk.setProperty(V63, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V63, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V63, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty(V63, "onBackColor", GARDENTEK_BLUE);

  Blynk.setProperty(V64, "offLabel", D1_Name);
  Blynk.setProperty(V64, "onLabel", "D1 ON Label");
  Blynk.setProperty(V64, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V64, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V64, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty(V64, "onBackColor", GARDENTEK_BLUE);
        
  Blynk.setProperty( V7, "offLabel", "MODE");
  Blynk.setProperty( V7, "onLabel", "MODE");
  Blynk.setProperty( V7, "offColor", GARDENTEK_GREEN); 
  Blynk.setProperty( V7, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty( V7, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty( V7, "onBackColor", GARDENTEK_BLUE);  

  Blynk.setProperty(V17, "offLabel", "MODE");
  Blynk.setProperty(V17, "onLabel", "MODE");
  Blynk.setProperty(V17, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V17, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V17, "offBackColor", GARDENTEK_BLUE);  
  Blynk.setProperty(V17, "onBackColor", GARDENTEK_BLUE);  

  Blynk.setProperty(V27, "offLabel", "MODE");
  Blynk.setProperty(V27, "onLabel", "MODE");
  Blynk.setProperty(V27, "offColor", GARDENTEK_GREEN); 
  Blynk.setProperty(V27, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V27, "offBackColor", GARDENTEK_BLUE); 
  Blynk.setProperty(V27, "onBackColor", GARDENTEK_BLUE);  

  Blynk.setProperty(V37, "offLabel", "MODE");
  Blynk.setProperty(V37, "onLabel", "MODE");
  Blynk.setProperty(V37, "offColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V37, "onColor", GARDENTEK_GREEN);  
  Blynk.setProperty(V37, "offBackColor", GARDENTEK_BLUE); 
  Blynk.setProperty(V37, "onBackColor", GARDENTEK_BLUE); 
        
  Blynk.setProperty( V8, "isHidden", true);
  Blynk.setProperty(V18, "isHidden", true);
  Blynk.setProperty(V28, "isHidden", true);
  Blynk.setProperty(V38, "isHidden", true);
      
  Blynk.setProperty( V9, "isHidden", true);
  Blynk.setProperty(V19, "isHidden", true);
  Blynk.setProperty(V29, "isHidden", true);
  Blynk.setProperty(V39, "isHidden", true);
}  

//This function checks for changes in the ModeCmd or Daily/Cycle Timer settings and if so writes the new value to the EEPROM
void EEPROM_Update() {

  //Device1
  if ( D1_Name != D1_Name_prev ) {
    //EEPROM write D1_Name
  }
  if ( D1_ModeCmd != D1_ModeCmd_prev ) {
    //EEPROM write D1_ModeCmd
  }
  if ( D1_Daily_Timer_ON_Time != D1_Daily_Timer_ON_Time_prev ) {
    //EEPROM write D1_Daily_Timer_ON_Time
  }
  if ( D1_Daily_Timer_OFF_Time != D1_Daily_Timer_OFF_Time_prev ) {
    //EEPROM write D1_Daily_Timer_OFF_Time
  }
  if ( D1_Cycle_Timer_ON_Length != D1_Cycle_Timer_ON_Length_prev ) {
    //EEPROM write D1_Cycle_Timer_ON_length
  }
  if ( D1_Cycle_Timer_OFF_Length != D1_Cycle_Timer_OFF_Length_prev ) {
    //EEPROM write D1_Cycle_Timer_OFF_length
  }

  //Device2
  if ( D2_Name != D2_Name_prev ) {
    //EEPROM write D2_Name
  }  
  if ( D2_ModeCmd != D2_ModeCmd_prev ) {
    //EEPROM write D2_ModeCmd
  }
  if ( D2_Daily_Timer_ON_Time != D2_Daily_Timer_ON_Time_prev ) {
    //EEPROM write D2_Daily_Timer_ON_Time
  }
  if ( D2_Daily_Timer_OFF_Time != D2_Daily_Timer_OFF_Time_prev ) {
    //EEPROM write D2_Daily_Timer_OFF_Time
  }
  if ( D2_Cycle_Timer_ON_Length != D2_Cycle_Timer_ON_Length_prev ) {
    //EEPROM write D2_Cycle_Timer_ON_length
  }
  if ( D2_Cycle_Timer_OFF_Length != D2_Cycle_Timer_OFF_Length_prev ) {
    //EEPROM write D2_Cycle_Timer_OFF_length
  }

  //Device3
  if ( D3_Name != D3_Name_prev ) {
    //EEPROM write D3_Name
  }  
  if ( D3_ModeCmd != D3_ModeCmd_prev ) {
    //EEPROM write D3_ModeCmd
  }
  if ( D3_Daily_Timer_ON_Time != D3_Daily_Timer_ON_Time_prev ) {
    //EEPROM write D3_Daily_Timer_ON_Time
  }
  if ( D3_Daily_Timer_OFF_Time != D3_Daily_Timer_OFF_Time_prev ) {
    //EEPROM write D3_Daily_Timer_OFF_Time
  }
  if ( D3_Cycle_Timer_ON_Length != D3_Cycle_Timer_ON_Length_prev ) {
    //EEPROM write D3_Cycle_Timer_ON_length
  }
  if ( D3_Cycle_Timer_OFF_Length != D3_Cycle_Timer_OFF_Length_prev ) {
    //EEPROM write D3_Cycle_Timer_OFF_length
  }
      
  //Device4
  if ( D4_Name != D4_Name_prev ) {
    //EEPROM write D4_Name
  }  
  if ( D4_ModeCmd != D4_ModeCmd_prev ) {
    //EEPROM write D4_ModeCmd
  }
  if ( D4_Daily_Timer_ON_Time != D4_Daily_Timer_ON_Time_prev ) {
    //EEPROM write D4_Daily_Timer_ON_Time
  }
  if ( D4_Daily_Timer_OFF_Time != D4_Daily_Timer_OFF_Time_prev ) {
    //EEPROM write D4_Daily_Timer_OFF_Time
  }
  if ( D4_Cycle_Timer_ON_Length != D4_Cycle_Timer_ON_Length_prev ) {
    //EEPROM write D4_Cycle_Timer_ON_length
  }
  if ( D4_Cycle_Timer_OFF_Length != D4_Cycle_Timer_OFF_Length_prev ) {
    //EEPROM write D4_Cycle_Timer_OFF_length
  }
}

//These are the main control loops for each device that sets the relay commands
void D1_control_loop() {    
  
  if ( D1_ModeCmdRqst != D1_ModeCmdRqst_prev ) {                  //Use a placeholder so Blynk doesn't write a control variable mid-loop
    D1_ModeCmd = D1_ModeCmdRqst;
      if ( D1_ModeCmd != 3 ) {                                  
        D1_ResetCycleTimer = true;                              
      }
  }
  D1_ModeCmdRqst_prev = D1_ModeCmdRqst;
  
  switch (D1_ModeCmd) {                                     
    
    case 0: //OFF
	    
      if ( digitalRead(relay1_out) != LOW || D1_ModeCmd != D1_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V0, "color", WARNING_RED);              
        Blynk.setProperty(V8, "isHidden", true);
        Blynk.setProperty(V9, "isHidden", true);
        Blynk.virtualWrite(V0, 0);
        //digitalWrite(relay1_out, LOW);
        D1_RelayCmd = 0;
      }
      break; 

    case 1: //ON
	    
      if ( digitalRead(relay1_out) != HIGH || D1_ModeCmd != D1_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V0, "color", GARDENTEK_GREEN);            
        Blynk.setProperty(V8, "isHidden", true);
        Blynk.setProperty(V9, "isHidden", true);
        Blynk.virtualWrite(V0, 1);
        //digitalWrite(relay1_out, HIGH);
        D1_RelayCmd = 1;
      }
      break;

    case 2: //DAILY TIMER
      if ( D1_ModeCmd != D1_ModeCmd_prev ) {                    //Do this once when entering DAILY mode
        Blynk.setProperty(V9, "isHidden", true);
        Blynk.virtualWrite(V0, 2);
      } 

      //Check for INVALID CONFIG and display warning and set MODE bar color	  
      if ( D1_ModeCmd != D1_ModeCmd_prev || D1_Daily_Timer_ON_Time != D1_Daily_Timer_ON_Time_prev || D1_Daily_Timer_OFF_Time != D1_Daily_Timer_OFF_Time_prev ) {
        if ( ( D1_Daily_Timer_ON_Time == 0 || D1_Daily_Timer_OFF_Time == 0 || D1_Daily_Timer_ON_Time == D1_Daily_Timer_OFF_Time ) ) {
          Blynk.setProperty(V0, "color", WARNING_RED);          
          Blynk.setProperty(V8, "isHidden", false);		
          //digitalWrite(relay1_out, LOW);
          D1_RelayCmd = 0;
          D1_Daily_Timer_Fault_prev = true;
        }
        else {
          D1_Daily_Timer_Fault_prev = false;
          Blynk.setProperty(V0, "color", GARDENTEK_BLUE);               
          Blynk.setProperty(V8, "isHidden", true);
        }
      }
      
      //Run the daily timer	if config is valid      
      if ( D1_Daily_Timer_Fault_prev == false ) { //Do this code if we don't have a timer config fault

        if ( D1_Daily_Timer_ON_Time < D1_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D1_Daily_Timer_ON_Time && current_seconds_after_midnight < D1_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay1_out, HIGH);
            D1_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay1_out, LOW); 
            D1_RelayCmd = 0;
          }
        }
        if ( D1_Daily_Timer_ON_Time > D1_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D1_Daily_Timer_ON_Time || current_seconds_after_midnight < D1_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay1_out, HIGH);
            D1_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay1_out, LOW);
            D1_RelayCmd = 0;
          }
        }
      }
      break;
    
    case 3: //CYCLE TIMER
      if ( D1_ModeCmd != D1_ModeCmd_prev ) {                    //Do this once when entering CYCLE mode
        Blynk.setProperty(V8, "isHidden", true);
        Blynk.virtualWrite(V0, 3);
      }

      //Check for INVALID CONFIG and display warning and set MODE bar color
      if ( D1_ModeCmd != D1_ModeCmd_prev || D1_Cycle_Timer_ON_Length != D1_Cycle_Timer_ON_Length_prev || D1_Cycle_Timer_OFF_Length != D1_Cycle_Timer_OFF_Length_prev ) {
        D1_ResetCycleTimer = true;
        if ( D1_Cycle_Timer_ON_Length == 0 || D1_Cycle_Timer_OFF_Length == 0 ) {
          Blynk.setProperty(V0, "color", WARNING_RED); 
          Blynk.setProperty(V9, "isHidden", false);               
          //digitalWrite(relay1_out, LOW);
          D1_RelayCmd = 0;
          D1_Cycle_Timer_Fault_prev = true;
        }
        else {
          Blynk.setProperty(V0, "color", GARDENTEK_GREEN);
          Blynk.setProperty(V9, "isHidden", true);  
          D1_Cycle_Timer_Fault_prev = false;        
        }
      }

      //Run the cycle timer if config is valid
      if ( D1_Cycle_Timer_Fault_prev == false ) {

	      //Reset cycle timer and calculate times (counts)      
      	if ( D1_ResetCycleTimer == true ) { //|| D1_Cycle_Timer_ON_Length != D1_Cycle_Timer_ON_Length_prev || D1_Cycle_Timer_OFF_Length != D1_Cycle_Timer_OFF_Length_prev ) {
          //Blynk.setProperty(V0, "color", GARDENTEK_GREEN);
          //Blynk.setProperty(V9, "isHidden", true); 
          D1_ResetCycleTimer = false;	        		              //clear this in case this is how we got here
          D1_Cycle_Timer_ON_Counts = D1_Cycle_Timer_ON_Length / loop_rate;
	        D1_Cycle_Timer_OFF_Counts = D1_Cycle_Timer_OFF_Length / loop_rate;
	        D1_Cycle_ON_current_count = 1;
          D1_Cycle_OFF_current_count = 1;
        }

	      //Turn ON for calculated counts and the OFF for calculated counts. Then reset.      
        if ( D1_Cycle_ON_current_count <= D1_Cycle_Timer_ON_Counts ) {
          D1_Cycle_ON_current_count++;
	        //digitalWrite(relay1_out, HIGH);
          D1_RelayCmd = 1;
	      }  
	      else {
  	      if ( D1_Cycle_OFF_current_count <= D1_Cycle_Timer_OFF_Counts - 1 ) {   //Subtract 1 count that will be used to set the reset flag true 
	          D1_Cycle_OFF_current_count++;	
            //digitalWrite(relay1_out, LOW);
	          D1_RelayCmd = 0;
    	    }
	        else {                                                                 //Here we use the subtracted count to write reset flag true
	          D1_ResetCycleTimer = true;
            //digitalWrite(relay1_out, LOW);
            D1_RelayCmd = 0;	
  	      }
	      }     
      }
      break;
  }

  if ( D1_RelayCmd != D1_RelayCmd_prev || main_loop_firstcall == true ) {
    if ( D1_RelayCmd == 0 ) {
      digitalWrite(relay1_out, LOW);
    }
    else {
      digitalWrite(relay1_out, HIGH);
    }
  }
  D1_RelayCmd_prev = D1_RelayCmd;
}

void D2_control_loop() {    
  
  if ( D2_ModeCmdRqst != D2_ModeCmdRqst_prev ) {                  //Use a placeholder so Blynk doesn't write a control variable mid-loop
    D2_ModeCmd = D2_ModeCmdRqst;
      if ( D2_ModeCmd != 3 ) {                                  
        D2_ResetCycleTimer = true;                              
      }
  }
  D2_ModeCmdRqst_prev = D2_ModeCmdRqst;
  
  switch (D2_ModeCmd) {                                     
    Serial.println(D2_ModeCmd);
    case 0: //OFF
	    
      if ( digitalRead(relay2_out) != LOW || D2_ModeCmd != D2_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V10, "color", WARNING_RED);              
        Blynk.setProperty(V18, "isHidden", true);
        Blynk.setProperty(V19, "isHidden", true);
        Blynk.virtualWrite(V10, 0);
        //digitalWrite(relay2_out, LOW);
        D2_RelayCmd = 0;
      }
      break; 

    case 1: //ON
	    
      if ( digitalRead(relay2_out) != HIGH || D2_ModeCmd != D2_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V10, "color", GARDENTEK_GREEN);            
        Blynk.setProperty(V18, "isHidden", true);
        Blynk.setProperty(V19, "isHidden", true);
        Blynk.virtualWrite(V10, 1);
        //digitalWrite(relay2_out, HIGH);
        D2_RelayCmd = 1;
      }
      break;

    case 2: //DAILY TIMER
      if ( D2_ModeCmd != D2_ModeCmd_prev ) {                    //Do this once when entering DAILY mode
        Blynk.setProperty(V19, "isHidden", true);
        Blynk.virtualWrite(V10, 2);
      } 

      //Check for INVALID CONFIG and display warning and set MODE bar color	  
      if ( D2_ModeCmd != D2_ModeCmd_prev || D2_Daily_Timer_ON_Time != D2_Daily_Timer_ON_Time_prev || D2_Daily_Timer_OFF_Time != D2_Daily_Timer_OFF_Time_prev ) {
        if ( ( D2_Daily_Timer_ON_Time == 0 || D2_Daily_Timer_OFF_Time == 0 || D2_Daily_Timer_ON_Time == D2_Daily_Timer_OFF_Time ) ) {
          Blynk.setProperty(V10, "color", WARNING_RED);          
          Blynk.setProperty(V18, "isHidden", false);		
          //digitalWrite(relay2_out, LOW);
          D2_RelayCmd = 0;
          D2_Daily_Timer_Fault_prev = true;
        }
        else {
          D2_Daily_Timer_Fault_prev = false;
          Blynk.setProperty(V10, "color", GARDENTEK_BLUE);               
          Blynk.setProperty(V18, "isHidden", true);
        }
      }
      
      //Run the daily timer	if config is valid      
      if ( D2_Daily_Timer_Fault_prev == false ) { //Do this code if we don't have a timer config fault

        if ( D2_Daily_Timer_ON_Time < D2_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D2_Daily_Timer_ON_Time && current_seconds_after_midnight < D2_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay2_out, HIGH);
            D2_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay2_out, LOW); 
            D2_RelayCmd = 0;
          }
        }
        if ( D2_Daily_Timer_ON_Time > D2_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D2_Daily_Timer_ON_Time || current_seconds_after_midnight < D2_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay2_out, HIGH);
            D2_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay2_out, LOW);
            D2_RelayCmd = 0;
          }
        }
      }
      break;
    
    case 3: //CYCLE TIMER
      if ( D2_ModeCmd != D2_ModeCmd_prev ) {                    //Do this once when entering CYCLE mode
        Blynk.setProperty(V18, "isHidden", true);
        Blynk.virtualWrite(V10, 3);
      }

      //Check for INVALID CONFIG and display warning and set MODE bar color
      if ( D2_ModeCmd != D2_ModeCmd_prev || D2_Cycle_Timer_ON_Length != D2_Cycle_Timer_ON_Length_prev || D2_Cycle_Timer_OFF_Length != D2_Cycle_Timer_OFF_Length_prev ) {
        D2_ResetCycleTimer = true;
        if ( D2_Cycle_Timer_ON_Length == 0 || D2_Cycle_Timer_OFF_Length == 0 ) {
          Blynk.setProperty(V10, "color", WARNING_RED); 
          Blynk.setProperty(V19, "isHidden", false);               
          //digitalWrite(relay2_out, LOW);
          D2_RelayCmd = 0;
          D2_Cycle_Timer_Fault_prev = true;
        }
        else {
          Blynk.setProperty(V10, "color", GARDENTEK_GREEN);
          Blynk.setProperty(V19, "isHidden", true);  
          D2_Cycle_Timer_Fault_prev = false;        
        }
      }

      //Run the cycle timer if config is valid
      if ( D2_Cycle_Timer_Fault_prev == false ) {

	      //Reset cycle timer and calculate times (counts)      
      	if ( D2_ResetCycleTimer == true ) { //|| D2_Cycle_Timer_ON_Length != D2_Cycle_Timer_ON_Length_prev || D2_Cycle_Timer_OFF_Length != D2_Cycle_Timer_OFF_Length_prev ) {
          //Blynk.setProperty(V10, "color", GARDENTEK_GREEN);
          //Blynk.setProperty(V19, "isHidden", true); 
          D2_ResetCycleTimer = false;	        		              //clear this in case this is how we got here
          D2_Cycle_Timer_ON_Counts = D2_Cycle_Timer_ON_Length / loop_rate;
	        D2_Cycle_Timer_OFF_Counts = D2_Cycle_Timer_OFF_Length / loop_rate;
	        D2_Cycle_ON_current_count = 1;
          D2_Cycle_OFF_current_count = 1;
        }

	      //Turn ON for calculated counts and the OFF for calculated counts. Then reset.      
        if ( D2_Cycle_ON_current_count <= D2_Cycle_Timer_ON_Counts ) {
          D2_Cycle_ON_current_count++;
	        //digitalWrite(relay2_out, HIGH);
          D2_RelayCmd = 1;
	      }  
	      else {
  	      if ( D2_Cycle_OFF_current_count <= D2_Cycle_Timer_OFF_Counts - 1 ) {   //Subtract 1 count that will be used to set the reset flag true 
	          D2_Cycle_OFF_current_count++;	
            //digitalWrite(relay2_out, LOW);
	          D2_RelayCmd = 0;
    	    }
	        else {                                                                 //Here we use the subtracted count to write reset flag true
	          D2_ResetCycleTimer = true;
            //digitalWrite(relay2_out, LOW);
            D2_RelayCmd = 0;	
  	      }
	      }     
      }
      break;
  }

  if ( D2_RelayCmd != D2_RelayCmd_prev || main_loop_firstcall == true ) {
    if ( D2_RelayCmd == 0 ) {
      digitalWrite(relay2_out, LOW);
    }
    else {
      digitalWrite(relay2_out, HIGH);
    }
  }
  D2_RelayCmd_prev = D2_RelayCmd;
}

void D3_control_loop() {    
  
  if ( D3_ModeCmdRqst != D3_ModeCmdRqst_prev ) {                  //Use a placeholder so Blynk doesn't write a control variable mid-loop
    D3_ModeCmd = D3_ModeCmdRqst;
      if ( D3_ModeCmd != 3 ) {                                  
        D3_ResetCycleTimer = true;                              
      }
  }
  D3_ModeCmdRqst_prev = D3_ModeCmdRqst;
  
  switch (D3_ModeCmd) {                                     
    
    case 0: //OFF
	    
      if ( digitalRead(relay3_out) != LOW || D3_ModeCmd != D3_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V20, "color", WARNING_RED);              
        Blynk.setProperty(V28, "isHidden", true);
        Blynk.setProperty(V29, "isHidden", true);
        Blynk.virtualWrite(V20, 0);
        //digitalWrite(relay3_out, LOW);
        D3_RelayCmd = 0;
      }
      break; 

    case 1: //ON
	    
      if ( digitalRead(relay3_out) != HIGH || D3_ModeCmd != D3_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V20, "color", GARDENTEK_GREEN);            
        Blynk.setProperty(V28, "isHidden", true);
        Blynk.setProperty(V29, "isHidden", true);
        Blynk.virtualWrite(V20, 1);
        //digitalWrite(relay3_out, HIGH);
        D3_RelayCmd = 1;
      }
      break;

    case 2: //DAILY TIMER
      if ( D3_ModeCmd != D3_ModeCmd_prev ) {                    //Do this once when entering DAILY mode
        Blynk.setProperty(V29, "isHidden", true);
        Blynk.virtualWrite(V20, 2);
      } 

      //Check for INVALID CONFIG and display warning and set MODE bar color	  
      if ( D3_ModeCmd != D3_ModeCmd_prev || D3_Daily_Timer_ON_Time != D3_Daily_Timer_ON_Time_prev || D3_Daily_Timer_OFF_Time != D3_Daily_Timer_OFF_Time_prev ) {
        if ( ( D3_Daily_Timer_ON_Time == 0 || D3_Daily_Timer_OFF_Time == 0 || D3_Daily_Timer_ON_Time == D3_Daily_Timer_OFF_Time ) ) {
          Blynk.setProperty(V20, "color", WARNING_RED);          
          Blynk.setProperty(V28, "isHidden", false);		
          //digitalWrite(relay3_out, LOW);
          D3_RelayCmd = 0;
          D3_Daily_Timer_Fault_prev = true;
        }
        else {
          D3_Daily_Timer_Fault_prev = false;
          Blynk.setProperty(V20, "color", GARDENTEK_BLUE);               
          Blynk.setProperty(V28, "isHidden", true);
        }
      }
      
      //Run the daily timer	if config is valid      
      if ( D3_Daily_Timer_Fault_prev == false ) { //Do this code if we don't have a timer config fault

        if ( D3_Daily_Timer_ON_Time < D3_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D3_Daily_Timer_ON_Time && current_seconds_after_midnight < D3_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay3_out, HIGH);
            D3_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay3_out, LOW); 
            D3_RelayCmd = 0;
          }
        }
        if ( D3_Daily_Timer_ON_Time > D3_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D3_Daily_Timer_ON_Time || current_seconds_after_midnight < D3_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay3_out, HIGH);
            D3_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay3_out, LOW);
            D3_RelayCmd = 0;
          }
        }
      }
      break;
    
    case 3: //CYCLE TIMER
      if ( D3_ModeCmd != D3_ModeCmd_prev ) {                    //Do this once when entering CYCLE mode
        Blynk.setProperty(V28, "isHidden", true);
        Blynk.virtualWrite(V20, 3);
      }

      //Check for INVALID CONFIG and display warning and set MODE bar color
      if ( D3_ModeCmd != D3_ModeCmd_prev || D3_Cycle_Timer_ON_Length != D3_Cycle_Timer_ON_Length_prev || D3_Cycle_Timer_OFF_Length != D3_Cycle_Timer_OFF_Length_prev ) {
        D3_ResetCycleTimer = true;
        if ( D3_Cycle_Timer_ON_Length == 0 || D3_Cycle_Timer_OFF_Length == 0 ) {
          Blynk.setProperty(V20, "color", WARNING_RED); 
          Blynk.setProperty(V29, "isHidden", false);               
          //digitalWrite(relay3_out, LOW);
          D3_RelayCmd = 0;
          D3_Cycle_Timer_Fault_prev = true;
        }
        else {
          Blynk.setProperty(V20, "color", GARDENTEK_GREEN);
          Blynk.setProperty(V29, "isHidden", true);  
          D3_Cycle_Timer_Fault_prev = false;        
        }
      }

      //Run the cycle timer if config is valid
      if ( D3_Cycle_Timer_Fault_prev == false ) {

	      //Reset cycle timer and calculate times (counts)      
      	if ( D3_ResetCycleTimer == true ) { //|| D3_Cycle_Timer_ON_Length != D3_Cycle_Timer_ON_Length_prev || D3_Cycle_Timer_OFF_Length != D3_Cycle_Timer_OFF_Length_prev ) {
          //Blynk.setProperty(V20, "color", GARDENTEK_GREEN);
          //Blynk.setProperty(V29, "isHidden", true); 
          D3_ResetCycleTimer = false;	        		              //clear this in case this is how we got here
          D3_Cycle_Timer_ON_Counts = D3_Cycle_Timer_ON_Length / loop_rate;
	        D3_Cycle_Timer_OFF_Counts = D3_Cycle_Timer_OFF_Length / loop_rate;
	        D3_Cycle_ON_current_count = 1;
          D3_Cycle_OFF_current_count = 1;
        }

	      //Turn ON for calculated counts and the OFF for calculated counts. Then reset.      
        if ( D3_Cycle_ON_current_count <= D3_Cycle_Timer_ON_Counts ) {
          D3_Cycle_ON_current_count++;
	        //digitalWrite(relay3_out, HIGH);
          D3_RelayCmd = 1;
	      }  
	      else {
  	      if ( D3_Cycle_OFF_current_count <= D3_Cycle_Timer_OFF_Counts - 1 ) {   //Subtract 1 count that will be used to set the reset flag true 
	          D3_Cycle_OFF_current_count++;	
            //digitalWrite(relay3_out, LOW);
	          D3_RelayCmd = 0;
    	    }
	        else {                                                                 //Here we use the subtracted count to write reset flag true
	          D3_ResetCycleTimer = true;
            //digitalWrite(relay3_out, LOW);
            D3_RelayCmd = 0;	
  	      }
	      }     
      }
      break;
  }

  if ( D3_RelayCmd != D3_RelayCmd_prev || main_loop_firstcall == true ) {
    if ( D3_RelayCmd == 0 ) {
      digitalWrite(relay3_out, LOW);
    }
    else {
      digitalWrite(relay3_out, HIGH);
    }
  }
  D3_RelayCmd_prev = D3_RelayCmd;
}

void D4_control_loop() {    
  
  if ( D4_ModeCmdRqst != D4_ModeCmdRqst_prev ) {                  //Use a placeholder so Blynk doesn't write a control variable mid-loop
    D4_ModeCmd = D4_ModeCmdRqst;
      if ( D4_ModeCmd != 3 ) {                                  
        D4_ResetCycleTimer = true;                              
      }
  }
  D4_ModeCmdRqst_prev = D4_ModeCmdRqst;
  
  switch (D4_ModeCmd) {                                     
    
    case 0: //OFF
	    
      if ( digitalRead(relay4_out) != LOW || D4_ModeCmd != D4_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V30, "color", WARNING_RED);              
        Blynk.setProperty(V38, "isHidden", true);
        Blynk.setProperty(V39, "isHidden", true);
        Blynk.virtualWrite(V30, 0);
        //digitalWrite(relay4_out, LOW);
        D4_RelayCmd = 0;
      }
      break; 

    case 1: //ON
	    
      if ( digitalRead(relay4_out) != HIGH || D4_ModeCmd != D4_ModeCmd_prev ) { //Do this once when entering DAILY mode or if output did not write
        Blynk.setProperty(V30, "color", GARDENTEK_GREEN);            
        Blynk.setProperty(V38, "isHidden", true);
        Blynk.setProperty(V39, "isHidden", true);
        Blynk.virtualWrite(V30, 1);
        //digitalWrite(relay4_out, HIGH);
        D4_RelayCmd = 1;
      }
      break;

    case 2: //DAILY TIMER
      if ( D4_ModeCmd != D4_ModeCmd_prev ) {                    //Do this once when entering DAILY mode
        Blynk.setProperty(V39, "isHidden", true);
        Blynk.virtualWrite(V30, 2);
      } 

      //Check for INVALID CONFIG and display warning and set MODE bar color	  
      if ( D4_ModeCmd != D4_ModeCmd_prev || D4_Daily_Timer_ON_Time != D4_Daily_Timer_ON_Time_prev || D4_Daily_Timer_OFF_Time != D4_Daily_Timer_OFF_Time_prev ) {
        if ( ( D4_Daily_Timer_ON_Time == 0 || D4_Daily_Timer_OFF_Time == 0 || D4_Daily_Timer_ON_Time == D4_Daily_Timer_OFF_Time ) ) {
          Blynk.setProperty(V30, "color", WARNING_RED);          
          Blynk.setProperty(V38, "isHidden", false);		
          //digitalWrite(relay4_out, LOW);
          D4_RelayCmd = 0;
          D4_Daily_Timer_Fault_prev = true;
        }
        else {
          D4_Daily_Timer_Fault_prev = false;
          Blynk.setProperty(V30, "color", GARDENTEK_BLUE);               
          Blynk.setProperty(V38, "isHidden", true);
        }
      }
      
      //Run the daily timer	if config is valid      
      if ( D4_Daily_Timer_Fault_prev == false ) { //Do this code if we don't have a timer config fault

        if ( D4_Daily_Timer_ON_Time < D4_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D4_Daily_Timer_ON_Time && current_seconds_after_midnight < D4_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay4_out, HIGH);
            D4_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay4_out, LOW); 
            D4_RelayCmd = 0;
          }
        }
        if ( D4_Daily_Timer_ON_Time > D4_Daily_Timer_OFF_Time ) {
          if ( current_seconds_after_midnight >= D4_Daily_Timer_ON_Time || current_seconds_after_midnight < D4_Daily_Timer_OFF_Time ) {
            //digitalWrite(relay4_out, HIGH);
            D4_RelayCmd = 1;
          }
          else {
            //digitalWrite(relay4_out, LOW);
            D4_RelayCmd = 0;
          }
        }
      }
      break;
    
    case 3: //CYCLE TIMER
      if ( D4_ModeCmd != D4_ModeCmd_prev ) {                    //Do this once when entering CYCLE mode
        Blynk.setProperty(V38, "isHidden", true);
        Blynk.virtualWrite(V30, 3);
      }

      //Check for INVALID CONFIG and display warning and set MODE bar color
      if ( D4_ModeCmd != D4_ModeCmd_prev || D4_Cycle_Timer_ON_Length != D4_Cycle_Timer_ON_Length_prev || D4_Cycle_Timer_OFF_Length != D4_Cycle_Timer_OFF_Length_prev ) {
        D4_ResetCycleTimer = true;
        if ( D4_Cycle_Timer_ON_Length == 0 || D4_Cycle_Timer_OFF_Length == 0 ) {
          Blynk.setProperty(V30, "color", WARNING_RED); 
          Blynk.setProperty(V39, "isHidden", false);               
          //digitalWrite(relay4_out, LOW);
          D4_RelayCmd = 0;
          D4_Cycle_Timer_Fault_prev = true;
        }
        else {
          Blynk.setProperty(V30, "color", GARDENTEK_GREEN);
          Blynk.setProperty(V39, "isHidden", true);  
          D4_Cycle_Timer_Fault_prev = false;        
        }
      }

      //Run the cycle timer if config is valid
      if ( D4_Cycle_Timer_Fault_prev == false ) {

	      //Reset cycle timer and calculate times (counts)      
      	if ( D4_ResetCycleTimer == true ) { //|| D4_Cycle_Timer_ON_Length != D4_Cycle_Timer_ON_Length_prev || D4_Cycle_Timer_OFF_Length != D4_Cycle_Timer_OFF_Length_prev ) {
          //Blynk.setProperty(V30, "color", GARDENTEK_GREEN);
          //Blynk.setProperty(V39, "isHidden", true); 
          D4_ResetCycleTimer = false;	        		              //clear this in case this is how we got here
          D4_Cycle_Timer_ON_Counts = D4_Cycle_Timer_ON_Length / loop_rate;
	        D4_Cycle_Timer_OFF_Counts = D4_Cycle_Timer_OFF_Length / loop_rate;
	        D4_Cycle_ON_current_count = 1;
          D4_Cycle_OFF_current_count = 1;
        }

	      //Turn ON for calculated counts and the OFF for calculated counts. Then reset.      
        if ( D4_Cycle_ON_current_count <= D4_Cycle_Timer_ON_Counts ) {
          D4_Cycle_ON_current_count++;
	        //digitalWrite(relay4_out, HIGH);
          D4_RelayCmd = 1;
	      }  
	      else {
  	      if ( D4_Cycle_OFF_current_count <= D4_Cycle_Timer_OFF_Counts - 1 ) {   //Subtract 1 count that will be used to set the reset flag true 
	          D4_Cycle_OFF_current_count++;	
            //digitalWrite(relay4_out, LOW);
	          D4_RelayCmd = 0;
    	    }
	        else {                                                                 //Here we use the subtracted count to write reset flag true
	          D4_ResetCycleTimer = true;
            //digitalWrite(relay4_out, LOW);
            D4_RelayCmd = 0;	
  	      }
	      }     
      }
      break;
  }

  if ( D4_RelayCmd != D4_RelayCmd_prev || main_loop_firstcall == true ) {
    if ( D4_RelayCmd == 0 ) {
      digitalWrite(relay4_out, LOW);
    }
    else {
      digitalWrite(relay4_out, HIGH);
    }
  }
  D4_RelayCmd_prev = D4_RelayCmd;
}

//This function is called in the 'app_loop' function in BlynkEdgent.h. This function runs constantly whether connected to wifi or not.
//If connected to wifi but not the Blynk server, there will be intermittent interuptions while the device tries to connect to the Blynk server.
//Here we use a simple millis timer to execute the code within at a hard-coded loop rate.
//If millis rolls over to 0, the very next control loop may execute faster than the loop rate but will not create any other issues.
void main_loop() { loop_rate = 250; //set loop_rate (ms) here
  
  if ( ButtonPressed == true && main_loop_firstcall == false ) {
    PanelControl();
  }
 
  if ( ((millis() - PreviousMillis) >= loop_rate) || ( millis() < PreviousMillis )) { 
    PreviousMillis = millis();

    if ( main_loop_firstcall == true ) {
      
      //EEPROM READ:
      //D1_ModeCmd, D2_ModeCmd, D3_ModeCmd, D4_ModeCmd
      //D1_Daily_Timer_ON_Time, D2_Daily_Timer_ON_Time, D3_Daily_Timer_ON_Time, D4_Daily_Timer_ON_Time
      //D1_Daily_Timer_OFF_Time, D2_Daily_Timer_OFF_Time, D3_Daily_Timer_OFF_Time, D4_Daily_Timer_OFF_Time
      //D1_Cycle_Timer_ON_Length, D2_Cycle_Timer_ON_Length, D3_Cycle_Timer_ON_Length, D4_Cycle_Timer_ON_Length
      //D1_Cycle_Timer_OFF_Length, D2_Cycle_Timer_OFF_Length, D3_Cycle_Timer_OFF_Length, D4_Cycle_Timer_OFF_Length

      D1_ResetCycleTimer = true;
      D2_ResetCycleTimer = true;
      D3_ResetCycleTimer = true;
      D4_ResetCycleTimer = true;
      
      //Initialize panel 
      timer_segment = 0;
      display_device = 0;
      PanelControl();

      //Simulated EEPROM READS///////////////////////////TEST CODE
      D1_ModeCmdRqst = 3;
      D1_Cycle_Timer_ON_Length = 5000;
      D1_Cycle_Timer_OFF_Length = 3000;
      testvalue = 0;
      EEPROM.write(5, testvalue);
      ////////////////////////////////////////////////////////////
    }

    //TEST CODE #remove////////////////////////////////////////////
    Serial.println(testvalue);
    Serial.println(EEPROM.read(5));
    ///////////////////////////////////////////////////////////////
    GetRTC();

    Blynk_Enter_Connected_Mode = Blynk_Enter_Connected_Mode_rqst; //Read the Blynk request here instead of controlling based on the Blynk request which can happen anywhere in the loop

    if (Blynk.connected() == true) {
      Serial.println("BLYNK SERVER CONNECTED");

      //Sync all datastreams from server and reset alarm states and alarm flags
      if ( Blynk_Enter_Connected_Mode ) {
        Blynk.syncAll();                                        //sync with server on first connect or reconnect
        ReArmAlarms();
      } 

      Blynk.sendInternal("rtc", "sync");                        //query Blynk server for unix rtc. This will write to Blynk server pin "InternalPinRTC"
                               
      current_unix_timestamp.getDateTime(current_UNIX_time);    //read "InternalPinRTC" unix time and format to usable time
      current_seconds_after_midnight = current_unix_timestamp.second + current_unix_timestamp.minute * 60 + current_unix_timestamp.hour * 60 * 60;            
      
      if ((current_seconds_after_midnight > (rtc_seconds_after_midnight + 30)) || (current_seconds_after_midnight < (rtc_seconds_after_midnight - 30))) {
        SetRTC();
      }

      SendRelayCoils();
      DetectAndSendRelayFaults();
      CheckHideDevices();
      CheckDeviceNameChanges();

      Blynk_Enter_Connected_Mode_rqst = false;
    }
    else { 
      Serial.println("BLYNK SERVER DISCONNECTED");
      current_seconds_after_midnight = rtc_seconds_after_midnight;
    }  
    
    D1_control_loop();
    D2_control_loop();
    D3_control_loop();
    D4_control_loop();
    
    EEPROM_Update();
    wifistrength();

    D1_ModeCmd_prev = D1_ModeCmd;
    D1_Daily_Timer_ON_Time_prev = D1_Daily_Timer_ON_Time;
    D1_Daily_Timer_OFF_Time_prev = D1_Daily_Timer_OFF_Time;
    D1_Cycle_Timer_ON_Length_prev = D1_Cycle_Timer_ON_Length;
    D1_Cycle_Timer_OFF_Length_prev = D1_Cycle_Timer_OFF_Length;
    
    D2_ModeCmd_prev = D2_ModeCmd;
    D2_Daily_Timer_ON_Time_prev = D2_Daily_Timer_ON_Time;
    D2_Daily_Timer_OFF_Time_prev = D2_Daily_Timer_OFF_Time;
    D2_Cycle_Timer_ON_Length_prev = D2_Cycle_Timer_ON_Length;
    D2_Cycle_Timer_OFF_Length_prev = D2_Cycle_Timer_OFF_Length;
    
    D3_ModeCmd_prev = D3_ModeCmd;
    D3_Daily_Timer_ON_Time_prev = D3_Daily_Timer_ON_Time;
    D3_Daily_Timer_OFF_Time_prev = D3_Daily_Timer_OFF_Time;
    D3_Cycle_Timer_ON_Length_prev = D3_Cycle_Timer_ON_Length;
    D3_Cycle_Timer_OFF_Length_prev = D3_Cycle_Timer_OFF_Length;
    
    D4_ModeCmd_prev = D4_ModeCmd;
    D4_Daily_Timer_ON_Time_prev = D4_Daily_Timer_ON_Time;
    D4_Daily_Timer_OFF_Time_prev = D4_Daily_Timer_OFF_Time;
    D4_Cycle_Timer_ON_Length_prev = D4_Cycle_Timer_ON_Length;
    D4_Cycle_Timer_OFF_Length_prev = D4_Cycle_Timer_OFF_Length;

    main_loop_firstcall = false;

    //Comment all these prints out for customer devices
    Serial.print("**************************MAIN LOOP -- RTC Time:  ");
    Serial.printf("%02d", rtc_time.Hour);
    Serial.print(":");
    Serial.printf("%02d", rtc_time.Minute);
    Serial.print(":");
    Serial.printf("%02d", rtc_time.Second);
    Serial.print("  ");
    Serial.print(rtc_time.Month);
    Serial.print("-");
    Serial.print(rtc_time.Day);
    Serial.print("-");
    Serial.print(tmYearToCalendar(rtc_time.Year));
    Serial.println("  **************************");
    
    Serial.print("RTC seconds after midnight:        ");
    Serial.println(rtc_seconds_after_midnight);
    Serial.print("Current seconds after midnight:    ");
    Serial.println(current_seconds_after_midnight);

    Serial.print("WiFi Signal Strength: ");
    Serial.println(signalquality);  
	  
    Serial.print(D1_Name);
    Serial.print(":                    ");
    Serial.print(D1_ModeCmd);
    Serial.print(" (");
    Serial.print(D1_RelayCmd);
    Serial.println(")");
	  
    Serial.print(D2_Name);
    Serial.print(":                    ");
    Serial.print(D2_ModeCmd);
    Serial.print(" (");
    Serial.print(D2_RelayCmd);
    Serial.println(")");
	  
    Serial.print(D3_Name);
    Serial.print(":                    ");
    Serial.print(D3_ModeCmd);
    Serial.print(" (");
    Serial.print(D3_RelayCmd);
    Serial.println(")");
	  
    Serial.print(D4_Name);
    Serial.print(":                    ");
    Serial.print(D4_ModeCmd);
    Serial.print(" (");
    Serial.print(D4_RelayCmd);
    Serial.println(")");

    
  }
}
