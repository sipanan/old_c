/* 
 * File:   ULN.h
 * Author: quednau
 *
 * Created on 9. Dezember 2016, 11:30
 */

#ifndef ULN_H
#define	ULN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void ULN_Init(void);
void ULN_S1(bool Set);
void ULN_S2_DC_ON(bool Set);
void ULN_S3_OBEN(bool Set);
void ULN_S4_AC_ON(bool Set);
void ULN_LED_TAST(bool Set);
void ULN_DC_AC(bool Set);
void ULN_HV_AC_ON(bool Set);
void ULN_PWRDRV(bool Set);
void ULN_OPEN(void);
void ULN_CLOSE(void);
    
extern uint8_t ULN_OUT;
typedef union {
  struct {
      unsigned RE1_S1 : 1;
      unsigned RE2_S2 : 1;
      unsigned RE3_S3 : 1;
      unsigned RE4_S4 : 1;
      unsigned RE0_LED_TAST : 1;
      unsigned RF1_DC_AC : 1;
      unsigned RF0_HV_EN : 1;
      unsigned : 1;
  };
  struct {
      unsigned w : 8;
  };
} __ULN_OUTbits_t;
extern volatile __ULN_OUTbits_t ULN_OUTbits asm ("ULN_OUT");

#ifdef	__cplusplus
}
#endif

#endif	/* ULN_H */

