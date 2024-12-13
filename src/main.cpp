#include <stdio.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LpCoreVanTx.hpp"

#define VAN_RX_PIN GPIO_NUM_3
#define VAN_TX_PIN GPIO_NUM_2

LpCoreVanTx* vanTx;

extern "C" void app_main(void)
{

    /* If user is using USB-serial-jtag then idf monitor needs some time to
    *  re-connect to the USB port. We wait 1 sec here to allow for it to make the reconnection
    *  before we print anything. Otherwise the chip will go back to sleep again before the user
    *  has time to monitor any output.
    */
    vTaskDelay(pdMS_TO_TICKS(1000));

    vanTx = new LpCoreVanTx(VAN_RX_PIN, VAN_TX_PIN, LpCoreVanTx::LP_VAN_NETWORK_SPEED::LP_VAN_125KBPS);
    vanTx->Start();

    uint8_t packet[7] = { 0x0F, 0x07, 0x81, 0x1D, 0xA4 ,0x93, -7 * 2 + 0x50 };

    do
    {
        printf("Sending VAN DATA\r\n");
        if (vanTx->IsTxPossible())
        {
            vanTx->SendNormalFrame(0x8A4, packet, 7, false);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    } while (1);
}
