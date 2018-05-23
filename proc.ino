#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn


static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
    const uint32_t rc = VARIANT_MCK / 256 / frequency; 
    tone_pin = ulPin;
    toggle_count = 0;  
    if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
     else toggle_count = -1;

    if (!TCChanEnabled) {
      pmc_set_writeprotect(false);
      pmc_enable_periph_clk((uint32_t)TONE_IRQ);
      TC_Configure(chTC, chNo,
        TC_CMR_TCCLKS_TIMER_CLOCK4 |
        TC_CMR_WAVE |        
        TC_CMR_WAVSEL_UP_RC ); 
        
      chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  
      chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
       NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
    }
    if (!pinEnabled[ulPin]) {
      pinMode(ulPin, OUTPUT);
      pinEnabled[ulPin] = 1;
    }
    TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);     
                TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
  TC_Stop(chTC, chNo);  // stop timer
  digitalWrite(ulPin,LOW);  // no signal on pin
}

void TC3_Handler ( void ) {
  TC_GetStatus(TC1, 0);
  if (toggle_count != 0){
    // toggle pin  TODO  better
    digitalWrite(tone_pin,pin_state= !pin_state);
    if (toggle_count > 0) toggle_count--;
  } else {
    noTone(tone_pin);
  }
}



#define HC06 Serial3
const int v1 = 3;
const int v2 = 9;
const int v3 = 10;
const int v4 = 11;
int trig = 0;
int val = 0;
int force_tr = 255;
bool act = true;



void setup() 
{
  HC06.begin(9600);
  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(v3, OUTPUT);
  pinMode(v4, OUTPUT);
  digitalWrite(v1, LOW);
  digitalWrite(v2, LOW);
  digitalWrite(v3, LOW);
  digitalWrite(v4, LOW);
}


void loop() 
{
   if (HC06.available())
   {
        val = HC06.read();
        // val = Serial.read();
          switch(val)
          {
          case '1':
          {
            trig = 1;  
            break;  
          }
          case '0':
          {
            trig = 0;  
            break;  
          }
          case '2':
          {
            trig = 2;  
            break;
          }
          case '3':
          {
            trig = 3;  
            break;
          }
          case '4':
          {
            trig = 4;  
            break;
          }
          case '5':
          {
            force_tr = 100;
            break;
          }
      }}
      switch(trig)
  {
    case 0:
    {
      analogWrite(v1, 0);
      analogWrite(v2, 0);
      analogWrite(v3, 0);
      analogWrite(v4, 0);
      break;
    }
    case 1:
    {
      
      tone(8, 750vv, 250);
      delay(250);
      trig=0;
      break;
    }
    case 2:
    {
      tone(8, 500, 250);
      delay(250);
      trig=0;
      break;
    }
    }
  }
   
  
