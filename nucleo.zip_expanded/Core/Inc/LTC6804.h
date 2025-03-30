/*
 * LTC6804.h
 *
 *  Created on: Mar 30, 2025
 *      Author: verma
 */

#ifndef INC_LTC6804_H_
#define INC_LTC6804_H_

#include "main.h"

#define CELL 1
#define AUX 2
#define STAT 3

#define MD_422HZ_1KHZ 0
#define MD_27KHZ_14KHZ 1
#define MD_7KHZ_3KHZ 2
#define MD_26HZ_2KHZ 3

#define ADC_OPT_ENABLED 1
#define ADC_OPT_DISABLED 0

#define CELL_CH_ALL 0
#define CELL_CH_1and7 1
#define CELL_CH_2and8 2
#define CELL_CH_3and9 3
#define CELL_CH_4and10 4
#define CELL_CH_5and11 5
#define CELL_CH_6and12 6

#define SELFTEST_1 1
#define SELFTEST_2 2

#define AUX_CH_ALL 0
#define AUX_CH_GPIO1 1
#define AUX_CH_GPIO2 2
#define AUX_CH_GPIO3 3
#define AUX_CH_GPIO4 4
#define AUX_CH_GPIO5 5
#define AUX_CH_VREF2 6

#define STAT_CH_ALL 0
#define STAT_CH_SOC 1
#define STAT_CH_ITEMP 2
#define STAT_CH_VREGA 3
#define STAT_CH_VREGD 4

#define DCP_DISABLED 0
#define DCP_ENABLED 1

#define PULL_UP_CURRENT 1
#define PULL_DOWN_CURRENT 0



#define NUM_RX_BYT 8
#define CELL 1
#define AUX 2
#define STAT 3
#define CFGRA 0
#define CFGRB 4
#define CS_PIN 13

//! Cell Voltage data structure.
typedef struct
{
  uint16_t c_codes[18];//!< Cell Voltage Codes
  uint8_t pec_match[6];//!< If a PEC error was detected during most recent read cmd
} cv;

//! AUX Reg Voltage Data
typedef struct
{
  uint16_t a_codes[9];//!< Aux Voltage Codes
  uint8_t pec_match[4];//!< If a PEC error was detected during most recent read cmd
} ax;

typedef struct
{
  uint16_t stat_codes[4];//!< A two dimensional array of the stat voltage codes.
  uint8_t flags[3]; //!< byte array that contains the uv/ov flag data
  uint8_t mux_fail[1]; //!< Mux self test status flag
  uint8_t thsd[1]; //!< Thermal shutdown status
  uint8_t pec_match[2];//!< If a PEC error was detected during most recent read cmd
} st;

typedef struct
{
  uint8_t tx_data[6];
  uint8_t rx_data[8];
  uint8_t rx_pec_match;//!< If a PEC error was detected during most recent read cmd
} ic_register;

typedef struct
{
  uint16_t pec_count;
  uint16_t cfgr_pec;
  uint16_t cell_pec[6];
  uint16_t aux_pec[4];
  uint16_t stat_pec[2];
} pec_counter;

typedef struct
{
  uint8_t cell_channels;
  uint8_t stat_channels;
  uint8_t aux_channels;
  uint8_t num_cv_reg;
  uint8_t num_gpio_reg;
  uint8_t num_stat_reg;
} register_cfg;

typedef struct
{

  ic_register config;
  ic_register configb;
  cv   cells;
  ax   aux;
  st   stat;
  ic_register com;
  ic_register pwm;
  ic_register pwmb;
  ic_register sctrl;
  ic_register sctrlb;
  bool isospi_reverse;
  pec_counter crc_count;
  register_cfg ic_reg;
  long system_open_wire;
} cell_asic;

/*!   calculates  and returns the CRC15
  @returns The calculated pec15 as an unsigned int
*/
uint16_t pec15_calc(uint8_t len, //!< the length of the data array being passed to the function
                    uint8_t *data //!<  the array of data that the PEC will be generated from
                   );

