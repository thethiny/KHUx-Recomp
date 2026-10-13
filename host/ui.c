/* Android UI the game asks Java for: the text input dialog (EditBox) and text rendered in a system font
 * (Cocos2dxBitmap). */
#include "host.h"

extern HWND gl_hwnd;

/* ---------------------------------------------------------------- text input box
 * A small window over the game with an edit field and OK; Enter = OK, closing it = keep the old text. Not modal:
 * the game keeps rendering, and the main loop delivers the answer (edit_box_take). */
#define ID_EDIT 100
static HWND box, edit;
static char box_old[1024];
static char *box_answer;            /* set when the user is done; taken by the main loop */
static int box_inited;

static void box_done(int ok) {
    if (!box) return;
    char *r;
    if (ok) {
        int n = GetWindowTextLengthW(edit);
        wchar_t *w = calloc((size_t)n + 1, sizeof(wchar_t));
        GetWindowTextW(edit, w, n + 1);
        int len = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
        r = calloc((size_t)len + 1, 1);
        WideCharToMultiByte(CP_UTF8, 0, w, -1, r, len, NULL, NULL);
        free(w);
    } else {
        r = _strdup(box_old);
    }
    HWND b = box;
    box = edit = NULL;
    DestroyWindow(b);
    if (gl_hwnd) SetForegroundWindow(gl_hwnd);
    free(box_answer);
    box_answer = r;
}

static LRESULT CALLBACK box_proc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wp) == IDOK) { box_done(1); return 0; }
        if (LOWORD(wp) == IDCANCEL) { box_done(0); return 0; }
        break;
    case WM_CLOSE: box_done(0); return 0;
    }
    return DefWindowProcW(h, msg, wp, lp);
}

void edit_box_open(const char *title, const char *current, int max_len, int password) {
    HINSTANCE inst = GetModuleHandleW(NULL);
    if (!box_inited) {
        WNDCLASSW wc = {0};
        wc.lpfnWndProc = box_proc;
        wc.hInstance = inst;
        wc.lpszClassName = L"KHUxInput";
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc);
        box_inited = 1;
    }
    if (box) box_done(0);
    snprintf(box_old, sizeof(box_old), "%s", current);
    wchar_t wt[256], wc_[1024];
    MultiByteToWideChar(CP_UTF8, 0, title && *title ? title : "Input", -1, wt, 256);
    MultiByteToWideChar(CP_UTF8, 0, current, -1, wc_, 1024);
    RECT gr = {0};
    if (gl_hwnd) GetWindowRect(gl_hwnd, &gr);
    int bw = 420, bh = 120;
    box = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_CONTROLPARENT, L"KHUxInput", wt,
                          WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, gr.left + (gr.right - gr.left - bw) / 2,
                          gr.top + (gr.bottom - gr.top - bh) / 2, bw, bh, gl_hwnd, NULL, inst, NULL);
    HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    edit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", wc_, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL |
                           (password ? ES_PASSWORD : 0), 12, 14, bw - 40, 24, box, (HMENU)ID_EDIT, inst, NULL);
    HWND ok = CreateWindowExW(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
                              bw - 108, 46, 80, 26, box, (HMENU)IDOK, inst, NULL);
    SendMessageW(edit, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessageW(ok, WM_SETFONT, (WPARAM)font, TRUE);
    if (max_len > 0) SendMessageW(edit, EM_LIMITTEXT, (WPARAM)max_len, 0);
    SendMessageW(edit, EM_SETSEL, 0, -1);
    SetForegroundWindow(box);
    SetFocus(edit);
}

/* Enter/Escape/Tab handling for the box (called by the message pump) */
int edit_box_message(MSG *m) { return box && IsDialogMessageW(box, m); }

/* main loop: the finished answer, or NULL */
char *edit_box_take(void) {
    char *r = box_answer;
    box_answer = NULL;
    return r;
}

/* the control channel answered instead: close the box without an answer */
void edit_box_dismiss(void) {
    if (!box) return;
    HWND b = box;
    box = edit = NULL;
    DestroyWindow(b);
}

/* ---------------------------------------------------------------- text in a system font */
typedef struct Line { wchar_t *s; int n, w; } Line;

static int text_width(HDC dc, const wchar_t *s, int n) {
    SIZE sz = {0};
    if (n > 0) GetTextExtentPoint32W(dc, s, n, &sz);
    return sz.cx;
}

