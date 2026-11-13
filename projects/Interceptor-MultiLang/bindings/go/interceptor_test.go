package interceptor

import (
	"os"
	"testing"
)

func TestRoundTrip(t *testing.T) {
	if os.Getenv("INTERCEPTOR_TEST_BACKEND") != "1" {
		t.Skip("Use Build.ps1 -Test; no implicit physical input capture")
	}
	io, e := New()
	if e != nil {
		t.Fatal(e)
	}
	defer io.Close()
	devices, e := io.Devices()
	if e != nil || len(devices) != 6 {
		t.Fatalf("devices: %v", e)
	}
	batch, e := io.Receive(0)
	if e != nil {
		t.Fatal(e)
	}
	if len(batch.Inputs) != 6 || batch.Inputs[0].Sequence != 0x8000000000000001 {
		t.Fatal("ABI or uint64 mismatch")
	}
	batch.Inputs[1].State["y"] = -7
	batch.Inputs[2].HID.Axes[0].Value = 50
	batch.Inputs[2].HID.Axes[0].Normalized = .5
	batch.Inputs[5].Blocked = true
	if n, e := io.Send(batch); e != nil || n != 5 {
		t.Fatalf("send %d: %v", n, e)
	}
	if n, e := io.Send(batch); e != nil || n != 0 {
		t.Fatal("replayed batch")
	}
	if n, e := io.Inject("mouse", map[string]int64{"x": 200}); e != nil || n != 1 {
		t.Fatal(e)
	}
	d, e := io.Descriptor("fixture:4", true)
	if e != nil || d.Applicable {
		t.Fatal("descriptor")
	}
	next, e := io.Receive(0)
	if e != nil {
		t.Fatal(e)
	}
	if _, e = io.Send(batch); e == nil {
		t.Fatal("stale batch accepted")
	}
	if e = io.Discard(next); e != nil {
		t.Fatal(e)
	}
}