/*!  Wake isoSPI up from idle state */
void wakeup_idle(uint8_t total_ic);//!< number of ICs in the daisy chain

/*!  Wake the LTC6813 from the sleep state */
void wakeup_sleep(uint8_t total_ic); //!< number of ICs in the daisy chain

/*! Sense a command to the bms IC. This code will calculate the PEC code for the transmitted command*/
void cmd_68(uint8_t tx_cmd[2]); //!< 2 Byte array containing the BMS command to be sent

//! Writes an array of data to the daisy chain
void write_68(uint8_t total_ic , //!< number of ICs in the daisy chain
              uint8_t tx_cmd[2], //!< 2 Byte array containing the BMS command to be sent
              uint8_t data[] //!< Array containing the data to be written to the BMS ICs
             );
//! Issues a command onto the daisy chain and reads back 6*total_ic data in the rx_data array
int8_t read_68( uint8_t total_ic, //!< number of ICs in the daisy chain
                uint8_t tx_cmd[2], //!< 2 Byte array containing the BMS command to be sent
                uint8_t *rx_data); //!< Array that the read back data will be stored.

/*! Starts the Mux Decoder diagnostic self test

 Running this command will start the Mux Decoder Diagnostic Self Test
 This test takes roughly 1mS to complete. The MUXFAIL bit will be updated,
 the bit will be set to 1 for a failure and 0 if the test has been passed.
 */
void LTC680x_diagn();

//! Sends the poll adc command
//! @returns 1 byte read back after a pladc command. If the byte is not 0xFF ADC conversion has completed
uint8_t LTC680x_pladc();

//! This function will block operation until the ADC has finished it's conversion
//! @returns the approximate time it took for the ADC function to complete.
uint32_t LTC680x_pollAdc();

/*! Starts cell voltage conversion

  Starts ADC conversions of the LTC6811 Cpin inputs.
  The type of ADC conversion executed can be changed by setting the following parameters:
*/
void LTC680x_adcv(uint8_t MD, //!< ADC Conversion Mode
                  uint8_t DCP, //!< Controls if Discharge is permitted during conversion
                  uint8_t CH //!< Sets which Cell channels are converted
                 );

/*!  Starts cell voltage  and GPIO 1&2 conversion
*/
void LTC680x_adcvax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);


/*!  Starts cell voltage self test conversion
*/
void LTC680x_cvst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Self Test Mode
);

/*!  Starts cell voltage and SOC conversion
*/
void LTC680x_adcvsc(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);
/*!  Starts cell voltage overlap conversion
*/
void LTC680x_adol(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Discharge permitted during conversion
);

/*!  Start an open wire Conversion
*/
void LTC680x_adow(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t PUP //!< Controls if Discharge is permitted during conversion
);


/*!  Start a GPIO and Vref2 Conversion
*/
void LTC680x_adax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an GPIO Redundancy test
*/
void LTC680x_adaxd(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an Auxiliary Register Self Test Conversion
*/
void LTC680x_axst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);



/*!  Start a Status ADC Conversion
*/
void LTC680x_adstat(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHST //!< Sets which Stat channels are converted
);

/*!   Start a Status register redundancy test Conversion
*/
void LTC680x_adstatd(
  uint8_t MD, //!< ADC Mode
  uint8_t CHST //!< Sets which Status channels are converted
);


/*!  Start a Status Register Self Test Conversion
*/
void LTC680x_statst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);

void LTC680x_rdcv_reg(uint8_t reg, //!<Determines which cell voltage register is read back
                      uint8_t total_ic, //!<the number of ICs in the
                      uint8_t *data //!<An array of the unparsed cell codes
                     );
/*! helper function that parses voltage measurement registers
*/
int8_t parse_cells(uint8_t current_ic,
                   uint8_t cell_reg,
                   uint8_t cell_data[],
                   uint16_t *cell_codes,
                   uint8_t *ic_pec);

