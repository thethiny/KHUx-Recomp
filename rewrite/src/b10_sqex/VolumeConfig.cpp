// Batch #10: SQEX::VolumeConfigSystem - per-bus and per-label volumes, loaded from and saved to a small text
// format:
//   ;BUS MASTER <volume>
//   ;BUS <bus> <volume>
//   <label> <volume>        (the label belongs to the bus line above it)
//   ;END
// Buses are 16-byte entries {name[12], volume +0xC}; labels 60-byte entries {bus* +0, name[0x32] +4, volume +0x38}.
// Names compare case-insensitively over the asked name's length.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

// the system's globals (each read through its GOT slot)
constexpr uint32_t BUSES_GOT = 0xFD92D8, LABELS_GOT = 0xFD92DC, INITIALIZED_GOT = 0xFD92E0,
                   MAX_BUSES_GOT = 0xFD92E4, MAX_LABELS_GOT = 0xFD92E8, MASTER_GOT = 0xFD92EC,
                   LOADED_GOT = 0xFD92F0, BUS_COUNT_GOT = 0xFD92F4, LABEL_COUNT_GOT = 0xFD92F8;
constexpr uint32_t BUS_SIZE = 0x10, LABEL_SIZE = 0x3C;
// imports and libc
constexpr uint32_t STRLEN = 0x40E08C, STRNCASECMP = 0x442370, MEMCPY = 0x442298, MEMSET = 0x442310,
                   STRCHR = 0x410198, STRCPY = 0x42A9AC, STRCAT = 0x44A9D4, MALLOC = 0x40FF58, FREE = 0x40FF64;
constexpr uint32_t STRTOD = rwlib::sub_C9CC58, SPRINTF = 0xCA4A95;
constexpr uint32_t S_END = 0xEEDBDC, S_BUS = 0xEEDBE4, S_MASTER = 0xEEDBEC, S_FLOAT = 0xEEDBF4;
constexpr uint32_t S_BUS_MASTER = 0xEEDBF8;   // ";BUS MASTER " (12 bytes)
constexpr uint32_t S_BUS_PREFIX = 0xEEDC08;   // ";BUS " (5 bytes with its NUL)
constexpr uint32_t S_NEWLINE = 0xEE7940;      // "\n"
constexpr uint32_t DATA_BUFFER_SIZE = 0xFA000;

uint32_t var(uint32_t got) { return ld32(got); }   // the global's address
uint32_t strlen_(uint32_t s) { return rw::call(STRLEN, {s}); }
bool startsWith(uint32_t s, uint32_t prefix, uint32_t n) { return rw::call(STRNCASECMP, {s, prefix, n}) == 0; }
float strtod_(uint32_t s) {
    const uint64_t bits = rw::call64(STRTOD, {s, 0});
    double d;
    std::memcpy(&d, &bits, 8);
    return float(d);
}
void formatFloat(uint32_t buf, float v) {
    const double d = v;
    uint64_t bits;
    std::memcpy(&bits, &d, 8);
    rw::call(SPRINTF, {buf, S_FLOAT, uint32_t(bits), uint32_t(bits >> 32)});
}

// the label whose name starts like `name` (labels' names at +4), or 0
uint32_t findLabel(uint32_t name) {
    const int32_t n = lds32(var(LABEL_COUNT_GOT));
    if (n <= 0) return 0;
    const uint32_t len = strlen_(name);
    uint32_t label = ld32(var(LABELS_GOT));
    for (int32_t k = 0; k < n; k++, label += LABEL_SIZE)
        if (startsWith(name, label + 4, len)) return label;
    return 0;
}
uint32_t findBus(uint32_t name) {
    const int32_t n = lds32(var(BUS_COUNT_GOT));
    if (n <= 0) return 0;
    const uint32_t len = strlen_(name);
    uint32_t bus = ld32(var(BUSES_GOT));
    for (int32_t k = 0; k < n; k++, bus += BUS_SIZE)
        if (startsWith(name, bus, len)) return bus;
    return 0;
}

// Load's scan from a token to the next one: the start of the token after the one at p, or where the text ends
uint32_t nextToken(uint32_t p) {
    uint32_t c = ld8(p);
    if (c == 0) return p;
    bool space = false;
    for (;;) {
        if (c > 0x20 && space) return p;
        if (c <= 0x20) space = true;
        c = ld8(p + 1);
        p++;
        if (c == 0) return p;
    }
}

}  // namespace

struct VolumeConfig {
    static int32_t initialize(uint32_t maxBuses, uint32_t maxLabels) {
        const uint32_t buses = guest::newArray(maxBuses <= 0x7F00000u ? maxBuses * BUS_SIZE : 0xFFFFFFFFu);
        st32(var(BUSES_GOT), buses);
        if (buses == 0) return -1;
        const uint32_t labels = guest::newArray(maxLabels <= 0x2200000u ? maxLabels * LABEL_SIZE : 0xFFFFFFFFu);
        st32(var(LABELS_GOT), labels);
        if (labels == 0) return -1;
        st8(var(INITIALIZED_GOT), 1);
        st32(var(MAX_BUSES_GOT), maxBuses);
        st32(var(MAX_LABELS_GOT), maxLabels);
        return 0;
    }

