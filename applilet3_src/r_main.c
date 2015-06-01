  /*****************************************************************************
  *                     ARVENI radio transmitter ON/OFF.
  ******************************************************************************
  * File Name    : main.c
  * Version      : V1.00
  * Device       : R5F1026A
  * Tool-Chain   : IAR Systems iccrl78
  * Description  : This file implements main function and others application
                    functions.
  * Creation Date: 09/12/2013
  ******************************************************************************
  * Product :   777A (ARVENI reference)
  *             ARVENI radio transmitter ON/OFF.
  ******************************************************************************
  * Functional description :
  *
  * The device is maximum optimised for ultra low power.
  *
  * When the µc start to be powered, it initialise himself and CC110L chip.
  * 
  * Befor any transmition, it check if Vcr voltage is under the treshold defined
  *  in this file.
  * If not, it send RF frame while the Vcr voltage is upper the treshold.
  *
  * When the voltage is under the treshold voltage, the µc stop the RF transmition
  *  and kill himself by short circut the voltage Vcr as long as possible to
  *  permit to the system to restart imediatly if the user push again on the
  *  button.
  *
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * RF frame structure :
  *             LSB                                       MSB
  *             ----------------------------------------------
  * bits        |       <0-6>        /  <7>   |    <8-39>    |  = 5 bytes
  * description | Frame size in byte / status | Unique Tx ID |
  *             ----------------------------------------------
  *****************************************************************************/

  /*** Includes ***************************************************************/
  #include "r_cg_macrodriver.h"
  #include "r_cg_userdefine.h"

  #include "..\user_src\CC110L.h"
  #include "..\user_src\Hardware_Profile.h"

  /*** Option bytes ***********************************************************/
  #pragma location = "OPTBYTE"
  __root const uint8_t opbyte0 = 0xEFU;
  #pragma location = "OPTBYTE"
  __root const uint8_t opbyte1 = 0xFFU;
  #pragma location = "OPTBYTE"
  __root const uint8_t opbyte2 = 0xAAU;
  #pragma location = "OPTBYTE"
  __root const uint8_t opbyte3 = 0x04U;

  /*** Security ID ************************************************************/
  #pragma location = "SECUID"
  __root const uint8_t secuid[10] = 
      {0x00U, 0x00U, 0x00U, 0x00U, 'V', '1', '.', '0', '0'};

  /*** Functions definitions **************************************************/
  void R_MAIN_UserInit(void);
  void SPI_Send_SINGLE ( uint8_t data );
  void SPI_Send_BURST ( uint8_t *data, uint8_t size );

