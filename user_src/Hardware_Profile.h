  /*****************************************************************************
  *                     ARVENI radio transmitter ON/OFF.
  ******************************************************************************
  * File Name    : Hardware_Profile.h
  * Version      : V1.00
  * Device       : R5F1026A
  ******************************************************************************
  * WATTS Electronics
  * Main author : Mickael MOREY              V1.00
  ******************************************************************************
  * Product :   P06171/00 (WATTS reference), 888A (ARVENI reference)
  *             ARVENI radio receiver ON/OFF.
  *****************************************************************************/

#ifndef _H_HARDWARE_PROFILE
#define _H_HARDWARE_PROFILE

/*
                    RL78/G12
              --------------------
            * |               ANI1 | <-  Valim
Shunt_VCR <-  | P42                | *
Shunt_VCR <-  | P41     R          | *
            * |         5    SCK00 |  -> SCK
            * |         F     SI00 | <-  MISO
      BP1  -> | P137    1     SO00 |  -> MOSI
            * |         0    INTP2 | <-  GDO0
            * |         2      P14 |  -> /CS
      Vss     |         6          | *
      Vdd     |                    | *
              --------------------
*/

/*** Ports ***/
#define         nCS_CC110L                      P1_bit.no4
#define         SCK_CC110L                      P1_bit.no0
#define         MISO_CC110L                     P1_bit.no1
#define         MOSI_CC110L                     P1_bit.no2
#define         GDO0_CC110L                     
#define         BP1                             P13_bit.no7
#define         VALIM                           
#define         SHUNT_VCR                       P4
#define         GDO0                            P1_bit.no3
/*** Analogue input ***/
#define         CHANEL_ANI_VALIM                1
/*** Port mode (direction) ***/
#define         PM_nCS_CC110L                   PM1_bit.no4
#define         PM_SCK_CC110L                   PM1_bit.no0
#define         PM_MISO_CC110L                  PM1_bit.no1
#define         PM_MOSI_CC110L                  PM1_bit.no2
#define         PM_GDO0_CC110L                  PM1_bit.no3
#define         PM_BP1                          PM13_bit.no7
#define         PM_SHUNT_VCR                    PM2_bit.no2
/*** A/D port configuration (analog/digital) ***/
#define         ADPC_VALIM_SHUNT_VCR            ADPC
/*** Port mode control (analog/digital) ***/
#define         PMC_SCK_CC110L                  PMC1_bit.no0
#define         PMC_MISO_CC110L                 PMC1_bit.no1
#define         PMC_MOSI_CC110L                 PMC1_bit.no2
#define         PMC_GDO0_CC110L                 PMC1_bit.no3

/*** Pins configuration definition ***/
#define         INPUT
#define         OUTPUT
#define         ANALOG
#define         DIGITAL

#endif