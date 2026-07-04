#pragma once
#ifndef QUERY_UTILS_H_
#define QUERY_UTILS_H_

#include <Arduino.h>

namespace QUERY_Utils {

    // Inspect a received raw AX.25 packet string and respond to APRS station
    // capability queries directed at our callsign (or the configured tactical
    // object name, if set) or broadcast to APRS/IGATE.
    //
    // Handles: ?APRS? ?APRSD ?APRSH ?APRSL ?APRSP ?APRSS ?APRST ?APRSV
    //          ?PING? ?VER ?IGATE?
    //
    // Most responses are queued via STATION_Utils::addToOutputPacketBuffer().
    // ?APRS?/?APRSP instead schedule a jittered position-beacon reply — see
    // tick(). Duplicate queries from the same sender are suppressed for 60 s.
    void processLoRaPacket(const String& rawPacket);

    // Poll for a jittered query-response beacon that's come due. Call every loop
    // iteration (see STATION_Utils::processOutputPacketBuffer() for the same
    // self-contained poll-and-drain pattern).
    void tick();

}

#endif
