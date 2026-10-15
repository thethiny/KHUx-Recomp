/* OpenSL ES with real output. An SLObjectItf or SLxxxItf in the guest is a pointer to a cell holding a vtable
 * pointer; every vtable entry is a host slot.
 *
 * The game's sound library (sqexsdlib, CoreAudioOutInit) makes one AudioPlayer: 16-bit PCM from an Android simple
 * buffer queue, with a callback that mixes and enqueues the next buffer each time one finishes. Here an audio
 * thread plays through waveOut: it copies enqueued buffers into 20 ms output chunks (four in flight) and calls the
 * game's callback for every buffer it has used, so the game's own mixer runs at the device's pace. The queue
 * state the game sees (GetState) is the number of buffers not yet used. */
#include "host.h"
#include <mmsystem.h>

#define VT_SLOTS 40
#define SL_PLAYSTATE_STOPPED 1
#define SL_PLAYSTATE_PAUSED 2
#define SL_PLAYSTATE_PLAYING 3

typedef struct Buf { uint8_t *data; uint32_t size, pos; struct Buf *next; } Buf;
typedef struct SLObj {
    uint32_t cell;
    char kind[24];
    uint32_t state, play_state;
    struct { char itf[40]; uint32_t cell; } itfs[16];
    int n_itfs;
    /* audio player */
    uint32_t rate, channels, bits;
    Buf *head, *tail;
    uint32_t queued, index;
    uint32_t cb, ctx, bq_cell;
    HANDLE thread;
} SLObj;

#define SL_MAX 512
static struct { uint32_t cell; SLObj *obj; } cells[SL_MAX * 8];
static int n_cells;
static struct { char itf[40]; uint32_t vt; } vtables[64];
static int n_vtables;
static struct { uint32_t guid; char name[40]; } iids[64];
static int n_iids;
static CRITICAL_SECTION sl_lock;

static const char *M_OBJECT[] = {"Realize", "Resume", "GetState", "GetInterface", "RegisterCallback",
    "AbortAsyncOperation", "Destroy", "SetPriority", "GetPriority", "SetLossOfControlInterfaces"};
static const char *M_ENGINE[] = {"CreateLEDDevice", "CreateVibraDevice", "CreateAudioPlayer", "CreateAudioRecorder",
    "CreateMidiPlayer", "CreateListener", "Create3DGroup", "CreateOutputMix", "CreateMetadataExtractor",
    "CreateExtensionObject", "QueryNumSupportedInterfaces", "QuerySupportedInterfaces",
    "QueryNumSupportedExtensions", "QuerySupportedExtension", "IsExtensionSupported"};
static const char *M_PLAY[] = {"SetPlayState", "GetPlayState", "GetDuration", "GetPosition", "RegisterCallback",
    "SetCallbackEventsMask", "GetCallbackEventsMask", "SetMarkerPosition", "ClearMarkerPosition",
    "GetMarkerPosition", "SetPositionUpdatePeriod", "GetPositionUpdatePeriod"};
static const char *M_BQ[] = {"Enqueue", "Clear", "GetState", "RegisterCallback"};
static const char *M_VOLUME[] = {"SetVolumeLevel", "GetVolumeLevel", "GetMaxVolumeLevel", "SetMute", "GetMute",
    "EnableStereoPosition", "IsEnabledStereoPosition", "SetStereoPosition", "GetStereoPosition"};

uint32_t sl_iid(const char *itf) {
    uint32_t g = gzalloc(16);
    { glog(g, (uint32_t)(strlen(itf) < 16 ? strlen(itf) : 16)); memcpy(GP(g), itf, strlen(itf) < 16 ? strlen(itf) : 16); }
    if (n_iids < 64) {
        iids[n_iids].guid = g;
        strncpy(iids[n_iids++].name, itf, 39);
    }
    return g;
}

static const char *iid_name(uint32_t g) {
    for (int k = 0; k < n_iids; k++) if (iids[k].guid == g) return iids[k].name;
    return "UNKNOWN";
}