/*** Minimum Vcr value [V] to send a RF frame ***/
/*** Edit this value to change the threshlold ***/
  #define VCR_MIN_TH              7.5
  /*** Don't édit the value below if hardware and ADC config doesn't change ***/
  #define R1_VALUE                150                                           // R1 value in [Kohm]
  #define R2_VALUE                1000                                          // R2 value in [Kohm]
  #define VCR_MESURE              VCR_MIN_TH*R1_VALUE/(R1_VALUE+R2_VALUE)       // = Valim [V] value (ADC input) across R1, R2 resistor bridge.
  #define VDD                     1.94                                          // VDD [V] value (régulator TPS62122 output) => Vref+ of ADC and Vref- = GND.
  #define MAX_ADC                 255                                           // ADC resolution = 8bits.
  #define TH_ADC                  (uint8_t)(VCR_MESURE/VDD*MAX_ADC)             // Value of ADC result register conresponding of Vcr threshold value.

  /*** CC110L Registers configuration *****************************************/
  uint8_t init_CC110L[ ] =
  { 0x36U, 0x42U, 0x06U, 0x07U, 0x58U, 0xACU, 0x06U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x21U, 0x65U, 0xEAU, 0xFCU, 0x3BU, 0x83U, 0x22U,
    0xF8U, 0x52U, 0x00U, 0x32U, 0x18U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x17U, 0x00U, 0x10U, 0xEAU };
  /*** RF frame content *******************************************************/
  #pragma location=0x3FF0                                                       // 15 end bytes of program flash memory are reserved for the unique identifier of the product.
  __far const uint8_t payload_CC110L[ ] =
  { 0x05U, 0x00U, 0x00U, 0x00U, 0x01U, 0x11U };                                 // Unique identifier incrémented in production for each product.

  
  /*** Code start *************************************************************/
  void main(void)
  {
      DI();                                                                     // Disable interupts.
      /*** Clock Generator Configuration (CHAPTER 5, p114) ***/

      /*** Port Configuration (CHAPTER 4, p83) ***/
        // Analog/Digital
      PMC1 = 0xE0;                                                              // SCK, SDI, SDO, GDO0 and /CS as digital I/O. p96
      PMC4 = 0xF9;
        // Input/Output
      PM1 = 0xEA;                                                               // SCK and SDO and /CS as output, SDI and GDO0 as input. p90
      PM4 = 0xF9;                                                               // P40 as input with PU activated to reduce consumption, Shunt_Vcr output. p90
      PM6 = 0xFC;                                                               // P60, P61 as output, resduce consumption when pin not used and havn't analog input. p90
        // Level output
      P1 = 0x15;                                                                // SCK, SDO and /CS output high. p92

      /*** ADC Registers configuration (CHAPTER 10, p265) ***/
      ADCEN = 1;                                                                // Enables clock supply, ADC registers can be read/write. Register PER0 p270
      ADPC = 0x03;                                                              // ANI1/P21 and ANI0/P20 as analog input, ANI3/P23 and ANI2/P22 as digital I/O. p97
      ADS = 0x01;                                                               // Select chanel ANI1. p285
      ADM0 = 0x3D;                                                              // Select mode; fAD = fCLK/2; Low voltage 1 (1.8V < Vdd < 5.5V, sampling 7 fAD); Enable ADC SAR voltage comparator. p271
      ADM1 = 0x20;                                                              // Software trigger; One-shot conversion. p280
      ADM2 = 0x01;                                                              // Vref+ = Vdd; Vref- = Vss; interrupt when result(ADCR) is between ADLL and ADUL; SNOOZE mode not used; 8bit resolution. p281-282

      ADIF = 0;                                                                 // Clear Interrupt Flag. Register IF1H p589

      /*** SPI Configuration (CHAPTER 11, p319) ***/
      SAU0EN = 1;                                                               // Enables clock supply, Serial array registers can be read/write. Register PER0 p330
      ST0L_bit.no0 = 1;                                                         // Stops the communication operation. Register ST0 p342
      SIR00 = 0x0007;                                                           // Clear error flags. p338

      SCR00 = 0xC007;                                                           // Transmission/reception, SPI mode 1, MSB first, 8-bit data length. p335
      SDR00 = 0x0000;                                                           // fMCK = fCLK/2. p336
      SO0 = 0x0101;                                                             // Serial clock output value is "1", Serial data output value is "1". p345    
      SOE0L_bit.no0 = 1;                                                        // Enables output by serial communication operation. Register SOE0 p 344
      SS0L_bit.no0 = 1;                                                         // Enters the communication wait status. Register SS0 p341

      /*** CC110L Registers configuration ***/
      nCS_CC110L = 0;
      while ( MISO_CC110L );
      SPI_Send_BURST ( (uint8_t*)&init_CC110L[0], sizeof( init_CC110L ) );
      while ( SSR00_bit.no6 );                                                  // Last SPI TX in progress.
      nCS_CC110L = 1; 
      /*** CC110L RF Output power setting ***/
      nCS_CC110L = 0;
      while ( MISO_CC110L );  
      SPI_Send_SINGLE( WRITE_SINGLE + ADDR_PATABLE );                           // PATABLE acces write mode.
      SPI_Send_SINGLE( 0xC0 );                                                  // Tx power set to maximum : 12dBm @ 868MHz.
      while ( SSR00_bit.no6 );                                                  // Last SPI TX in progress. 
      nCS_CC110L = 1;

      /*** P53/INTP2 initialisation for reading GDO0 ***/
      PMK2 = 0;                                                                 // IT Mask enabled to wake-up µc. Register MK0L p590
      EGN0_bit.no2 = 1;                                                         // IT on INTP2 falling edge. p596
      PIF2 = 0;                                                                 // Clear IT Flag. Register IF0L p588

      do
      {
        /*** Convert Vcr to check if it's possible to send RF data. ***/
        ADCS = 1;                                                               // Start conversion. Register ADM0 p271
        while ( !ADIF );                                                        // Wait end of ADC conversion.
        ADIF = 0;
        if ( ADCRH > TH_ADC )                                                   // If there is enough energy.
        {                                                                       // Send RF frame.
          /*** Load Tx FIFO ***/
          nCS_CC110L = 0;
          while ( MISO_CC110L );
          SPI_Send_SINGLE( WRITE_BURST + ADDR_TX_FIFO );                        // Acces to Tx FIFO.
          SPI_Send_BURST ( (uint8_t*)&payload_CC110L[0], sizeof( payload_CC110L ) );
          while ( SSR00_bit.no6 );                                              // Last SPI TX in progress.
          nCS_CC110L = 1;

          /*** Start RF Tx ***/
          nCS_CC110L = 0;
          while ( MISO_CC110L );
          SPI_Send_SINGLE( STX );                                               // STX commande.
          while ( SSR00_bit.no6 );                                              // Last SPI TX in progress.
          nCS_CC110L = 1;

          /*** µc enter in SLEEP (STOP) mode waiting End Tx -> GDO0 falling edge ***/
          PIF2 = 0;                                                             // End Tx -> Falling edge appened, clear IT Flag (before entring STOP mode).
          STOP( );
        }
      } while ( ADCRH > TH_ADC );                                               // Restart loop if tere was enouth energy.

      /*** Turn CC110L in IDLE mode ***/
      nCS_CC110L = 0;
      while ( MISO_CC110L );
      SPI_Send_SINGLE( SIDLE );                                                 // IDLE commande.
      while ( SSR00_bit.no6 );                                                  // Last SPI TX in progress.
      nCS_CC110L = 1;
      /*** Turn CC110L in power down mode ***/
      nCS_CC110L = 0;
      while ( MISO_CC110L );
      SPI_Send_SINGLE( SPWD );                                                  // Power down commande.
      while ( SSR00_bit.no6 );                                                  // Last SPI TX in progress.
      nCS_CC110L = 1;

      SAU0EN = 0;                                                               // Disables SAR clock supply. Register PER0 p330
      ADCEN = 0;                                                                // Disables ADC clock supply. Register PER0 p270
      SHUNT_VCR = 0x06;                                                         // Discharge Cr (use P41 and P42 hight current port).
      STOP();
  }

  /*****************************************************************************
    Descritpion :
      Write SPI single data.
    Parameters :
      Input :           data          = byte to write
  *****************************************************************************/
  void SPI_Send_SINGLE ( uint8_t data )
  {  
      SIO00 = data;                                                             // started by writing data to SDR[7:0].
      __no_operation(); __no_operation(); __no_operation(); __no_operation();   // Whait end of transfert. (optimised time)
      __no_operation(); __no_operation(); __no_operation();
  }

  /*****************************************************************************
    Descritpion :
      Write SPI datas in burst mode.
    Parameters :
      Input :           size          = nb of data bytes to write
                       *pData         = tab of data to write.
  *****************************************************************************/
  void SPI_Send_BURST ( uint8_t *pData, uint8_t size )
  {
      uint8_t ii;

      for ( ii = 0 ; ii < size ; ii++ )
      { 
          SIO00 = pData[ii];                                                     // started by writing data to SDR[7:0].
          __no_operation(); __no_operation(); __no_operation(); __no_operation();// Whait end of transfert.(optimised time)
          __no_operation(); __no_operation(); __no_operation(); __no_operation();
          __no_operation();
      }
  }