/*!  Read the raw data from the LTC681x auxiliary register

 The function reads a single GPIO voltage register and stores thre read data
 in the *data point as a byte array. This function is rarely used outside of
 the LTC680x_rdaux() command.
 */
void LTC680x_rdaux_reg(  uint8_t reg, //Determines which GPIO voltage register is read back
                         uint8_t total_ic, //The number of ICs in the system
                         uint8_t *data //Array of the unparsed auxiliary codes
                      );
/*!  Read the raw data from the LTC680x stat register

 The function reads a single GPIO voltage register and stores thre read data
 in the *data point as a byte array. This function is rarely used outside of
 the LTC680x_rdstat() command.
 */
void LTC680x_rdstat_reg(uint8_t reg, //Determines which stat register is read back
                        uint8_t total_ic, //The number of ICs in the system
                        uint8_t *data //Array of the unparsed stat codes
                       );

/*!  Clears the LTC680x cell voltage registers

The command clears the cell voltage registers and initializes
all values to 1. The register will read back hexadecimal 0xFF
after the command is sent.
*/
void LTC680x_clrcell();
/*! Clears the LTC680x Auxiliary registers

The command clears the Auxiliary registers and initializes
all values to 1. The register will read back hexadecimal 0xFF
after the command is sent.
*/
void LTC680x_clraux();

/*!  Clears the LTC680x Stat registers

The command clears the Stat registers and initializes
all values to 1. The register will read back hexadecimal 0xFF
after the command is sent.
*/
void LTC680x_clrstat();

/*!  Clears the LTC680x SCTRL registers

The command clears the SCTRL registers and initializes
all values to 0. The register will read back hexadecimal 0x00
after the command is sent.
*/
void LTC680x_clrsctrl();

/*! Starts the Mux Decoder diagnostic self test

Running this command will start the Mux Decoder Diagnostic Self Test
This test takes roughly 1mS to complete. The MUXFAIL bit will be updated,
the bit will be set to 1 for a failure and 0 if the test has been passed.
*/
void LTC680x_diagn();

/*!  Reads and parses the LTC680x cell voltage registers.

 The function is used to read the cell codes of the LTC6811.
 This function will send the requested read commands parse the data
 and store the cell voltages in the cell_asic structure.
 */
uint8_t LTC680x_rdcv(uint8_t reg, // Controls which cell voltage register is read back.
                     uint8_t total_ic, // the number of ICs in the system
                     cell_asic ic[] // Array of the parsed cell codes
                    );

/*!  Reads and parses the LTC680x auxiliary registers.

 The function is used to read the  parsed GPIO codes of the LTC6811. This function will send the requested
 read commands parse the data and store the gpio voltages in the cell_asic structure.
*/
int8_t LTC680x_rdaux(uint8_t reg, //Determines which GPIO voltage register is read back.
                     uint8_t total_ic,//the number of ICs in the system
                     cell_asic ic[]//!< Measurement Data Structure
                    );

/*!  Reads and parses the LTC680x stat registers.

 The function is used to read the  parsed status codes of the LTC6811. This function will send the requested
 read commands parse the data and store the status voltages in the cell_asic structure
 */
int8_t LTC680x_rdstat(  uint8_t reg, //!<Determines which Stat  register is read back.
                        uint8_t total_ic,//!<the number of ICs in the system
                        cell_asic ic[]//!< Measurement Data Structure
                     );
/*!  Write the LTC680x CFGRA

 This command will write the configuration registers of the LTC680xs
 connected in a daisy chain stack. The configuration is written in descending
 order so the last device's configuration is written first.
 */
void LTC680x_wrcfg(uint8_t total_ic, //The number of ICs being written to
                   cell_asic ic[] //A two dimensional array of the configuration data that will be written
                  );