static void sl_method(CPU *c);

static uint32_t make_cell(const char *itf, SLObj *o) {
    uint32_t vt = 0;
    for (int k = 0; k < n_vtables; k++) if (!strcmp(vtables[k].itf, itf)) vt = vtables[k].vt;
    if (!vt) {
        const char **names = NULL;
        int nn = 0;
        if (!strcmp(itf, "OBJECT")) { names = M_OBJECT; nn = 10; }
        else if (!strcmp(itf, "ENGINE")) { names = M_ENGINE; nn = 15; }
        else if (!strcmp(itf, "PLAY")) { names = M_PLAY; nn = 12; }
        else if (!strcmp(itf, "ANDROIDSIMPLEBUFFERQUEUE") || !strcmp(itf, "BUFFERQUEUE")) { names = M_BQ; nn = 4; }
        else if (!strcmp(itf, "VOLUME")) { names = M_VOLUME; nn = 9; }
        vt = gzalloc(4 * VT_SLOTS);
        for (int k = 0; k < VT_SLOTS; k++) {
            char full[96];
            if (k < nn) snprintf(full, sizeof(full), "SL%s::%s", itf, names[k]);
            else snprintf(full, sizeof(full), "SL%s::slot%d", itf, k);
            gw32(vt + 4u * (uint32_t)k, host_fn(_strdup(full), sl_method));
        }
        if (n_vtables >= 64) fatal("OpenSL: too many interfaces");
        strncpy(vtables[n_vtables].itf, itf, 39);
        vtables[n_vtables++].vt = vt;
    }
    uint32_t cell = gzalloc(4);
    gw32(cell, vt);
    if (n_cells >= SL_MAX * 8) fatal("OpenSL: too many interface cells");
    cells[n_cells].cell = cell;
    cells[n_cells++].obj = o;
    return cell;
}

static SLObj *obj_new(const char *kind) {
    SLObj *o = calloc(1, sizeof(SLObj));
    strncpy(o->kind, kind, 23);
    o->state = 1;              /* SL_OBJECT_STATE_UNREALIZED */
    o->play_state = SL_PLAYSTATE_STOPPED;
    o->cell = make_cell("OBJECT", o);
    return o;
}

static SLObj *owner(uint32_t cell) {
    for (int k = 0; k < n_cells; k++) if (cells[k].cell == cell) return cells[k].obj;
    return NULL;
}

/* ---------------------------------------------------------------- audio thread */
#define CHUNKS 4

/* Fill `out` from the player's queue; for every buffer used up, the game's callback runs (lock released). */
static void fill(CPU *c, SLObj *o, uint8_t *out, uint32_t n) {
    uint32_t got = 0;
    while (got < n) {
        EnterCriticalSection(&sl_lock);
        Buf *b = o->play_state == SL_PLAYSTATE_PLAYING ? o->head : NULL;
        if (!b) { LeaveCriticalSection(&sl_lock); break; }
        uint32_t take = b->size - b->pos;
        if (take > n - got) take = n - got;
        memcpy(out + got, b->data + b->pos, take);
        b->pos += take;
        got += take;
        int used_up = b->pos == b->size;
        if (used_up) {
            o->head = b->next;
            if (!o->head) o->tail = NULL;
            o->queued--;
            o->index++;
            free(b->data);
            free(b);
        }
        uint32_t cb = o->cb, ctx = o->ctx, bq = o->bq_cell;
        LeaveCriticalSection(&sl_lock);
        if (used_up && cb) { shadow_guest_enter(); GCALL(c, cb, bq, ctx); shadow_guest_leave(); }
    }
    if (got < n) memset(out + got, 0, n - got);
}