/* Lines: explicit newlines, then word wrap at max_w (0 = no wrap). */
static int layout(HDC dc, wchar_t *t, int max_w, Line *out, int cap) {
    int nl = 0;
    wchar_t *p = t;
    for (;;) {
        wchar_t *e = wcschr(p, L'\n');
        int len = e ? (int)(e - p) : (int)wcslen(p);
        int start = 0;
        do {
            int take = len - start;
            if (max_w > 0) {
                while (take > 0 && text_width(dc, p + start, take) > max_w) {
                    int k = take - 1;
                    while (k > 0 && p[start + k] != L' ') k--;
                    take = k > 0 ? k : take - 1;      /* break at a space, else mid-word */
                }
                if (take <= 0) take = 1;
            }
            if (nl < cap) {
                out[nl].s = p + start;
                out[nl].n = take;
                out[nl].w = text_width(dc, p + start, take);
                nl++;
            }
            start += take;
            while (start < len && p[start] == L' ') start++;
        } while (start < len);
        if (!e) break;
        p = e + 1;
    }
    return nl;
}

uint8_t *text_render(const char *utf8, int size, int align, int want_w, int want_h, float r, float g, float b,
                     int stroke, float sr, float sg, float sb, int *out_w, int *out_h) {
    int wl = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t *t = calloc((size_t)wl + 1, sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, t, wl);
    HDC dc = CreateCompatibleDC(NULL);
    HFONT font = CreateFontW(-size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial");
    HGDIOBJ old_font = SelectObject(dc, font);
    TEXTMETRICW tm;
    GetTextMetricsW(dc, &tm);
    Line lines[256];
    int nl = layout(dc, t, want_w > 0 ? want_w - 2 * stroke : 0, lines, 256);
    int text_w = 0;
    for (int k = 0; k < nl; k++) if (lines[k].w > text_w) text_w = lines[k].w;
    int line_h = tm.tmHeight, text_h = line_h * nl;
    int w = want_w > 0 ? want_w : text_w + 2 * stroke, h = want_h > 0 ? want_h : text_h + 2 * stroke;
    if (w <= 0 || h <= 0) { SelectObject(dc, old_font); DeleteObject(font); DeleteDC(dc); free(t); return NULL; }
    /* white-on-black coverage mask in a 32-bit top-down DIB */
    BITMAPINFO bi = {0};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    uint32_t *px;
    HBITMAP bmp = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, (void **)&px, NULL, 0);
    HGDIOBJ old_bmp = SelectObject(dc, bmp);
    memset(px, 0, (size_t)w * h * 4);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255));
    int ha = align & 0x0F, va = (align >> 4) & 0x0F;
    int y0 = va == 2 ? h - text_h - stroke : va == 3 ? (h - text_h) / 2 : stroke;
    int *xs = calloc((size_t)(nl ? nl : 1), sizeof(int));
    for (int k = 0; k < nl; k++) {
        int x = ha == 2 ? w - lines[k].w - stroke : ha == 3 ? (w - lines[k].w) / 2 : stroke;
        xs[k] = x;
        TextOutW(dc, x, y0 + k * line_h, lines[k].s, lines[k].n);
    }
    GdiFlush();
    uint8_t *cov = malloc((size_t)w * h), *outline = NULL;
    for (int k = 0; k < w * h; k++) cov[k] = (uint8_t)(px[k] & 0xFF);
    if (stroke > 0) {
        /* outline coverage: the text dilated by `stroke` pixels (max over a disc) */
        outline = calloc((size_t)w * h, 1);
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++) {
                uint8_t m = 0;
                for (int dy = -stroke; dy <= stroke; dy++)
                    for (int dx = -stroke; dx <= stroke; dx++) {
                        if (dx * dx + dy * dy > stroke * stroke) continue;
                        int xx = x + dx, yy = y + dy;
                        if (xx < 0 || yy < 0 || xx >= w || yy >= h) continue;
                        if (cov[yy * w + xx] > m) m = cov[yy * w + xx];
                    }
                outline[y * w + x] = m;
            }
    }
    /* premultiplied RGBA (Android bitmap memory order R, G, B, A): fill over outline */
    uint8_t *rgba = malloc((size_t)w * h * 4);
    for (int k = 0; k < w * h; k++) {
        float fa = cov[k] / 255.0f, oa = outline ? outline[k] / 255.0f : 0;
        float a = fa + oa * (1 - fa);
        float cr = r * fa + sr * oa * (1 - fa), cg = g * fa + sg * oa * (1 - fa), cb = b * fa + sb * oa * (1 - fa);
        rgba[4 * k] = (uint8_t)(cr * 255 + 0.5f);
        rgba[4 * k + 1] = (uint8_t)(cg * 255 + 0.5f);
        rgba[4 * k + 2] = (uint8_t)(cb * 255 + 0.5f);
        rgba[4 * k + 3] = (uint8_t)(a * 255 + 0.5f);
    }
    free(xs);
    free(cov);
    free(outline);
    SelectObject(dc, old_bmp);
    SelectObject(dc, old_font);
    DeleteObject(bmp);
    DeleteObject(font);
    DeleteDC(dc);
    free(t);
    *out_w = w;
    *out_h = h;
    return rgba;
}
