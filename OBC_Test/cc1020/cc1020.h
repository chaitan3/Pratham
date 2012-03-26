
// Default RF setups, stored in Flash
    
// SmartRF Studio v5.3.0.0

// Device: CC1020

// System parameters:
// X-tal frequency: 14.745600 MHz	Internal
// X-tal accuracy: +/-  5 ppm
// RF frequency A: 434.052500 MHz	Active  	Rx
// RF frequency B: 434.052500 MHz	Inactive	Tx
// Frequency separation: 4.950 kHz
// Data rate: 4.800 kBaud
// Data Format: NRZ		Accurate
// RF output power: +10 dBm
// Channel width: 25 kHz
// Modulation: FSK		Dithering enabled
// Lock: Continuous
// Carrier sense offset:   0 dBm	DCLK squelch disabled
// Operator Mode: Rx

const char DefaultConfig433[32] =
{
  0x0F,  // 0x01, INTERFACE		//DCLK_CS will be used in RX mode
  0xFF,  // 0x02, RESET
  0x8F,  // 0x03, SEQUENCING
  0x3A,  // 0x04, FREQ_2A  	39
  0x95,  // 0x05, FREQ_1A	F0
  0x69,  // 0x06, FREQ_0A	0F
  0x3A,  // 0x07, CLOCK_A
  0x3A,  // 0x08, FREQ_2B  	39
  0x95,  // 0x09, FREQ_1B	F0
  0x69,  // 0x0A, FREQ_0B	0F
  0x3A,  // 0x0B, CLOCK_B
  0x44,  // 0x0C, VCO
  0x50,  // 0x0D, MODEM
  0x0B,  // 0x0E, DEVIATION
  0xCC,  // 0x0F, AFC_CONTROL
  0x2F,  // 0x10, FILTER
  0x61,  // 0x11, VGA1
  0x55,  // 0x12, VGA2
  0x2F,  // 0x13, VGA3
  0x2D,  // 0x14, VGA4			Carrier Sense Threshold Level = 5dB
  0x10,  // 0x15, LOCK          // When not used, set as high
  0x78,  // 0x16, FRONTEND
  0x47,  // 0x17, ANALOG, RX=47/TX=47
  0x14,  // 0x18, BUFF_SWING
  0x22,  // 0x19, BUFF_CURRENT
  0xAE,  // 0x1A, PLL_BW
  0x34,  // 0x1B, CALIBRATE
  0xF0,  // 0x1C, PA_POWER		// 10dbm output power
  0x00,  // 0x1D, MATCH
  0x00,  // 0x1E, PHASE_COMP
  0x00,  // 0x1F, GAIN_COMP
  0x00   // 0x20, POWERDOWN
};


/* Constants defined for CC1020 */

/* Register addresses */

#define CC1020_MAIN            0x00
#define CC1020_INTERFACE       0x01
#define CC1020_RESET           0x02
#define CC1020_SEQUENCING      0x03
#define CC1020_FREQ_2A         0x04
#define CC1020_FREQ_1A         0x05
#define CC1020_FREQ_0A         0x06
#define CC1020_CLOCK_A         0x07
#define CC1020_FREQ_2B         0x08
#define CC1020_FREQ_1B         0x09
#define CC1020_FREQ_0B         0x0A
#define CC1020_CLOCK_B         0x0B
#define CC1020_VCO             0x0C
#define CC1020_MODEM           0x0D
#define CC1020_DEVIATION       0x0E
#define CC1020_AFC_CONTROL     0x0F
#define CC1020_FILTER          0x10
#define CC1020_VGA1            0x11
#define CC1020_VGA2            0x12
#define CC1020_VGA3            0x13
#define CC1020_VGA4            0x14
#define CC1020_LOCK            0x15
#define CC1020_FRONTEND        0x16
#define CC1020_ANALOG          0x17
#define CC1020_BUFF_SWING      0x18
#define CC1020_BUFF_CURRENT    0x19
#define CC1020_PLL_BW          0x1A
#define CC1020_CALIBRATE       0x1B
#define CC1020_PA_POWER        0x1C
#define CC1020_MATCH           0x1D
#define CC1020_PHASE_COMP      0x1E
#define CC1020_GAIN_COMP       0x1F
#define CC1020_POWERDOWN       0x20
#define CC1020_TEST1           0x21
#define CC1020_TEST2           0x22
#define CC1020_TEST3           0x23
#define CC1020_TEST4           0x24
#define CC1020_TEST5           0x25
#define CC1020_TEST6           0x26
#define CC1020_TEST7           0x27
#define CC1020_STATUS          0x40
#define CC1020_RESET_DONE      0x41
#define CC1020_RSSI            0x42
#define CC1020_AFC             0x43
#define CC1020_GAUSS_FILTER    0x44
#define CC1020_STATUS1         0x45
#define CC1020_STATUS2         0x46
#define CC1020_STATUS3         0x47
#define CC1020_STATUS4         0x48
#define CC1020_STATUS5         0x49
#define CC1020_STATUS6         0x4A
#define CC1020_STATUS7         0x4B

#define PA_POWER        0x0F			//0 dbm output power
// Constants for CC1020

#define TXANALOG 0x47

