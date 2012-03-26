#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int init_CC1020(void);
void WriteToCC1020Register(unsigned char addr, unsigned char data);
unsigned char ReadFromCC1020Register(unsigned char addr);
void SetupCC1020PD(void);
void ResetCC1020(void);
void SetupCC1020Allregisters(void);
void WakeUpCC1020ToTX(char TXANALOG1);
int CalibrateCC1020(char PA_POWER1);
char SetupCC1020TX(char TXANALOG1, char PA_POWER1);

#define CAL_ATTEMPT_MAX  10

#endif
