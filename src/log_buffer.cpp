/* log_buffer.cpp — ring buffer storage and helpers for web UI log streaming.
 * Only compiled when HAS_WEB_UI is defined; nRF52 and headless builds produce
 * no code or data from this translation unit.
 */

#include "log_buffer.h"

#ifdef HAS_WEB_UI

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

namespace LogBuffer {

Entry    ring[RING_SIZE];       // zero-initialised at startup (seq == 0 → empty)
uint32_t nextSeq  = 1;
uint8_t  writeIdx = 0;

void push(char type, const char* text) {
    Entry& e = ring[writeIdx];
    e.ms   = millis();
    e.seq  = nextSeq++;
    e.type = type;
    strncpy(e.text, text, TEXT_LEN - 1);
    e.text[TEXT_LEN - 1] = '\0';
    writeIdx = (writeIdx + 1) % RING_SIZE;
}

void pushf(char type, const char* fmt, ...) {
    char buf[TEXT_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    push(type, buf);
}

void toJson(const Entry& e, char* buf, int bufLen) {
    // JSON-escape the message text (handles ", \, \n, \r).
    char escaped[TEXT_LEN * 2];
    int j = 0;
    for (int i = 0; e.text[i] != '\0' && i < TEXT_LEN && j < (int)sizeof(escaped) - 3; i++) {
        char c = e.text[i];
        if      (c == '\\') { escaped[j++] = '\\'; escaped[j++] = '\\'; }
        else if (c == '"')  { escaped[j++] = '\\'; escaped[j++] = '"';  }
        else if (c == '\n' || c == '\r') { escaped[j++] = '\\'; escaped[j++] = 'n'; }
        else escaped[j++] = c;
    }
    escaped[j] = '\0';
    snprintf(buf, bufLen, "{\"ms\":%lu,\"t\":\"%c\",\"msg\":\"%s\"}",
             (unsigned long)e.ms, e.type, escaped);
}

} // namespace LogBuffer

#endif // HAS_WEB_UI
