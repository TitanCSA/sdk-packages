<?php
declare(strict_types=1);

namespace TitanCSA\Products\WindowsGamepadFilter;

use FFI;
use FFI\CData;

final class NativeException extends \RuntimeException {
    public function __construct(int $code, string $message, public readonly int $accepted = 0) { parent::__construct($message, $code); }
}

final class Field {
    public function __construct(public int $value, public float $normalized, public string $data, public readonly array $metadata) {}
}

final class Input {
    public bool $blocked = false;
    /** @param list<Field> $fields */
    public function __construct(public readonly string $kind, public readonly string $token, public readonly string $sequence,
        public readonly string $timestamp, public readonly array $fields, public readonly array $metadata) {}
    public function field(string $name): Field {
        $matches = array_values(array_filter($this->fields, fn(Field $f) => $f->metadata['name'] === $name));
        if (count($matches) !== 1) throw new \InvalidArgumentException('Field name is absent or ambiguous; select from fields using descriptor metadata.');
        return $matches[0];
    }
}

final class Batch implements \Countable, \IteratorAggregate {
    /** @param list<Input> $events */
    public function __construct(public array $events) {}
    public function count(): int { return count($this->events); }
    public function getIterator(): \Traversable { yield from $this->events; }
}

/** Direct Windows x64 FFI client. Receive consumes input: always forward or explicitly discard each batch. */
final class Interceptor {
    /** Keep each DLL/FFI scope alive for all of its sessions. */
    private static array $libraries = [];
    private FFI $ffi;
    private int $session = 0;
    private ?Batch $current = null;
    private ?CData $nativeBatch = null;
    private array $issuedEvents = [];
    private const KINDS = ['', 'keyboard', 'mouse', 'hid', 'xinput', 'xinputhid', 'gip'];