/*!  Write the LTC680x CFGRB register

 This command will write the configuration registers of the LTC680xs
 connected in a daisy chain stack. The configuration is written in descending
 order so the last device's configuration is written first.
 */
void LTC680x_wrcfgb(uint8_t total_ic, //The number of ICs being written to
                    cell_asic ic[] //A two dimensional array of the configuration data that will be written
                   );
/*!  Reads the LTC680x CFGRA register
*/
int8_t LTC680x_rdcfg(uint8_t total_ic, //Number of ICs in the system
                     cell_asic ic[] //A two dimensional array that the function stores the read configuration data.
                    );

/*!  Reads the LTC680x CFGRB register
*/
int8_t LTC680x_rdcfgb(uint8_t total_ic, //Number of ICs in the system
                      cell_asic ic[] //A two dimensional array that the function stores the read configuration data.
                     );


/*!  Reads pwm registers of a LTC6804 daisy chain
*/
int8_t LTC680x_rdpwm(uint8_t total_ic, //!<Number of ICs in the system
                     uint8_t pwmReg, //!< The PWM Register to be written A or B
                     cell_asic ic[] //!< ASIC Variable
                    );

/*!  Write the LTC680x PWM register

 This command will write the pwm registers of the LTC680x
 connected in a daisy chain stack. The pwm is written in descending
 order so the last device's pwm is written first.
*/
void LTC680x_wrpwm(uint8_t total_ic, //!< The number of ICs being written to
                   uint8_t pwmReg,  //!< The PWM Register to be written
                   cell_asic ic[] //!< ASIC Variable
                  );

/*!  issues a stcomm command and clocks data out of the COMM register */
void LTC680x_stcomm();

/*!  Reads comm registers of a LTC681x daisy chain
*/
int8_t LTC680x_rdcomm(uint8_t total_ic, //!< Number of ICs in the system
                      cell_asic ic[] //!< ASIC Variable
                     );

/*!  Write the LTC680x COMM register

 This command will write the comm registers of the LTC680x
 connected in a daisy chain stack. The comm is written in descending
 order so the last device's configuration is written first.
 */
void LTC680x_wrcomm(uint8_t total_ic, //!< The number of ICs being written to
                    cell_asic ic[] ///!< ASIC Variable
                   );

/*! Selft Test Helper Function*/
uint16_t LTC680x_st_lookup(
  uint8_t MD, //ADC Mode
  uint8_t ST //Self Test
);

/*! Helper Function to clear DCC bits in the CFGR Registers*/
void clear_discharge(uint8_t total_ic,
                     cell_asic ic[]);

/*! Helper function that runs the ADC Self Tests*/
int16_t LTC680x_run_cell_adc_st(uint8_t adc_reg,
                                uint8_t total_ic,
                                cell_asic ic[]);

/*! Helper function that runs the ADC Digital Redudancy commands and checks output for errors*/
int16_t LTC680x_run_adc_redundancy_st(uint8_t adc_mode,
                                      uint8_t adc_reg,
                                      uint8_t total_ic,
                                      cell_asic ic[]);

/*! Helper function that runs the datasheet open wire algorithm*/
void LTC680x_run_openwire(uint8_t total_ic,
                          cell_asic ic[]);

/*! Helper Function that runs the ADC Overlap test*/
uint16_t LTC680x_run_adc_overlap(uint8_t total_ic,
                                 cell_asic ic[]);
/*! Helper Function that counts overall PEC errors and register/IC PEC errors*/
void LTC680x_check_pec(uint8_t total_ic,
                       uint8_t reg,
                       cell_asic ic[]);

/*! Helper Function that resets the PEC error counters */
void LTC680x_reset_crc_count(uint8_t total_ic,
                             cell_asic ic[]);

/*! Helper Function to initialize the CFGR data structures*/
void LTC680x_init_cfg(uint8_t total_ic,
                      cell_asic ic[]);

