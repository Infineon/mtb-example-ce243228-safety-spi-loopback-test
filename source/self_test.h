/******************************************************************************
 * File Name:   self_test.h
 *
 * Description: This file is the public interface of self_test.c source file.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * $ Copyright 2026 Infineon Technologies AG $
 *******************************************************************************/

#ifndef SELF_TEST_H_
#define SELF_TEST_H_

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "cy_pdl.h"
#include "SelfTest.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* State used by main() after the SPI self-test passes to run the user
 * application.  Must differ from all PASS/ERROR codes in SelfTest_common.h
 * (values 1–6). */
#define EXE_CUSTOM_APP  (7u)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void    spi_self_test_init(void);
uint8_t spi_self_test(void);

#endif /* SELF_TEST_H_ */

/* [] END OF FILE */
