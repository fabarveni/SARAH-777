  /*****************************************************************************
  *                     ARVENI radio transmitter ON/OFF.
  ******************************************************************************
  * File Name    : CC110L.h
  * Version      : V1.00
  * Device       : R5F1026A
  ******************************************************************************
  * WATTS Electronics
  * Main author : Mickael MOREY              V1.00
  ******************************************************************************
  * Product :   P06171/00 (WATTS reference), 888A (ARVENI reference)
  *             ARVENI radio receiver ON/OFF.
  ******************************************************************************
  * Define commandes and registers addresse of CC110L IHM.
  *****************************************************************************/

#ifndef _H_CC110L
#define _H_CC110L


/*** Command Strobes p53 ***/
#define         SRES                            0x30                            // Reset chip.
#define         SFSTXON                         0x31                            // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA): Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
#define         SXOFF                           0x32                            // Turn off crystal oscillator.
#define         SCAL                            0x33                            // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without setting manual calibration mode (MCSM0.FS_AUTOCAL=0) 
#define         SRX                             0x34                            // In IDLE state: Enable RX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
#define         STX                             0x35                            // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled: Only go to TX if channel is clear.
#define         SIDLE                           0x36                            // Enter IDLE state.
#define         SPWD                            0x39                            // Enter power down mode when CSn goes high.
#define         SFRX                            0x3A                            // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states.
#define         SFTX                            0x3B                            // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states.
#define         SNOP                            0x3D                            // No operation. May be used to get access to the chip status byte.

/*** Registers acces mode ***/
#define         WRITE_SINGLE                    0x00
#define         WRITE_BURST                     0x40
#define         READ_SINGLE                     0x80
#define         READ_BURST                      0xC0

/*** Configuration registers addresses p54 ***/
#define         ADDR_IOCFG2                     0x00                            // GDO2 output pin configuration. p57
#define         ADDR_IOCFG1                     0x01                            // GDO1 output pin configuration. p57
#define         ADDR_IOCFG0                     0x02                            // GDO0 output pin configuration. p57
#define         ADDR_FIFOTHR                    0x03                            // RX FIFO and TX FIFO thresholds. p58
#define         ADDR_SYNC1                      0x04                            // Sync word, high byte. p59
#define         ADDR_SYNC0                      0x05                            // Sync word, low byte. p59
#define         ADDR_PKTLEN                     0x06                            // Packet length. p59
#define         ADDR_PKTCTRL1                   0x07                            // Packet automation control. p59
#define         ADDR_PKTCTRL0                   0x08                            // Packet automation control. p60
#define         ADDR_ADDR                       0x09                            // Device address. p60
#define         ADDR_CHANNR                     0x0A                            // Channel number. p60
#define         ADDR_FSCTRL1                    0x0B                            // Frequency synthesizer control. p60
#define         ADDR_FSCTRL0                    0x0C                            // Frequency synthesizer control. p61
#define         ADDR_FREQ2                      0x0D                            // Frequency control word, high byte. p61
#define         ADDR_FREQ1                      0x0E                            // Frequency control word, middle byte. p61
#define         ADDR_FREQ0                      0x0F                            // Frequency control word, low byte. p61
#define         ADDR_MDMCFG4                    0x10                            // Modem configuration. p61
#define         ADDR_MDMCFG3                    0x11                            // Modem configuration. p61
#define         ADDR_MDMCFG2                    0x12                            // Modem configuration. p62
#define         ADDR_MDMCFG1                    0x13                            // Modem configuration. p63
#define         ADDR_MDMCFG0                    0x14                            // Modem configuration. p63
#define         ADDR_DEVIATN                    0x15                            // Modem deviation setting. p64
#define         ADDR_MCSM2                      0x16                            // Main Radio Control State Machine configuration. p64
#define         ADDR_MCSM1                      0x17                            // Main Radio Control State Machine configuration. p65
#define         ADDR_MCSM0                      0x18                            // Main Radio Control State Machine configuration. p66
#define         ADDR_FOCCFG                     0x19                            // Frequency Offset Compensation configuration. p67
#define         ADDR_BSCFG                      0x1A                            // Bit Synchronization configuration. p68
#define         ADDR_AGCCTRL2                   0x1B                            // AGC control. p69
#define         ADDR_AGCCTRL1                   0x1C                            // AGC control. p70
#define         ADDR_AGCCTRL0                   0x1D                            // AGC control. p71
#define         ADDR_FREND1                     0x21                            // Front end RX configuration.  p71
#define         ADDR_FREND0                     0x22                            // Front end TX configuration. p72
#define         ADDR_FSCAL3                     0x23                            // Frequency synthesizer calibration. p72
#define         ADDR_FSCAL2                     0x24                            // Frequency synthesizer calibration. p72
#define         ADDR_FSCAL1                     0x25                            // Frequency synthesizer calibration. p72
#define         ADDR_FSCAL0                     0x26                            // Frequency synthesizer calibration. p73
#define         ADDR_TEST2                      0x2C                            // Various test settings. p73
#define         ADDR_TEST1                      0x2D                            // Various test settings. p73
#define         ADDR_TEST0                      0x2E                            // Various test settings. p73
#define         ADDR_PATABLE                    0x3E

/*** Stats Registers p55 ***/
#define         PARTNUM                         0xF0                            // Part number for CC110L. p74
#define         VERSION                         0xF1                            // Current version number p74
#define         FREQEST                         0xF2                            // Frequency Offset Estimate p74
#define         CRC_REG                         0xF3                            // CRC OK p74
#define         RSSI                            0xF4                            // Received signal strength indication p74
#define         MARCSTATE                       0xF5                            // Control state machine state p75
#define         PKTSTATUS                       0xF8                            // Current GDOx status and packet status p76
#define         TXBYTES                         0xFA                            // Underflow and number of bytes in the TX FIFO p76
#define         RXBYTES                         0xFB                            // Overflow and number of bytes in the RX FIFO p76

/*** Fifo ***/
#define         ADDR_TX_FIFO                    0x3F
#define         ADDR_RX_FIFO                    0x3F + READ_SINGLE

#endif