/*! Helper function to set appropriate bits in CFGR register based on bit function*/
void LTC680x_set_cfgr(uint8_t nIC,
                      cell_asic ic[],
                      bool refon,
                      bool adcopt,
                      bool gpio[5],
                      bool dcc[12]);

/*! Helper function to turn the refon bit HIGH or LOW*/
void LTC680x_set_cfgr_refon(uint8_t nIC,
                            cell_asic ic[],
                            bool refon);

/*! Helper function to turn the ADCOPT bit HIGH or LOW*/
void LTC680x_set_cfgr_adcopt(uint8_t nIC,
                             cell_asic ic[],
                             bool adcopt);

/*! Helper function to turn the GPIO bits HIGH or LOW*/
void LTC680x_set_cfgr_gpio(uint8_t nIC,
                           cell_asic ic[],
                           bool gpio[]);

/*! Helper function to turn the DCC bits HIGH or LOW*/
void LTC680x_set_cfgr_dis(uint8_t nIC,
                          cell_asic ic[],
                          bool dcc[]);
/*!  Helper function to set uv field in CFGRA register*/
void LTC680x_set_cfgr_uv(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t uv);

/*!  Helper function to set ov field in CFGRA register*/
void LTC680x_set_cfgr_ov(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t ov);

//This needs a PROGMEM =  when using with a LINDUINO
extern const uint16_t crc15Table[256];

void LTC6804_init_reg_limits(uint8_t total_ic, cell_asic ic[]);
//extern const uint16_t crc15Table[256];

/*! Starts the Mux Decoder diagnostic self test

 Running this command will start the Mux Decoder Diagnostic Self Test
 This test takes roughly 1mS to complete. The MUXFAIL bit will be updated,
 the bit will be set to 1 for a failure and 0 if the test has been passed.

*/
void LTC6804_diagn();


//! Sends the poll adc command
//! @returns 1 byte read back after a pladc command. If the byte is not 0xFF ADC conversion has completed
uint8_t LTC6804_pladc();


//! This function will block operation until the ADC has finished it's conversion
//! @returns the approximate time it took for the ADC function to complete.
uint32_t LTC6804_pollAdc();

/*! Starts cell voltage conversion
*/
void LTC6804_adcv(uint8_t MD, //!< ADC Conversion Mode
                  uint8_t DCP, //!< Controls if Discharge is permitted during conversion
                  uint8_t CH //!< Sets which Cell channels are converted
                 );

/*!  Starts cell voltage  and GPIO 1&2 conversion
*/
void LTC6804_adcvax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);


/*!  Starts cell voltage self test conversion
*/
void LTC6804_cvst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Self Test Mode
);

/*!  Starts cell voltage and SOC conversion
*/
void LTC6804_adcvsc(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);
/*!  Starts cell voltage overlap conversion
*/
void LTC6804_adol(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Discharge permitted during conversion
);

/*!  Start an open wire Conversion
*/
void LTC6804_adow(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t PUP //!< Controls if Discharge is permitted during conversion
);


/*!  Start a GPIO and Vref2 Conversion
*/
void LTC6804_adax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an GPIO Redundancy test
*/
void LTC6804_adaxd(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an Auxiliary Register Self Test Conversion
*/
void LTC6804_axst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);

/*!  Start a Status ADC Conversion
*/
void LTC6804_adstat(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHST //!< Sets which Stat channels are converted
);

/*!   Start a Status register redundancy test Conversion
*/
void LTC6804_adstatd(
  uint8_t MD, //!< ADC Mode
  uint8_t CHST //!< Sets which Status channels are converted
);


/*!  Start a Status Register Self Test Conversion
*/
void LTC6804_statst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);

/*!  Reads and parses the LTC6811 cell voltage registers.

  @return int8_t, PEC Status.
    0: No PEC error detected
    -1: PEC error detected, retry read
*/
uint8_t LTC6804_rdcv(uint8_t reg, //!< controls which cell voltage register is read back.
                     uint8_t total_ic, //!< the number of ICs in the daisy chain(-1 only)
                     cell_asic ic[] //!< array of the parsed cell codes from lowest to highest.
                    );