    public function __construct(?string $libraryPath = null) {
        if (PHP_OS_FAMILY !== 'Windows' || PHP_INT_SIZE !== 8) throw new \RuntimeException('Interceptor requires Windows x64 PHP.');
        if (!extension_loaded('FFI')) throw new \RuntimeException('Enable the PHP FFI extension and ffi.enable for this CLI application.');
        $libraryPath ??= getenv('WINDOWS_GAMEPAD_FILTER_SDK_NATIVE_PATH') ?: dirname(__DIR__) . '/native/WindowsGamepadFilterBridge.dll';
        $key = strtolower(realpath($libraryPath) ?: $libraryPath);
        $this->ffi = self::$libraries[$key] ??= FFI::cdef(file_get_contents(dirname(__DIR__) . '/native/ffi.h'), $libraryPath);
        foreach ([1 => ['windows_gamepad_filter_field',120], 2 => ['windows_gamepad_filter_event',64], 3 => ['windows_gamepad_filter_batch',24], 4 => ['windows_gamepad_filter_device',40], 5 => ['windows_gamepad_filter_error',776], 6 => ['windows_gamepad_filter_info',56]] as $id => [$name,$size]) {
            if ($this->ffi->windows_gamepad_filter_sizeof($id) !== $size || FFI::sizeof($this->ffi->new($name)) !== $size) throw new \RuntimeException('Incompatible native ABI: '.$name);
        }
        if ($this->ffi->windows_gamepad_filter_version() !== 1) throw new \RuntimeException('Unsupported native API version.');
        $session = $this->ffi->new('uint64_t'); $error = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_open(FFI::addr($session), FFI::addr($error)), $error);
        $this->session = $session->cdata;
    }
    private function active(): int { if (!$this->session) throw new \LogicException('Interceptor is closed.'); return $this->session; }
    private function error(): CData { return $this->ffi->new('windows_gamepad_filter_error'); }
    private function check(int $code, CData $error): int {
        if ($code !== 0) throw new NativeException($code, FFI::string($error->message), (int)$error->accepted);
        return (int)$error->accepted;
    }
    private static function text(CData|string|null $pointer): string {
        if (is_string($pointer)) return $pointer;
        return $pointer === null || FFI::isNull($pointer) ? '' : FFI::string($pointer);
    }
    private static function snapshot(CData $object, array $keys): array {
        $result = []; foreach ($keys as $key) $result[$key] = $object->$key; return $result;
    }
    public function initialize(): array {
        $out = $this->ffi->new('windows_gamepad_filter_init'); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_initialize($this->active(), FFI::addr($out), FFI::addr($e)), $e);
        return self::snapshot($out, ['ready','reboot_required','associated','already_associated','skipped']);
    }
    public function info(): array {
        $out = $this->ffi->new('windows_gamepad_filter_info'); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_info_get($this->active(), FFI::addr($out), FFI::addr($e)), $e);
        $r = self::snapshot($out, ['installed','active','trial_state','input_mask','controller_mask','device_count','queued','last_reset']);
        foreach (['remaining_ms','timestamp_frequency','generation'] as $key) $r[$key] = sprintf('%u', $out->$key);
        return $r;
    }
    public function devices(): array {
        $out = $this->ffi->new('const windows_gamepad_filter_devices *'); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_devices_get($this->active(), FFI::addr($out), FFI::addr($e)), $e);
        $result = [];
        for ($i=0; $i<$out->count; $i++) {
            $d = $out->items[$i]; $r = self::snapshot($d, ['id','vid','pid','usage_page','usage']);
            $r['kind'] = self::KINDS[$d->kind] ?? 'unknown';
            foreach (['token','name','hardware_id'] as $key) $r[$key] = self::text($d->$key);
            $result[] = $r;
        }
        return $result;
    }
    public function receive(?int $timeout = null): Batch {
        if ($timeout !== null && ($timeout < 0 || $timeout > 0xfffffffe)) throw new \InvalidArgumentException('Timeout must be milliseconds >= 0, or null for infinite.');
        $this->current = null; $this->nativeBatch = null;
        $out = $this->ffi->new('const windows_gamepad_filter_batch *'); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_receive($this->active(), $timeout ?? 0xffffffff, FFI::addr($out), FFI::addr($e)), $e);
        $events = [];
        for ($i=0; $i<$out->count; $i++) {
            $event = $out->events[$i]; $fields = [];
            for ($j=0; $j<$event->field_count; $j++) {
                $field = $event->fields[$j];
                $metadata = self::snapshot($field, ['minimum','maximum','physical_minimum','physical_maximum','group','index','usage_page','usage','collection','report_id','data_index','element','flags','units','units_exponent','bit_size']);
                $metadata['name'] = self::text($field->name);
                $fields[] = new Field($field->value, $field->normalized, $field->length ? FFI::string($field->data, $field->length) : '', $metadata);
            }
            $input = new Input(self::KINDS[$event->kind] ?? 'unknown', self::text($event->token), sprintf('%u', $event->sequence), sprintf('%u', $event->timestamp), $fields, self::snapshot($event, ['decode_error','decode_complete','parser_status','device']));
            $input->blocked = (bool)$event->blocked; $events[] = $input;
        }
        $this->nativeBatch = $out; $this->issuedEvents = $events; return $this->current = new Batch($events);
    }
    private function validate(Batch $batch): void {
        $this->active();
        if ($batch !== $this->current || $this->nativeBatch === null || $batch->events !== $this->issuedEvents || count($batch) !== $this->nativeBatch->count) throw new \InvalidArgumentException('Stale, resized, reordered or foreign batch.');
    }
    public function send(Batch $batch): int {
        $this->validate($batch);
        // Validate the complete edit set before writing into borrowed native views.
        foreach ($batch->events as $i => $event) {
            foreach ($event->fields as $j => $field) {
                if (!is_finite($field->normalized)) throw new \InvalidArgumentException('Normalized values must be finite.');
                if (strlen($field->data) !== $this->nativeBatch->events[$i]->fields[$j]->length) throw new \InvalidArgumentException('Do not resize native binary/report fields.');
            }
        }
        foreach ($batch->events as $i => $event) {
            $native = $this->nativeBatch->events[$i]; $native->blocked = (int)$event->blocked;
            foreach ($event->fields as $j => $field) {
                $native->fields[$j]->value = $field->value; $native->fields[$j]->normalized = $field->normalized;
                if ($field->data !== '') FFI::memcpy($native->fields[$j]->data, $field->data, strlen($field->data));
            }
        }
        $e = $this->error(); return $this->check($this->ffi->windows_gamepad_filter_send($this->active(), $this->nativeBatch->id, FFI::addr($e)), $e);
    }
    public function discard(Batch $batch): void {
        $this->validate($batch); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_discard($this->active(), $this->nativeBatch->id, FFI::addr($e)), $e);
    }
    public function cancel(): void { $e = $this->error(); $this->check($this->ffi->windows_gamepad_filter_cancel($this->active(), FFI::addr($e)), $e); }
    public function descriptor(string $token, bool $report = false): array {
        if (str_contains($token, "\0")) throw new \InvalidArgumentException('Token contains NUL.');
        $out = $this->ffi->new('windows_gamepad_filter_blob'); $e = $this->error();
        $this->check($this->ffi->windows_gamepad_filter_descriptor($this->active(), $token, (int)$report, FFI::addr($out), FFI::addr($e)), $e);
        return ['applicable'=>(bool)$out->applicable, 'format'=>(int)$out->format, 'data'=>$out->length ? FFI::string($out->data,$out->length) : ''];
    }
    public function inject(string $kind, array $state, ?string $token = null, ?string $report = null): int {
        $type = array_search($kind, self::KINDS, true);
        if (!$type) throw new \InvalidArgumentException('Unknown input kind.');
        if ($token !== null && str_contains($token,"\0")) throw new \InvalidArgumentException('Token contains NUL.');
        $count = count($state) + (int)($report !== null); $fields = $this->ffi->new('windows_gamepad_filter_field['.max(1,$count).']'); $buffers = []; $i = 0;
        foreach ($state as $name => $value) {
            if (!is_string($name) || str_contains($name,"\0") || !is_int($value)) throw new \InvalidArgumentException('State must contain string names and signed 64-bit integers.');
            $buffer = $this->ffi->new('char['.(strlen($name)+1).']'); FFI::memcpy($buffer,$name,strlen($name)); $buffers[] = $buffer;
            $fields[$i]->name = FFI::addr($buffer[0]); $fields[$i]->value = $value; $i++;
        }
        if ($report !== null) {
            $name = $this->ffi->new('char[7]'); FFI::memcpy($name,'report',6); $buffers[] = $name;
            $data = $this->ffi->new('uint8_t['.max(1,strlen($report)).']'); if ($report !== '') FFI::memcpy($data,$report,strlen($report)); $buffers[] = $data;
            $fields[$i]->name = FFI::addr($name[0]); $fields[$i]->group = 7; $fields[$i]->data = FFI::addr($data[0]); $fields[$i]->length = strlen($report);
        }
        $event = $this->ffi->new('windows_gamepad_filter_event'); $event->kind = $type; $event->fields = FFI::addr($fields[0]); $event->field_count = $count;
        if ($token !== null) { $t=$this->ffi->new('char['.(strlen($token)+1).']'); FFI::memcpy($t,$token,strlen($token)); $event->token=FFI::addr($t[0]); }
        $e = $this->error(); return $this->check($this->ffi->windows_gamepad_filter_inject($this->active(), FFI::addr($event), 1, FFI::addr($e)), $e);
    }
    public function close(): void {
        if ($this->session) { $session=$this->session; $this->session=0; $this->current=null; $this->nativeBatch=null; $e=$this->error(); $this->check($this->ffi->windows_gamepad_filter_close($session,FFI::addr($e)),$e); }
    }
    public function __destruct() { try { $this->close(); } catch (\Throwable) {} }
}
