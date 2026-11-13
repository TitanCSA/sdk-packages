#ifndef FYRAINPUT_GAMEPAD_BRIDGE_H
#define FYRAINPUT_GAMEPAD_BRIDGE_H
#include <stdint.h>
#ifdef IC_BRIDGE_EXPORTS
#define FYRAINPUT_GAMEPAD_API __declspec(dllexport)
#else
#define FYRAINPUT_GAMEPAD_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
/* Windows x64, cdecl, natural alignment. Never free borrowed pointers.
   Receive views remain valid until the next receive or close. Copy data before
   another thread calls either. Every function catches C++ exceptions. */
enum {
    FYRAINPUT_GAMEPAD_KEYBOARD = 1,
    FYRAINPUT_GAMEPAD_MOUSE = 2,
    FYRAINPUT_GAMEPAD_HID = 3,
    FYRAINPUT_GAMEPAD_XINPUT = 4,
    FYRAINPUT_GAMEPAD_XINPUTHID = 5,
    FYRAINPUT_GAMEPAD_GIP = 6
};
enum {
    FYRAINPUT_GAMEPAD_STATE = 0,
    FYRAINPUT_GAMEPAD_AXIS = 1,
    FYRAINPUT_GAMEPAD_TRIGGER = 2,
    FYRAINPUT_GAMEPAD_HAT = 3,
    FYRAINPUT_GAMEPAD_VALUE = 4,
    FYRAINPUT_GAMEPAD_BUTTON = 5,
    FYRAINPUT_GAMEPAD_BINARY = 6,
    FYRAINPUT_GAMEPAD_REPORT = 7
};
enum {
    FYRAINPUT_GAMEPAD_NORMALIZED = 1,
    FYRAINPUT_GAMEPAD_RELATIVE = 2,
    FYRAINPUT_GAMEPAD_HAS_NULL = 4,
    FYRAINPUT_GAMEPAD_IS_NULL = 8,
    FYRAINPUT_GAMEPAD_IS_TRIGGER = 16,
    FYRAINPUT_GAMEPAD_READ_ONLY = 32
};
#pragma pack(push, 8)
typedef struct fyrainput_gamepad_error {
    uint32_t code, accepted;
    char message[768];
} fyrainput_gamepad_error;
typedef struct fyrainput_gamepad_field {
    const char *name;
    int64_t value, minimum, maximum, physical_minimum, physical_maximum;
    double normalized;
    uint32_t group, index, usage_page, usage, collection, report_id, data_index, element;
    uint32_t flags, units, units_exponent, bit_size;
    uint8_t *data;
    uint32_t length, reserved;
} fyrainput_gamepad_field;
typedef struct fyrainput_gamepad_event {
    uint32_t kind, blocked;
    uint64_t sequence, timestamp;
    const char *token;
    fyrainput_gamepad_field *fields;
    uint32_t field_count, decode_error, decode_complete, parser_status, device, reserved;
} fyrainput_gamepad_event;
typedef struct fyrainput_gamepad_batch {
    uint64_t id;
    uint32_t count, reserved;
    fyrainput_gamepad_event *events;
} fyrainput_gamepad_batch;
typedef struct fyrainput_gamepad_device {
    uint32_t kind, id;
    uint16_t vid, pid, usage_page, usage;
    const char *token;
    const char *name;
    const char *hardware_id;
} fyrainput_gamepad_device;
typedef struct fyrainput_gamepad_devices {
    uint32_t count, reserved;
    const fyrainput_gamepad_device *items;
} fyrainput_gamepad_devices;
typedef struct fyrainput_gamepad_blob {
    uint32_t applicable, format, length, reserved;
    const uint8_t *data;
} fyrainput_gamepad_blob;
typedef struct fyrainput_gamepad_init {
    uint32_t ready, reboot_required, associated, already_associated, skipped, reserved;
} fyrainput_gamepad_init;
typedef struct fyrainput_gamepad_info {
    uint32_t installed, active, trial_state, input_mask, controller_mask, device_count, queued,
        last_reset;
    uint64_t remaining_ms, timestamp_frequency, generation;
} fyrainput_gamepad_info;
#pragma pack(pop)
FYRAINPUT_GAMEPAD_API const char* fyrainput_gamepad_product_name(void);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_input_mask(void);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_controller_mask(void);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_version(void);
/* sizeof: 1 field, 2 event, 3 batch, 4 device, 5 error, 6 info. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_sizeof(uint32_t type);
/* Open is lazy: no driver capture, installation, timer or worker thread. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_open(uint64_t *session, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_close(uint64_t session, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_initialize(uint64_t session, fyrainput_gamepad_init *result, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_info_get(uint64_t session, fyrainput_gamepad_info *result, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_devices_get(uint64_t session, const fyrainput_gamepad_devices **result, fyrainput_gamepad_error *error);
/* timeout in milliseconds, UINT32_MAX means infinite. Cancel returns empty. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_receive(uint64_t session, uint32_t timeout, const fyrainput_gamepad_batch **result,
                             fyrainput_gamepad_error *error);
/* Counts inferred from ticket. Retry forwards only the unaccepted remainder. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_send(uint64_t session, uint64_t ticket, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_discard(uint64_t session, uint64_t ticket, fyrainput_gamepad_error *error);
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_cancel(uint64_t session, fyrainput_gamepad_error *error);
/* Native HID report: report=1; common descriptor: report=0. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_descriptor(uint64_t session, const char *token, uint32_t report,
                                fyrainput_gamepad_blob *result, fyrainput_gamepad_error *error);
/* Synthetic input: kind and capture token select an existing source. Named
   STATE fields plus REPORT bytes use the same representation as receive.
   Input and field counts belong only to this low-level FFI; wrappers infer them. */
FYRAINPUT_GAMEPAD_API uint32_t fyrainput_gamepad_inject(uint64_t session, const fyrainput_gamepad_event *inputs, uint32_t count,
                            fyrainput_gamepad_error *error);
#ifdef __cplusplus
}
static_assert(sizeof(fyrainput_gamepad_field) == 120 && sizeof(fyrainput_gamepad_event) == 64 && sizeof(fyrainput_gamepad_batch) == 24,
              "C ABI layout");
static_assert(sizeof(fyrainput_gamepad_device) == 40 && sizeof(fyrainput_gamepad_error) == 776 && sizeof(fyrainput_gamepad_info) == 56,
              "C ABI layout");
#endif
#endif
