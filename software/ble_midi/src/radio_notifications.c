/*
 * MIT License
 * 
 * Copyright (c) 2024 Paul MIALANE, Lucas MICHEL, Justine OGER
 * 
 * This file includes work that is based on or derived from code by Per Gantelius.
 * 
 * Original Copyright (c) 2023 Per Gantelius
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <mpsl_radio_notification.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(ble_midi, CONFIG_BLE_MIDI_LOG_LEVEL);

#include "radio_notifications.h"


#define RADIO_NOTIF_PRIORITY 1

static radio_notification_cb_t user_callback = NULL;

/* Called just before each BLE connection event. */
static void radio_notif_handler(void)
{
    if (user_callback)
    {
        user_callback();
    }
}

int radio_notifications_init(radio_notification_cb_t callback) {    
    user_callback = callback;
	int rc = mpsl_radio_notification_cfg_set(MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_ACTIVE,
						 MPSL_RADIO_NOTIFICATION_DISTANCE_420US, TEMP_IRQn);
	if (rc == 0) {
		IRQ_CONNECT(TEMP_IRQn, RADIO_NOTIF_PRIORITY, radio_notif_handler, NULL, 0);
		LOG_INF("Finished setting up connection event interrupt");
	} else {
		LOG_ERR("mpsl_radio_notification_cfg_set failed with error %d", rc);
	}

	__ASSERT(rc == 0, "mpsl_radio_notification_cfg_set failed");

}

void set_radio_notifications_enabled(int enabled) {
    if (enabled) {
        irq_enable(TEMP_IRQn);
    } else {
        irq_disable(TEMP_IRQn);
    }
}