    // parses the text up to ";END" (0); -1 on a malformed line or when a table is full (at max - 1 entries)
    static int32_t load(uint32_t text) {
        const uint32_t busCount = var(BUS_COUNT_GOT), labelCount = var(LABEL_COUNT_GOT);
        st32(busCount, 0);
        st32(labelCount, 0);
        uint32_t bus = 0;   // the bus the following labels belong to
        for (uint32_t line = text;; line = rw::call(STRCHR, {line, '\n'}) + 1) {
            if (ld8(line) == '\n') continue;
            uint32_t p = line;
            while (ld8(p) - 1u <= 0x1Fu) p++;   // leading blanks
            if (startsWith(p, S_END, 4)) {
                st8(var(LOADED_GOT), 1);
                return 0;
            }
            if (!startsWith(p, S_BUS, 4)) {
                // "<label> <volume>"
                const uint32_t value = nextToken(p);
                const int32_t n = lds32(labelCount);
                if (n + 1 == lds32(var(MAX_LABELS_GOT))) return -1;
                rw::call(MEMSET, {ld32(var(LABELS_GOT)) + uint32_t(n) * LABEL_SIZE + 4, 0, 0x32});
                rw::call(MEMCPY, {ld32(var(LABELS_GOT)) + ld32(labelCount) * LABEL_SIZE + 4, p, value - p - 1});
                const uint32_t label = ld32(var(LABELS_GOT)) + ld32(labelCount) * LABEL_SIZE;
                stf(label + 0x38, 0.0f);
                if (ld8(value) == 0) return -1;
                stf(label + 0x38, strtod_(value));
                if (bus == 0) return -1;
                const uint32_t k = ld32(labelCount);
                st32(ld32(var(LABELS_GOT)) + k * LABEL_SIZE, bus);
                st32(labelCount, k + 1);
                continue;
            }
            // ";BUS <name> <volume>": the name token (a NUL before it is an error)
            uint32_t c = ld8(p);
            bool space = c <= 0x20;
            if (c == 0) return -1;
            for (;;) {
                if (c > 0x20 && space) break;
                if (c <= 0x20) space = true;
                c = ld8(++p);
                if (c == 0) return -1;
            }
            const uint32_t name = p;
            if (startsWith(name, S_MASTER, 6)) {
                const uint32_t value = nextToken(name);
                const uint32_t master = var(MASTER_GOT);
                stf(master, 0.0f);
                if (ld8(value) == 0) return -1;
                stf(master, strtod_(value));
                continue;
            }
            const uint32_t value = nextToken(name);
            const int32_t n = lds32(busCount);
            if (n + 1 == lds32(var(MAX_BUSES_GOT))) return -1;
            const uint32_t entry = ld32(var(BUSES_GOT)) + uint32_t(n) * BUS_SIZE;
            for (uint32_t k = 0; k < 10; k++) st8(entry + k, 0);   // (10 of the name's 12 bytes)
            rw::call(MEMCPY, {ld32(var(BUSES_GOT)) + ld32(busCount) * BUS_SIZE, name, value - name - 1});
            const uint32_t newBus = ld32(var(BUSES_GOT)) + ld32(busCount) * BUS_SIZE;
            // the same bus named again right after itself: skipped
            if (bus != 0 && startsWith(newBus, bus, strlen_(newBus))) continue;
            stf(newBus + 0xC, 0.0f);
            if (ld8(value) == 0) return -1;
            const float volume = strtod_(value);
            const uint32_t k = ld32(busCount);
            st32(busCount, k + 1);
            bus = ld32(var(BUSES_GOT)) + k * BUS_SIZE;
            stf(newBus + 0xC, volume);
        }
    }

    // the length of the text GetData writes (0 before a load)
    static uint32_t getDataSize() {
        if (ld8(var(LOADED_GOT)) == 0) return 0;
        guest::Frame f(0x20);
        const uint32_t number = f.addr(0x10);   // 10 bytes
        *f.at<uint32_t>(0x10) = 0;
        *f.at<uint32_t>(0x14) = 0;
        *f.at<uint16_t>(0x18) = 0;
        formatFloat(number, ldf(var(MASTER_GOT)));
        uint32_t total = strlen_(number) + 0xD;
        const uint32_t labelCount = var(LABEL_COUNT_GOT), labels = var(LABELS_GOT);
        uint32_t lastBus = 0;
        for (int32_t k = 0; k < lds32(labelCount); k++) {
            const uint32_t offset = uint32_t(k) * LABEL_SIZE;
            const uint32_t bus = ld32(ld32(labels) + offset);
            if (bus != lastBus) {
                formatFloat(number, ldf(bus + 0xC));
                const uint32_t nameLen = strlen_(bus);
                total = total + 8 + nameLen + strlen_(number);
            }
            formatFloat(number, ldf(ld32(labels) + offset + 0x38));
            const uint32_t nameLen = strlen_(ld32(labels) + offset + 4);
            total = total + nameLen + 2 + strlen_(number);
            lastBus = bus;
        }
        return total + 7;
    }

