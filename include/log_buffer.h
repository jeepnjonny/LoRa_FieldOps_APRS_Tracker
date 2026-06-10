/* log_buffer.h — fixed-size ring buffer for web UI live event streaming.
 *
 * Push calls are cheap (static char copy + millis()). When HAS_WEB_UI is not
 * defined (e.g., nRF52840), all functions compile to empty inlines — zero
 * overhead and zero RAM used.
 */

#pragma once

#ifdef HAS_WEB_UI

#include <Arduino.h>
#include <stdarg.h>

namespace LogBuffer {

// Event type tags — displayed as colored badges in the web UI.
static const char TYPE_RX   = 'R';  // LoRa packet received
static const char TYPE_TX   = 'T';  // LoRa packet transmitted
static const char TYPE_DIG  = 'D';  // Digipeated
static const char TYPE_IGT  = 'I';  // iGate upload to APRS-IS
static const char TYPE_NET  = 'N';  // APRS-IS connect / disconnect
static const char TYPE_INFO = 'i';  // General info

// Ring dimensions: 50 entries × 120 chars = ~6 KB static RAM.
// Oldest entry is silently overwritten when the ring is full.
constexpr int TEXT_LEN  = 120;
constexpr int RING_SIZE = 50;

struct Entry {
    uint32_t ms;            // millis() at time of push
    uint32_t seq;           // monotonic sequence number; 0 = empty slot
    char     type;          // TYPE_* constant above
    char     text[TEXT_LEN];
};

extern Entry    ring[RING_SIZE];
extern uint32_t nextSeq;    // next sequence number to assign (starts at 1)
extern uint8_t  writeIdx;   // ring write position

// Push a new event. Safe to call from any main-loop context. O(1), non-blocking.
void push(char type, const char* text);

// Printf-style push.
void pushf(char type, const char* fmt, ...);

// Iterate all ring entries in insertion order (oldest first).
// Skips empty (seq == 0) slots. Fn must have signature: void(const Entry&)
template<typename Fn>
inline void forEach(Fn cb) {
    for (int i = 0; i < RING_SIZE; i++) {
        const Entry& e = ring[(writeIdx + i) % RING_SIZE];
        if (e.seq == 0) continue;
        cb(e);
    }
}

// Serialize one entry to a JSON string into caller-supplied buffer.
// Format: {"ms":12345,"t":"R","msg":"KJ7NYE>APRS:..."}
void toJson(const Entry& e, char* buf, int bufLen);

} // namespace LogBuffer

#else  // HAS_WEB_UI not defined — no-op stubs, zero overhead

namespace LogBuffer {
    static const char TYPE_RX   = 'R';
    static const char TYPE_TX   = 'T';
    static const char TYPE_DIG  = 'D';
    static const char TYPE_IGT  = 'I';
    static const char TYPE_NET  = 'N';
    static const char TYPE_INFO = 'i';

    static inline void push(char, const char*) {}
    static inline void pushf(char, const char*, ...) {}
}

#endif // HAS_WEB_UI
