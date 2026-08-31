/******************************************************************************
 * File Name:   self_test.c
 *
 * Description: Provides DUT SPI (SCB1) initialization and the Class-B SPI
 *              loopback self-test wrapper.
 *
 *              The SCB hardware loopback (SCB_SPI_CTRL.LOOPBACK bit, same bit
 *              position as the UART loopback bit) is enabled once at init,
 *              routing MOSI back to MISO internally. No external wiring or
 *              SmartIO is required.
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * $ Copyright 2026 Infineon Technologies AG $
 *******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_retarget_io.h"
#include "self_test.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Delay after each test call to let any in-flight bytes fully drain before
 * the next iteration. */
#define BUFF_CLEAR_DELAY_US  (100u)

/*******************************************************************************
* Function Name: spi_self_test_init
********************************************************************************
* Summary:
*  Initializes the DUT SPI peripheral (SCB1), enables hardware internal loopback,
*  and clears both FIFOs.  Call once before entering the test loop.
*
* Parameters:
*  void
*
* Return:
*  void
*******************************************************************************/
void spi_self_test_init(void)
{
    cy_en_scb_spi_status_t init_status;

    /* NULL context — no interrupt-driven transfers, polling mode only */
    init_status = Cy_SCB_SPI_Init(CYBSP_DUT_SPI_HW, &CYBSP_DUT_SPI_config, NULL);
    if (init_status != CY_SCB_SPI_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SCB_SPI_SetActiveSlaveSelect(CYBSP_DUT_SPI_HW, CY_SCB_SPI_SLAVE_SELECT0);
    Cy_SCB_SPI_Enable(CYBSP_DUT_SPI_HW);

    /* Enable hardware internal loopback: MOSI is routed back to MISO inside
     * the SCB block.  No physical SPI pins are needed. */
    SCB_SPI_CTRL(CYBSP_DUT_SPI_HW) |= SCB_SPI_CTRL_LOOPBACK_Msk;

    Cy_SCB_SPI_ClearRxFifo(CYBSP_DUT_SPI_HW);
    Cy_SCB_SPI_ClearTxFifo(CYBSP_DUT_SPI_HW);

    printf("DUT SPI (SCB1) initialized - hardware loopback test ready.\r\n\n");
}

/*******************************************************************************
* Function Name: spi_self_test
********************************************************************************
* Summary:
*  Runs one iteration of the SPI loopback self-test.
*
*  Hardware loopback is permanently enabled (set in spi_self_test_init), so
*  each call directly runs SelfTest_SPI_SCB() without toggling any external MUX.
*  The caller repeats this function while PASS_STILL_TESTING_STATUS is returned.
*
* Parameters:
*  void
*
* Return:
*  PASS_STILL_TESTING_STATUS — test in progress, call again
*  PASS_COMPLETE_STATUS      — all 256 bytes verified, test passed
*  ERROR_STATUS or other     — test failure (see SelfTest_common.h)
*******************************************************************************/
uint8_t spi_self_test(void)
{
    uint8_t ret;

    /* Clear FIFOs before each test call */
    Cy_SCB_SPI_ClearRxFifo(CYBSP_DUT_SPI_HW);
    Cy_SCB_SPI_ClearTxFifo(CYBSP_DUT_SPI_HW);

    ret = SelfTest_SPI_SCB(CYBSP_DUT_SPI_HW);

    /* Short delay so any in-flight bytes fully drain from the FIFO */
    Cy_SysLib_DelayUs(BUFF_CLEAR_DELAY_US);
    Cy_SCB_SPI_ClearRxFifo(CYBSP_DUT_SPI_HW);
    Cy_SCB_SPI_ClearTxFifo(CYBSP_DUT_SPI_HW);

    return ret;
}

/* [] END OF FILE */
