#include "playSingleNote.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/sys/ring_buffer.h>
#include "include/ble_midi/ble_midi.h"
#include <stdio.h>
#include "dataType.h"

/************************ App state ************************/
K_MSGQ_DEFINE(button_event_q, sizeof(uint8_t), 128, 4);

#define SYSEX_TX_MESSAGE_SIZE 2000
#define SYSEX_TX_MAX_CHUNK_SIZE 240

struct sample_app_state_t
{
        int ble_midi_is_available;
        int sysex_rx_data_byte_count;
        int sysex_tx_data_byte_count;
        int sysex_tx_in_progress;
        int button_states[4];
};

struct sample_app_state_t sample_app_state = {.ble_midi_is_available = 0,
                                                     .sysex_tx_data_byte_count = 0,
                                                     .sysex_tx_in_progress = 0,
                                                     .sysex_rx_data_byte_count = 0,
                                                     .button_states = {0, 0, 0, 0}};

/* Ringbuf/work queue */
RING_BUF_DECLARE(midi_msg_ringbuf, 128);
static void midi_msg_work_cb(struct k_work *w)
{
        uint8_t data[4] = {0, 0, 0, 0};
        while (ring_buf_get(&midi_msg_ringbuf, &data, 4) == 4)
        {
                uint8_t msg_byte_count = data[0];
                uint8_t *msg_bytes = &data[1];
                printk("incoming MIDI message ");
                for (int i = 0; i < msg_byte_count; i++)
                {
                        printk("%02x ", msg_bytes[i]);
                }
                printk("\n");
        };
}
static K_WORK_DEFINE(midi_msg_work, midi_msg_work_cb);

/****************** BLE MIDI callbacks ******************/
static void ble_midi_available_cb(int is_available)
{
        printk("ble_midi_is_available : %d", is_available);
        sample_app_state.ble_midi_is_available = is_available;
}

static void tx_done_cb()
{
        if (sample_app_state.sysex_tx_in_progress)
        {
                if (sample_app_state.sysex_tx_data_byte_count == SYSEX_TX_MESSAGE_SIZE)
                {
                        ble_midi_tx_sysex_end();
                        sample_app_state.sysex_tx_in_progress = 0;
                }
                else
                {
                        uint8_t chunk[SYSEX_TX_MAX_CHUNK_SIZE];
                        for (int i = 0; i < SYSEX_TX_MAX_CHUNK_SIZE; i++)
                        {
                                chunk[i] = (sample_app_state.sysex_tx_data_byte_count + i) % 128;
                        }
                        int num_bytes_left =
                            SYSEX_TX_MESSAGE_SIZE - sample_app_state.sysex_tx_data_byte_count;
                        int num_bytes_to_send = num_bytes_left < SYSEX_TX_MAX_CHUNK_SIZE
                                                    ? num_bytes_left
                                                    : SYSEX_TX_MAX_CHUNK_SIZE;
                        int num_bytes_sent = ble_midi_tx_sysex_data(chunk, num_bytes_to_send);
                        sample_app_state.sysex_tx_data_byte_count += num_bytes_sent;
                }
        }
}

/** Called when a non-sysex message has been parsed */
static void ble_midi_message_cb(uint8_t *bytes, uint8_t num_bytes, uint16_t timestamp)
{
        uint8_t data[4] = {num_bytes, 0, 0, 0};
        for (int i = 0; i < num_bytes; i++)
        {
                data[i + 1] = bytes[i];
        }
        uint32_t num_bytes_written = ring_buf_put(&midi_msg_ringbuf, &data, 4);
        __ASSERT(num_bytes_written == 4, "Failed to write to MIDI msg ringbuf");
        int submit_rc = k_work_submit(&midi_msg_work);
}

/** Called when a sysex message starts */
static void ble_midi_sysex_start_cb(uint16_t timestamp)
{
        printk("rx sysex start, t %d\n", timestamp);
}
/** Called when a sysex data byte has been received */
static void ble_midi_sysex_data_cb(uint8_t data_byte)
{
        printk("rx sysex byte %02x\n", data_byte);
}
/** Called when a sysex message ends */
static void ble_midi_sysex_end_cb(uint16_t timestamp)
{
        printk("rx sysex end, t %d\n", timestamp);
}

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BLE_MIDI_SERVICE_UUID),
};


int isConnectedOverBLE(void){
        return sample_app_state.ble_midi_is_available;
}


void midiInitialize(void){

    uint32_t err = bt_enable(NULL);
    __ASSERT(err == 0, "bt_enable failed");

    /* Must be called after bt_enable */
        
    struct ble_midi_callbacks midi_callbacks = {.available_cb = ble_midi_available_cb,
                                                    .tx_done_cb = tx_done_cb,
                                                    .midi_message_cb = ble_midi_message_cb,
                                                    .sysex_start_cb = ble_midi_sysex_start_cb,
                                                    .sysex_data_cb = ble_midi_sysex_data_cb,
                                                    .sysex_end_cb = ble_midi_sysex_end_cb};
    ble_midi_init(&midi_callbacks);

    int ad_err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    printk("bt_le_adv_start %d\n", ad_err);
}


void sendNote(bool on_off, int note, int force){
    if (!sample_app_state.sysex_tx_in_progress)
    {
        printk("creating note to play\n");
        uint8_t toPlay[3] = {on_off ? 0x90 : 0x80, note, force};
        printk("going to play note number %d with on = %d\n", note, on_off);
        ble_midi_tx_msg(toPlay);
        printk("played my note\n");
    }
}