/*!  Reads and parses the LTC6811 auxiliary registers.
@return  int8_t, PEC Status
  0: No PEC error detected
 -1: PEC error detected, retry read
*/
int8_t LTC6804_rdaux(uint8_t reg,        //!< controls which GPIO voltage register is read back
                     uint8_t nIC,        //!< the number of ICs in the daisy chain
                     cell_asic ic[] //!< A two dimensional array of the parsed gpio voltage codes
                    );

/*!  Reads and parses the LTC6811 stat registers.

@return  int8_t, PEC Status
  0: No PEC error detected
 -1: PEC error detected, retry read
*/
int8_t LTC6804_rdstat(uint8_t reg, //Determines which Stat  register is read back.
                      uint8_t total_ic,//the number of ICs in the system
                      cell_asic ic[]
                     );

/*!  Clears the LTC6804 cell voltage registers
*/
void LTC6804_clrcell();

/*! Clears the LTC6804 Auxiliary registers
*/
void LTC6804_clraux();

/*!  Clears the LTC6804 Stat registers
*/
void LTC6804_clrstat();

/*!  Clears the LTC6804 Sctrl registers
*/
void LTC6804_clrsctrl();

/*!  Write the LTC6804 configuration register
*/
void LTC6804_wrcfg(uint8_t nIC, //!< The number of ICs being written
                   cell_asic ic[] //!< a two dimensional array of the configuration data that will be written
                  );

void LTC6804_wrcfgb(uint8_t nIC, //!< The number of ICs being written
                    cell_asic ic[] //!< a two dimensional array of the configuration data that will be written
                   );
/*!  Reads configuration registers of a LTC6804 daisy chain
@return int8_t, PEC Status.
  0: Data read back has matching PEC
   -1: Data read back has incorrect PEC
*/
int8_t LTC6804_rdcfg(uint8_t nIC, //!< number of ICs in the daisy chain
                     cell_asic ic[] //!< a two dimensional array that the function stores the read configuration data
                    );

int8_t LTC6804_rdcfgb(uint8_t nIC, //!< number of ICs in the daisy chain
                      cell_asic ic[] //!< a two dimensional array that the function stores the read configuration data
                     );
/*!  Write the LTC6804 PWM register
*/
void LTC6804_wrpwm(uint8_t nIC, //!< number of ICs in the daisy chain
                   uint8_t pwmReg,
                   cell_asic ic[]
                  );

/*!  Reads pwm registers of a LTC6811 daisy chain

*/
int8_t LTC6804_rdpwm(uint8_t nIC, //!< number of ICs in the daisy chain
                     uint8_t pwmReg,
                     cell_asic ic[] //!< a two dimensional array that the function stores the read pwm data
                    );

/*!  Write the LTC6804 Sctrl register
*/
void LTC6804_wrsctrl(uint8_t nIC, //!< number of ICs in the daisy chain
                     uint8_t sctrl_reg,
                     cell_asic ic[]
                    );


/*!  Reads sctrl registers of a LTC6804 daisy chain
@return int8_t, PEC Status.
  0: Data read back has matching PEC
  -1: Data read back has incorrect PEC
*/
int8_t LTC6804_rdsctrl(uint8_t nIC, //!< number of ICs in the daisy chain
                       uint8_t sctrl_reg,
                       cell_asic ic[] //!< a two dimensional array that the function stores the read pwm data
                      );


/*!  Start Sctrl data communication
This command will start the sctrl pulse communication over the spins
*/
void LTC6804_stsctrl();


/*!  Write the LTC6804 COMM register
*/
void LTC6804_wrcomm(uint8_t total_ic, //!< Number of ICs in the daisy chain
                    cell_asic ic[] //!< A two dimensional array of the comm data that will be written
                   );

