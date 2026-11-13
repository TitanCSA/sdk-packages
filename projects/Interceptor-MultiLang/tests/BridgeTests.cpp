#include "InterceptorBridge.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <thread>
#include <windows.h>
extern "C" __declspec(dllimport) void icb_test_mode(unsigned);
extern "C" __declspec(dllimport) long long icb_test_seen(unsigned, unsigned);
void Check(bool value) {
    if (!value)
        throw std::runtime_error("Bridge contract failed");
}
int main() {
    try {
        icb_error e{};
        uint64_t s = 0, other = 0;
        Check(icb_version() == 1 && icb_sizeof(1) == sizeof(icb_field));
        Check(!icb_open(&s, &e));
        Check(icb_open(&other, &e) == ERROR_BUSY && !other);
        icb_init init{};
        Check(!icb_initialize(s, &init, &e) && init.ready);
        icb_info info{};
        Check(!icb_info_get(s, &info, &e) && info.installed);
        const icb_devices *devices = nullptr;
        Check(!icb_devices_get(s, &devices, &e) && devices->count == 6);
        Check(std::strstr(devices->items[0].name, "\xc3\xa9"));
        const icb_batch *b = nullptr;
        Check(!icb_receive(s, 0, &b, &e) && b->count == 6);
        auto ticket = b->id;
        const icb_batch *unused = nullptr;
        Check(icb_receive(s, 0, &unused, &e) == ERROR_BUSY && !unused);
        for (unsigned n = 0; n < b->count; ++n) {
            auto &event = b->events[n];
            for (unsigned f = 0; f < event.field_count; ++f) {
                auto &v = event.fields[f];
                if (event.kind == 1 && !std::strcmp(v.name, "scan_code"))
                    v.value = 0x30;
                if (event.kind == 2 && !std::strcmp(v.name, "y"))
                    v.value = -7;
                if ((event.kind == 3 || event.kind == 5) && v.group == ICB_AXIS && v.index == 0) {
                    v.value = 50;
                    v.normalized = .5;
                }
                if (event.kind == 4 && !std::strcmp(v.name, "left_x"))
                    v.value = -100;
            }
        }
        b->events[5].blocked = 1;
        Check(!icb_send(s, ticket, &e) && e.accepted == 5);
        Check(icb_test_seen(1, 1) == 0x30 && icb_test_seen(2, 1) == -7 &&
              icb_test_seen(3, 1) == 50 && icb_test_seen(3, 2) == 500 &&
              icb_test_seen(4, 1) == -100 && icb_test_seen(6, 0) == 0);
        Check(!icb_send(s, ticket, &e) && e.accepted == 0);
        Check(!icb_receive(s, 0, &b, &e));
        Check(icb_send(s, ticket, &e) == ERROR_INVALID_HANDLE);
        ticket = b->id;
        icb_test_mode(2);
        Check(icb_send(s, ticket, &e) == ERROR_BUSY && e.accepted == 1);
        Check(!icb_send(s, ticket, &e) && e.accepted == 5);
        icb_test_mode(0);
        Check(!icb_receive(s, 0, &b, &e));
        for (unsigned f = 0; f < b->events[1].field_count; ++f)
            if (!std::strcmp(b->events[1].fields[f].name, "y"))
                b->events[1].fields[f].value = INT64_MAX;
        Check(icb_send(s, b->id, &e) == ERROR_INVALID_PARAMETER);
        Check(!icb_discard(s, b->id, &e));
        icb_blob blob{};
        Check(!icb_descriptor(s, "fixture:4", 1, &blob, &e) && !blob.applicable);
        Check(!icb_descriptor(s, "fixture:3", 1, &blob, &e) && blob.length == 4);
        icb_field x{};
        x.name = "x";
        x.value = 200;
        icb_event injection{};
        injection.kind = 2;
        injection.fields = &x;
        injection.field_count = 1;
        Check(!icb_inject(s, &injection, 1, &e) && e.accepted == 1);
        icb_test_mode(1);
        uint32_t result = 999;
        std::thread reader([&] {
            icb_error err{};
            const icb_batch *out = nullptr;
            result = icb_receive(s, 5000, &out, &err);
            Check(!result && out && !out->count);
        });
        Sleep(30);
        Check(!icb_cancel(s, &e));
        reader.join();
        std::thread closingReader([&] {
            icb_error err{};
            const icb_batch *out = nullptr;
            icb_receive(s, 5000, &out, &err);
        });
        Sleep(30);
        Check(!icb_close(s, &e));
        closingReader.join();
        Check(icb_close(s, &e) == ERROR_INVALID_HANDLE);
        icb_test_mode(0);
        for (unsigned i = 0; i < 200; ++i) {
            Check(!icb_open(&s, &e));
            Check(!icb_receive(s, 0, &b, &e));
            Check(!icb_send(s, b->id, &e));
            Check(!icb_close(s, &e));
        }
        std::cout << "PASS ABI, all families, edits, metadata, block, retries, stale tickets, "
                     "injection, cancellation, concurrent close and 200 lifecycle cycles\n";
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
