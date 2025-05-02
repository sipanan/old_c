/* 
 * File:   ads1210.h
 * Author: Quednau
 *
 * Created on 21. Juni 2012, 16:20
 */

#ifndef ADS1210_H
#define	ADS1210_H

#ifdef	__cplusplus
extern "C" {
#endif
bool ADS1210_Initialized;
uint8_t ADS1210_INCR;
typedef union {
  struct {
      unsigned A0 : 1;
      unsigned A1 : 1;
      unsigned A2 : 1;
      unsigned A3 : 1;
      unsigned : 1;
      unsigned MB0 : 1;
      unsigned MB1 : 1;
      unsigned R_W : 1;
  };
  struct {
    unsigned ADR : 4;
    unsigned : 1;
    unsigned MB : 2;
  };
  struct {
      unsigned w : 8;
  };
} __ADS1210_INCRbits_t;
extern volatile __ADS1210_INCRbits_t ADS1210_INCRbits asm ("ADS1210_INCR");


extern uint8_t ADS1210_CRB0;
typedef union {
    struct {
        unsigned DR7_0 : 8;
    };
    struct {
        unsigned w : 8;
    };
} __ADS1210_CRB0bits_t;
extern volatile __ADS1210_CRB0bits_t ADS1210_CRB0bits asm ("ADS1210_CRB0");

extern uint8_t ADS1210_CRB1;
typedef union {
    struct {
        unsigned DR8_12 : 5;
        unsigned SF : 3;
    };
    struct {
        unsigned w : 8;
    };
} __ADS1210_CRB1bits_t;
extern volatile __ADS1210_CRB1bits_t ADS1210_CRB1bits asm ("ADS1210_CRB1");

extern uint8_t ADS1210_CRB2;
typedef union {
    struct {
        unsigned CH : 2;
        unsigned GAIN : 3;
        unsigned MODE : 3;
    };
    struct {
        unsigned w : 8;
    };
} __ADS1210_CRB2bits_t;
extern volatile __ADS1210_CRB2bits_t ADS1210_CRB2bits asm ("ADS1210_CRB2");

extern uint8_t ADS1210_CRB3;
typedef union {
    struct {
        unsigned DS_DR : 1;
        unsigned SDL : 1;
        unsigned MSB : 1;
        unsigned BD : 1;
        unsigned U_B: 1;
        unsigned DF : 1;
        unsigned REFO : 1;
        unsigned BIAS : 1;
    };
    struct {
        unsigned w : 8;
    };
} __ADS1210_CRB3bits_t;

extern volatile __ADS1210_CRB3bits_t ADS1210_CRB3bits asm ("ADS1210_CRB3");

extern uint8_t ADS1210_DOR0,ADS1210_DOR1,ADS1210_DOR2;

// Prototypes for ADS1210.C
bool ADS1210_ReadDOR(void);
void ADS1210_ReadOCR(void);
void ADS1210_ReadFCR(void);
void ADS1210_ReadCR(void);
void ADS1210_WriteCR(void);
void ADS1210_Init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADS1210_H */

