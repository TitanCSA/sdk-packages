typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
enum {
    ICB_KEYBOARD = 1,
    ICB_MOUSE = 2,
    ICB_HID = 3,
    ICB_XINPUT = 4,
    ICB_XINPUTHID = 5,
    ICB_GIP = 6
};
enum {
    ICB_STATE = 0,
    ICB_AXIS = 1,
    ICB_TRIGGER = 2,
    ICB_HAT = 3,
    ICB_VALUE = 4,
    ICB_BUTTON = 5,
    ICB_BINARY = 6,
    ICB_REPORT = 7
};
enum {
    ICB_NORMALIZED = 1,
    ICB_RELATIVE = 2,
    ICB_HAS_NULL = 4,
    ICB_IS_NULL = 8,
    ICB_IS_TRIGGER = 16,
    ICB_READ_ONLY = 32
};

typedef struct icb_error {
    uint32_t code, accepted;
    char message[768];
} icb_error;
typedef struct icb_field {
    const char *name;
    int64_t value, minimum, maximum, physical_minimum, physical_maximum;
    double normalized;
    uint32_t group, index, usage_page, usage, collection, report_id, data_index, element;
    uint32_t flags, units, units_exponent, bit_size;
    uint8_t *data;
    uint32_t length, reserved;
} icb_field;
typedef struct icb_event {
    uint32_t kind, blocked;
    uint64_t sequence, timestamp;
    const char *token;
    icb_field *fields;
    uint32_t field_count, decode_error, decode_complete, parser_status, device, reserved;
} icb_event;
typedef struct icb_batch {
    uint64_t id;
    uint32_t count, reserved;
    icb_event *events;
} icb_batch;
typedef struct icb_device {
    uint32_t kind, id;
    uint16_t vid, pid, usage_page, usage;
    const char *token;
    const char *name;
    const char *hardware_id;
} icb_device;
typedef struct icb_devices {
    uint32_t count, reserved;
    const icb_device *items;
} icb_devices;
typedef struct icb_blob {
    uint32_t applicable, format, length, reserved;
    const uint8_t *data;
} icb_blob;
typedef struct icb_init {
    uint32_t ready, reboot_required, associated, already_associated, skipped, reserved;
} icb_init;
typedef struct icb_info {
    uint32_t installed, active, trial_state, input_mask, controller_mask, device_count, queued,
        last_reset;
    uint64_t remaining_ms, timestamp_frequency, generation;
} icb_info;
uint32_t icb_version(void);
/* sizeof: 1 field, 2 event, 3 batch, 4 device, 5 error, 6 info. */
uint32_t icb_sizeof(uint32_t type);
/* Open is lazy: no driver capture, installation, timer or worker thread. */
uint32_t icb_open(uint64_t *session, icb_error *error);
uint32_t icb_close(uint64_t session, icb_error *error);
uint32_t icb_initialize(uint64_t session, icb_init *result, icb_error *error);
uint32_t icb_info_get(uint64_t session, icb_info *result, icb_error *error);
uint32_t icb_devices_get(uint64_t session, const icb_devices **result, icb_error *error);
/* timeout in milliseconds, UINT32_MAX means infinite. Cancel returns empty. */
uint32_t icb_receive(uint64_t session, uint32_t timeout, const icb_batch **result,
                             icb_error *error);
/* Counts inferred from ticket. Retry forwards only the unaccepted remainder. */
uint32_t icb_send(uint64_t session, uint64_t ticket, icb_error *error);
uint32_t icb_discard(uint64_t session, uint64_t ticket, icb_error *error);
uint32_t icb_cancel(uint64_t session, icb_error *error);
/* Native HID report: report=1; common descriptor: report=0. */
uint32_t icb_descriptor(uint64_t session, const char *token, uint32_t report,
                                icb_blob *result, icb_error *error);
/* Synthetic input: kind and capture token select an existing source. Named
   STATE fields plus REPORT bytes use the same representation as receive.
   Input and field counts belong only to this low-level FFI; wrappers infer them. */
uint32_t icb_inject(uint64_t session, const icb_event *inputs, uint32_t count,
                            icb_error *error);