/*!  Reads comm registers of a LTC6804 daisy chain
@return int8_t, PEC Status.

  0: Data read back has matching PEC

  -1: Data read back has incorrect PEC

*/
int8_t LTC6804_rdcomm(uint8_t total_ic, //!< number of ICs in the daisy chain
                      cell_asic ic[] //!< Two dimensional array that the function stores the read comm data.
                     );

/*!  issues a stcomm command and clocks data out of the COMM register */
void LTC6804_stcomm();


/*! @returns returns the register data pattern for a given ADC MD and Self test */
uint16_t LTC6804_st_lookup(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Self test number
);

void LTC6804_set_discharge(int Cell,
                           uint8_t total_ic,
                           cell_asic ic[]);

/*! Helper function that runs the ADC Self Tests*/
int16_t LTC6804_run_cell_adc_st(uint8_t adc_reg,
                                uint8_t total_ic,
                                cell_asic ic[]);

/*! Helper function that runs the ADC Digital Redudancy commands and checks output for errors*/
int16_t LTC6804_run_adc_redundancy_st(uint8_t adc_mode,
                                      uint8_t adc_reg,
                                      uint8_t total_ic,
                                      cell_asic ic[]);

/*! Helper function that runs the datasheet open wire algorithm*/
void LTC6804_run_openwire(uint8_t total_ic,
                          cell_asic ic[]);

void LTC6804_set_discharge(int Cell,
                           uint8_t total_ic,
                           cell_asic ic[]);

/*! Helper Function that runs the ADC Overlap test*/
uint16_t LTC6804_run_adc_overlap(uint8_t total_ic,
                                 cell_asic ic[]);

/*! Helper Function that counts overall PEC errors and register/IC PEC errors*/
void LTC6804_check_pec(uint8_t total_ic,
                       uint8_t reg,
                       cell_asic ic[]);

/*! Helper Function that resets the PEC error counters */
void LTC6804_reset_crc_count(uint8_t total_ic,
                             cell_asic ic[]);

/*! Helper Function to initialize the CFGR data structures*/
void LTC6804_init_cfg(uint8_t total_ic,
                      cell_asic ic[]);

/*! Helper function to set appropriate bits in CFGR register based on bit function*/
void LTC6804_set_cfgr(uint8_t nIC,
                      cell_asic ic[],
                      bool refon,
                      bool adcopt,
                      bool gpio[5],
                      bool dcc[12]);

/*! Helper function to turn the refon bit HIGH or LOW*/
void LTC6804_set_cfgr_refon(uint8_t nIC,
                            cell_asic ic[],
                            bool refon);

/*! Helper function to turn the ADCOPT bit HIGH or LOW*/
void LTC6804_set_cfgr_adcopt(uint8_t nIC,
                             cell_asic ic[],
                             bool adcopt);

/*! Helper function to turn the GPIO bits HIGH or LOW*/
void LTC6804_set_cfgr_gpio(uint8_t nIC,
                           cell_asic ic[],
													 bool gpio[]);

/*! Helper function to turn the DCC bits HIGH or LOW*/
void LTC6804_set_cfgr_dis(uint8_t nIC,
                          cell_asic ic[],
                          bool dcc[]);
/*!  Helper function to set uv field in CFGRA register*/
void LTC6804_set_cfgr_uv(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t uv);

/*!  Helper function to set ov field in CFGRA register*/
void LTC6804_set_cfgr_ov(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t ov);

void LTC6804_init_max_min(uint8_t total_ic,
                          cell_asic ic[],
                          cell_asic ic_max[],
                          cell_asic ic_min[]);

void LTC6804_max_min(uint8_t total_ic, cell_asic ic_cells[],
                     cell_asic ic_min[],
                     cell_asic ic_max[],
                     cell_asic ic_delta[]);

#endif /* INC_LTC6804_H_ */