static DWORD WINAPI audio_thread(LPVOID p) {
    SLObj *o = p;
    HThread *t = thread_host("audio");
    WAVEFORMATEX wf = {0};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = (WORD)o->channels;
    wf.nSamplesPerSec = o->rate;
    wf.wBitsPerSample = (WORD)o->bits;
    wf.nBlockAlign = (WORD)(o->channels * o->bits / 8);
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    HANDLE ev = CreateEventA(NULL, FALSE, FALSE, NULL);
    HWAVEOUT wo;
    MMRESULT mr = waveOutOpen(&wo, WAVE_MAPPER, &wf, (DWORD_PTR)ev, 0, CALLBACK_EVENT);
    if (mr != MMSYSERR_NOERROR) {
        hlog("!! audio: waveOutOpen failed (%u): no sound", mr);
        return 0;
    }
    uint32_t chunk = (wf.nAvgBytesPerSec / 50) / wf.nBlockAlign * wf.nBlockAlign;     /* 20 ms */
    WAVEHDR hdr[CHUNKS];
    for (int k = 0; k < CHUNKS; k++) {
        memset(&hdr[k], 0, sizeof(WAVEHDR));
        hdr[k].lpData = malloc(chunk);
        hdr[k].dwBufferLength = chunk;
        waveOutPrepareHeader(wo, &hdr[k], sizeof(WAVEHDR));
        hdr[k].dwFlags |= WHDR_DONE;                       /* free to fill */
    }
    hlog("audio: %u Hz, %u channels, %u-bit, %u-byte chunks (thread %d)", o->rate, o->channels, o->bits, chunk, t->tid);
    for (;;) {
        for (int k = 0; k < CHUNKS; k++) {
            if (!(hdr[k].dwFlags & WHDR_DONE)) continue;
            fill(t->cpu, o, (uint8_t *)hdr[k].lpData, chunk);
            hdr[k].dwFlags &= ~(DWORD)WHDR_DONE;
            waveOutWrite(wo, &hdr[k], sizeof(WAVEHDR));
        }
        WaitForSingleObject(ev, 10);
    }
}