    // writes the text to dst (built in a 1000 KB scratch buffer); -1 before a load
    static int32_t getData(uint32_t dst) {
        if (ld8(var(LOADED_GOT)) == 0) return -1;
        const uint32_t buf = rw::call(MALLOC, {DATA_BUFFER_SIZE});
        rw::call(MEMSET, {buf, 0, DATA_BUFFER_SIZE});
        guest::Frame f(0x30);
        const uint32_t number = f.addr(0x20);
        *f.at<uint32_t>(0x20) = 0;
        *f.at<uint32_t>(0x24) = 0;
        *f.at<uint16_t>(0x28) = 0;
        formatFloat(number, ldf(var(MASTER_GOT)));
        const auto append = [&](uint32_t src, uint32_t n) {   // n bytes of a constant at the end
            const uint32_t len = strlen_(buf);
            for (uint32_t k = 0; k < n; k++) st8(buf + len + k, ld8(src + k));
            return len + n;
        };
        const auto appendNumber = [&](uint32_t at) { rw::call(STRCPY, {buf + at, number}); };
        appendNumber(append(S_BUS_MASTER, 12));
        append(S_NEWLINE, 2);
        const uint32_t labelCount = var(LABEL_COUNT_GOT), labels = var(LABELS_GOT);
        uint32_t lastBus = 0;
        for (int32_t k = 0; k < lds32(labelCount); k++) {
            const uint32_t offset = uint32_t(k) * LABEL_SIZE;
            const uint32_t bus = ld32(ld32(labels) + offset);
            if (bus != lastBus) {
                formatFloat(number, ldf(bus + 0xC));
                uint32_t len = strlen_(buf);
                st8(buf + len, '\n');
                for (uint32_t j = 0; j < 5; j++) st8(buf + len + 1 + j, ld8(S_BUS_PREFIX + j));
                rw::call(STRCPY, {buf + len + 6, bus});
                len = strlen_(buf);
                st8(buf + len, ' ');
                rw::call(STRCPY, {buf + len + 1, number});
                append(S_NEWLINE, 2);
            }
            formatFloat(number, ldf(ld32(labels) + offset + 0x38));
            rw::call(STRCAT, {buf, ld32(labels) + offset + 4});
            const uint32_t len = strlen_(buf);
            st8(buf + len, ' ');
            rw::call(STRCPY, {buf + len + 1, number});
            append(S_NEWLINE, 2);
            lastBus = bus;
        }
        const uint32_t len = strlen_(buf);
        st8(buf + len, '\n');
        for (uint32_t j = 0; j < 5; j++) st8(buf + len + 1 + j, ld8(S_END + j));
        rw::call(MEMCPY, {dst, buf, len + 7});
        rw::call(FREE, {buf});
        return 0;
    }

    static uint32_t getBusVolume(uint32_t name) {
        const uint32_t bus = findBus(name);
        return bus ? ld32(bus + 0xC) : 0x3F800000;
    }
    static int32_t setBusVolume(uint32_t name, uint32_t volume) {
        const uint32_t bus = findBus(name);
        if (bus == 0) return -1;
        st32(bus + 0xC, volume);
        return 0;
    }
    static uint32_t getLabelVolume(uint32_t name) {
        const uint32_t label = findLabel(name);
        return label ? ld32(label + 0x38) : 0x3F800000;
    }
    static int32_t setLabelVolume(uint32_t name, uint32_t volume) {
        const uint32_t label = findLabel(name);
        if (label == 0) return -1;
        st32(label + 0x38, volume);
        return 0;
    }
    // a label's volume times its bus's
    static uint32_t getVolume(uint32_t name) {
        const uint32_t label = findLabel(name);
        if (label == 0) return 0x3F800000;
        return fbits(ldf(label + 0x38) * ldf(ld32(label) + 0xC));
    }
};
RW_FUNCTION(0xCE43C4, "SQEX::VolumeConfigSystem::Initialize", VolumeConfig::initialize);
RW_FUNCTION(0xCE4540, "SQEX::VolumeConfigSystem::Load", VolumeConfig::load);
RW_FUNCTION(0xCE4A10, "SQEX::VolumeConfigSystem::GetDataSize", VolumeConfig::getDataSize);
RW_FUNCTION(0xCE4BCC, "SQEX::VolumeConfigSystem::GetData", VolumeConfig::getData);
RW_FUNCTION(0xCE4ED8, "SQEX::VolumeConfigSystem::GetBusVolume", VolumeConfig::getBusVolume);
RW_FUNCTION(0xCE4F50, "SQEX::VolumeConfigSystem::SetBusVolume", VolumeConfig::setBusVolume);
RW_FUNCTION(0xCE4FD4, "SQEX::VolumeConfigSystem::GetLabelVolume", VolumeConfig::getLabelVolume);
RW_FUNCTION(0xCE504C, "SQEX::VolumeConfigSystem::SetLabelVolume", VolumeConfig::setLabelVolume);
RW_FUNCTION(0xCE50D0, "SQEX::VolumeConfigSystem::GetVolume", VolumeConfig::getVolume);

}  // namespace sqex