/* ---------------------------------------------------------------- the interfaces */
static void sl_method(CPU *c) {
    const char *full = import_name(imp_target);           /* "SL<ITF>::<method>" */
    const char *sep = strstr(full, "::");
    char itf[48];
    size_t n = (size_t)(sep - full - 2);
    memcpy(itf, full + 2, n);
    itf[n] = 0;
    const char *m = sep + 2;
    EnterCriticalSection(&sl_lock);
    static char *seen[128];
    static int n_seen;
    int known = 0;
    for (int k = 0; k < n_seen; k++) if (!strcmp(seen[k], full)) known = 1;
    if (!known && n_seen < 128) {
        seen[n_seen++] = _strdup(full);
        hlog("  OpenSL first call: %s(0x%x, 0x%x, 0x%x, 0x%x)", full, c->r[0], c->r[1], c->r[2], c->r[3]);
    }
    SLObj *o = owner(c->r[0]);
    if (!o) fatal("OpenSL: %s on unknown interface 0x%08X", full, c->r[0]);
    uint32_t r = 0;
    int bq = !strcmp(itf, "ANDROIDSIMPLEBUFFERQUEUE") || !strcmp(itf, "BUFFERQUEUE");
    if (!strcmp(itf, "OBJECT") && !strcmp(m, "Realize")) o->state = 2;
    else if (!strcmp(itf, "OBJECT") && !strcmp(m, "GetState")) gw32(c->r[1], o->state);
    else if (!strcmp(itf, "OBJECT") && !strcmp(m, "GetInterface")) {
        const char *name = iid_name(c->r[1]);
        uint32_t cell = 0;
        for (int k = 0; k < o->n_itfs; k++) if (!strcmp(o->itfs[k].itf, name)) cell = o->itfs[k].cell;
        if (!cell) {
            if (o->n_itfs >= 16) fatal("OpenSL: too many interfaces on one object");
            cell = make_cell(name, o);
            strncpy(o->itfs[o->n_itfs].itf, name, 39);
            o->itfs[o->n_itfs++].cell = cell;
        }
        gw32(c->r[2], cell);
    } else if (!strcmp(itf, "ENGINE") && !strcmp(m, "CreateOutputMix")) {
        SLObj *no = obj_new("OutputMix");
        gw32(c->r[1], no->cell);
    } else if (!strcmp(itf, "ENGINE") && !strcmp(m, "CreateAudioPlayer")) {
        /* (self, SLObjectItf *out, SLDataSource *src, SLDataSink *sink, n, iids, required) */
        SLObj *no = obj_new("AudioPlayer");
        uint32_t src = c->r[2], fmt = src ? g32(src + 4) : 0;
        if (fmt && g32(fmt) == 2) {                       /* SL_DATAFORMAT_PCM */
            no->channels = g32(fmt + 4);
            no->rate = g32(fmt + 8) / 1000;               /* milliHz */
            no->bits = g32(fmt + 12);
        }
        if (!no->channels || !no->rate || (no->bits != 16 && no->bits != 8)) {
            hlog("!! OpenSL: unsupported player format (%u ch, %u Hz, %u bits): silent", no->channels, no->rate, no->bits);
            no->channels = 0;
        }
        gw32(c->r[1], no->cell);
        hlog("  OpenSL: AudioPlayer %u Hz, %u channels, %u-bit", no->rate, no->channels, no->bits);
    } else if (!strcmp(itf, "ENGINE") && !strcmp(m, "CreateAudioRecorder")) r = 12;   /* FEATURE_UNSUPPORTED */
    else if (!strcmp(itf, "PLAY") && !strcmp(m, "SetPlayState")) {
        o->play_state = c->r[1];
        if (o->play_state == SL_PLAYSTATE_PLAYING && !o->thread && o->channels)
            o->thread = CreateThread(NULL, 64u << 20, audio_thread, o, 0, NULL);
    } else if (!strcmp(itf, "PLAY") && !strcmp(m, "GetPlayState")) gw32(c->r[1], o->play_state);
    else if (!strcmp(itf, "PLAY") && !strcmp(m, "GetPosition")) gw32(c->r[1], 0);
    else if (!strcmp(itf, "PLAY") && !strcmp(m, "GetDuration")) gw32(c->r[1], 0xFFFFFFFFu);
    else if (bq && !strcmp(m, "GetState")) { gw32(c->r[1], o->queued); gw32(c->r[1] + 4, o->index); }
    else if (bq && !strcmp(m, "Enqueue")) {
        Buf *b = calloc(1, sizeof(Buf));
        b->size = c->r[2];
        b->data = malloc(b->size ? b->size : 1);
        memcpy(b->data, GP(c->r[1]), b->size);
        if (o->tail) o->tail->next = b; else o->head = b;
        o->tail = b;
        o->queued++;
    } else if (bq && !strcmp(m, "Clear")) {
        while (o->head) { Buf *b = o->head; o->head = b->next; free(b->data); free(b); }
        o->tail = NULL;
        o->queued = 0;
    } else if (bq && !strcmp(m, "RegisterCallback")) {
        o->cb = c->r[1];
        o->ctx = c->r[2];
        o->bq_cell = c->r[0];
    } else if (!strcmp(itf, "VOLUME") && (!strcmp(m, "GetMaxVolumeLevel") || !strcmp(m, "GetVolumeLevel"))) gw16(c->r[1], 0);
    else if (!strcmp(itf, "VOLUME") && !strcmp(m, "GetMute")) gw32(c->r[1], 0);
    LeaveCriticalSection(&sl_lock);
    RET(c, r);
}

static void i_sl_create_engine(CPU *c) {
    EnterCriticalSection(&sl_lock);
    SLObj *o = obj_new("Engine");
    LeaveCriticalSection(&sl_lock);
    gw32(c->r[0], o->cell);
    hlog("  slCreateEngine");
    RET(c, 0);
}

void sl_init(void) {
    InitializeCriticalSection(&sl_lock);
    imp("slCreateEngine", i_sl_create_engine);
}
