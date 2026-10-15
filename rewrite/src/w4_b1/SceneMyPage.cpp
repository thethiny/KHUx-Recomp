// SceneMyPage: construction, destruction and init.
#include "SceneMyPage_lib.hpp"

using namespace mypage;

SceneMyPage::Addr SceneMyPage::construct(Addr self) {
    call(smain::lib::Layer_ctor, {self});
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->vtable2 = VTABLE2;
    call(wl::MEMSET, {self + 0x28C, 0x64, 0});
    rw::set<uint8_t>(s, 0x2F0, 1);
    for (uint32_t off : {0x2F8u, 0x2F7u, 0x2F6u, 0x2F5u, 0x2F4u, 0x2F3u, 0x2F2u, 0x2F1u, 0x2F9u})
        rw::set<uint8_t>(s, off, 0);
    call(wl::MEMSET, {self + 0x2FC, 0x7C, 0});
    call(wl::Vec2_ctor, {self + OFF_CAMERA});
    call(wl::MEMSET, {self + 0x380, 0x29, 0});
    rw::set<uint8_t>(s, 0x3A9, 1);
    rw::set<uint32_t>(s, 0x3D8, 0);
    rw::set<uint32_t>(s, 0x3DC, 0);
    call(wl::MEMSET, {self + 0x3AA, 0x13, 0});
    rw::set<uint8_t>(s, 0x3CC, 0);
    rw::set<uint32_t>(s, 0x3C8, 0);
    rw::set<uint32_t>(s, 0x3C4, 0);
    rw::set<uint32_t>(s, 0x3C0, 0);
    rw::set<uint32_t>(s, 0x3D4, self + 0x3D8);     // images: empty map
    rw::set<uint32_t>(s, 0x3E4, 0);
    rw::set<uint32_t>(s, 0x3E8, 0);
    rw::set<uint32_t>(s, 0x3E0, self + 0x3E4);     // downloads: empty map
    rw::set<uint8_t>(s, 0x410, 0);
    call(wl::MEMSET, {self + 0x3EC, 0x20, 0});
    call(wl::MEMSET, {self + 0x414, 0x14, 0});
    return self;
}

SceneMyPage::Addr SceneMyPage::destroy(Addr self) {
    Self *s = S(self);
    rw::set<uint32_t>(s, 0, VTABLE);
    s->vtable2 = VTABLE2;
    call(M_cancelShopHandle, {self});
    // release the downloaded banner images, then clear the map
    const uint32_t imagesEnd = self + 0x3D8;
    for (uint32_t n = ld32(self + OFF_IMAGES); n != imagesEnd; n = treeNext(n))
        if (ld32(n + 0x14)) call(smain::lib::Ref_release, {ld32(n + 0x14)});
    call(ImageMap_destroy, {self + OFF_IMAGES, ld32(imagesEnd)});
    rw::set<uint32_t>(s, 0x3DC, 0);
    rw::set<uint32_t>(s, OFF_IMAGES, imagesEnd);
    rw::set<uint32_t>(s, 0x3D8, 0);
    uint32_t downloader = ld32(self + OFF_DOWNLOADER);
    if (downloader) call(fn::operator_delete, {call(ImageDownloader_dtor, {downloader})});
    rw::set<uint32_t>(s, OFF_DOWNLOADER, 0);
    call(IntVector_baseDtor, {self + OFF_INTS});
    call(UIntMap_destroy, {self + OFF_DOWNLOADS, ld32(self + 0x3E4)});
    call(ImageMap_destroy, {self + OFF_IMAGES, ld32(imagesEnd)});
    call(U64Vector_baseDtor, {self + OFF_IDS2});
    call(wl::Vec2_dtor, {self + OFF_CAMERA});
    call(MotionVector_baseDtor, {self + OFF_MOTIONS});
    call(ActorVector_dtor, {self + OFF_ACTORS});
    call(WidgetVector_dtor, {self + OFF_BALOONS});
    if (ld32(self + OFF_BALOON_SHOWN)) call(fn::operator_delete, {ld32(self + OFF_BALOON_SHOWN)});
    if (ld32(self + OFF_VEC_328)) call(fn::operator_delete, {ld32(self + OFF_VEC_328)});
    call(UIntVector_baseDtor, {self + OFF_ROUTE});
    call(UIntVectorVector_baseDtor, {self + OFF_ROUTES});
    call(Vec2Vector_baseDtor, {self + OFF_POINTS});
    call(ActorVector_dtor, {self + OFF_ACTORS_B});
    call(ActorVector_dtor, {self + OFF_ACTORS_A});
    call(U64Vector_baseDtor, {self + OFF_IDS});
    release(ld32(self + OFF_SHARED_CTL));
    call(smain::lib::Layer_dtor, {self});
    return self;
}

SceneMyPage::Addr SceneMyPage::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, 0x248, {self}) == 1) {
        call(smain::lib::Ref_autorelease, {self});
        return self;
    }
    vcall(self, 4, {self});
    return 0;
}

SceneMyPage::Addr SceneMyPage::createScene() {
    uint32_t scene = call(smain::lib::Scene_create, {});
    uint32_t layer = create();
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

// cancel the banner downloads still running, clear their map
void SceneMyPage::cancelShopHandle(Addr self) {
    const uint32_t end = self + 0x3E4;
    for (uint32_t n = ld32(self + OFF_DOWNLOADS); n != end; n = treeNext(n))
        call(DownloadManager_cancel, {call(DownloadManager_getInstance, {}), ld32(n + 0x14)});
    call(UIntMap_destroy, {self + OFF_DOWNLOADS, ld32(end)});
    Self *s = S(self);
    rw::set<uint32_t>(s, 0x3E8, 0);
    rw::set<uint32_t>(s, OFF_DOWNLOADS, end);
    rw::set<uint32_t>(s, 0x3E4, 0);
}

// sub_63E3AC: function<void(Touch*, Event*)>& = init::$_11{this} (swap with a temporary, which is destroyed)
uint32_t SceneMyPage::assignTouchEnded(Addr function, Addr lambda) {
    Frame f;
    uint32_t fo = f.alloc(24);
    st32(fo + 16, fo);
    st32(fo, VT_init_11);
    st32(fo + 4, ld32(lambda));
    call(wl::FunctionTouchV_swap, {fo, function});
    functionDestroy(fo);
    return 0;
}

namespace {
constexpr uint32_t S_SCENE = 0xE0363F;   // cocostudio/publish/MyPageScene_ver112.json (42)
constexpr uint32_t S_CENTER_UI_BG = 0xE0366A, S_PANEL_6 = 0xE03675, S_IMAGE_DB1 = 0xE0367D, S_SHADE = 0xE03687;
constexpr uint32_t S_SHADE_TOP = 0xE0368D, S_SHADE_BOTTOM = 0xE03697, S_JEWEL_BIG = 0xE036A4;
constexpr uint32_t S_TXT_WORDING1 = 0xDFF14C, S_PANEL_1 = 0xE02A0D, S_RIGHT_SOLO = 0xE036B1;
constexpr uint32_t S_JEWEL_SOLO = 0xE036BE, S_RIGHT_PARTY = 0xE036CC, S_CENTER_PARTY = 0xE036DA;
constexpr uint32_t S_RANK_SOLO1 = 0xE036E9, S_RANK_SOLO2 = 0xE036F4, S_BUTTON_PARTY2 = 0xE036FF;
constexpr uint32_t S_TXT_PARTY = 0xDF8DC4, S_CENTER_RANK = 0xE0370D, S_BUTTON_RANK = 0xE0371B;
constexpr uint32_t S_TXT_RANK = 0xDFE900, S_BUTTON_SEARCH = 0xE03727, S_TXT_SEARCH = 0xE0373A;
constexpr uint32_t S_RIGHT_BANNER = 0xE0374A, S_BUTTON_UPDATE = 0xDF9DFB;
constexpr uint32_t S_BALOON_JSON = 0xE03759, S_FUKIDASHI = 0xE03781;   // avatarFukidashi_%d
constexpr uint32_t ANCHOR_MIDDLE = 0xFEF5C0;

// a shadowed label with a TextData text
void shadowText(uint32_t button, uint32_t name, uint32_t len, int32_t textId) {
    uint32_t text = findNode(button, name, len);
    call(SceneUtil_setShadow, {text});
    setTextId(text, textId);
}

// node->setPositionY(node->getPositionY() +/- SceneUtil::getSideHeightMargin())
void shiftY(uint32_t n, bool up) {
    float y = bf(vcall(n, node::getPositionY, {n}));
    float m = bf(call(SceneUtil_getSideHeightMargin, {}));
    vcall(n, node::setPositionY, {n, fb(up ? y + m : y - m)});
}

// a centred child widget: position = (parent size / 2), anchor middle
void centerIn(uint32_t w, uint32_t parent) {
    float pw = ldf(vcall(parent, node::getContentSize, {parent}));
    float ph = ldf(vcall(parent, node::getContentSize, {parent}) + 4);
    Frame f;
    uint32_t v = f.alloc(8);
    call(wl::Vec2_ctorXY, {v, fb(pw * 0.5f), fb(ph * 0.5f)});
    vcall(w, node::setPosition, {w, v});
    call(wl::Vec2_dtor, {v});
}
}  // namespace

uint32_t SceneMyPage::init(Addr self) {
    if (call(smain::lib::Layer_init, {self}) != 1) return 0;
    Self *s = S(self);
    {
        // Misc 701: days a banner stays, in seconds
        Frame f;
        uint32_t misc = masterRow(f, Misc_create, 701);
        uint32_t days = sbRead(ld32(misc), 4, 4);
        rw::set<uint32_t>(s, OFF_EXPIRE_SECONDS, days * 86400);
        release(ld32(misc + 4));
    }
    uint32_t dl = call(fn::operator_new, {0x34});
    call(ImageDownloader_ctor, {dl});
    rw::set<uint32_t>(s, OFF_DOWNLOADER, dl);
    st32(dl + 0x18, 3600);
    rw::set<uint8_t>(s, 0x410, 0);
    call(SceneManager_setCanTouchHeader, {call(SceneManager_getInstance, {}), 0});
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t fo = newFunction(f, VT_init_0, {self});
        call(SceneManager_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t path = newStr(f, S_SCENE, 42);
        s->root = call(SceneUtil_createNodeWithSceneFile, {path});
        strFree(path);
    }
    vcall(self, node::addChild, {self, s->root});
    const uint32_t centerBG = findNode(s->root, S_CENTER_UI_BG, 10);
    s->panel6 = findNode(centerBG, S_PANEL_6, 7);
    vcall(s->panel6, 0x29C, {s->panel6, 0});   // setTouchEnabled(false)

    s->centerWidget = call(Widget_create, {});
    centerIn(s->centerWidget, centerBG);
    vcall(s->centerWidget, node::setAnchorPoint, {s->centerWidget, ANCHOR_MIDDLE});
    vcall(centerBG, node::addChildZ, {centerBG, s->centerWidget, 9999});

    s->fieldWidget = findNode(s->panel6, S_IMAGE_DB1, 9);
    if (s->fieldWidget) vcall(s->fieldWidget, node::removeFromParent, {s->fieldWidget});
    s->fieldWidget = call(Widget_create, {});
    centerIn(s->fieldWidget, s->panel6);
    vcall(s->fieldWidget, node::setAnchorPoint, {s->fieldWidget, ANCHOR_MIDDLE});
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fb(1590.0f), fb(896.0f)});
        vcall(s->fieldWidget, node::setContentSize, {s->fieldWidget, size});
    }
    vcall(s->fieldWidget, node::setScale, {s->fieldWidget, 0x3F3851ECu});   // 0.72
    vcall(s->panel6, node::addChild, {s->panel6, s->fieldWidget});

    s->fieldImage = call(ImageView_create, {});
    centerIn(s->fieldImage, s->fieldWidget);
    vcall(s->fieldImage, node::setAnchorPoint, {s->fieldImage, ANCHOR_MIDDLE});
    vcall(s->fieldWidget, node::addChild, {s->fieldWidget, s->fieldImage});
    call(M_initField, {self});

    {
        // walking speeds and the number of avatar columns from the screen width
        Frame f;
        uint32_t vs = f.alloc(8);
        call(fn::Director_getVisibleSize, {vs, call(fn::Director_getInstance, {})});
        float w = ldf(vs);
        stf(self + OFF_SPEED_FAST, w * bf(0x3CC49BA6u));
        stf(self + OFF_SPEED_MID, w * bf(0x3C9374BCu));
        stf(self + OFF_SPEED_SLOW, w * bf(0x3C449BA6u));
        float unit = ldf(self + OFF_WALK_UNIT) * 60.0f;
        double cols = double(std::floor(w / unit)) + 1.0;
        int32_t n = cols != cols ? 0 : cols >= 2147483647.0 ? 0x7FFFFFFF : cols <= -2147483648.0 ? int32_t(0x80000000u)
                                                                                                  : int32_t(cols);
        rw::set<int32_t>(s, OFF_COLUMNS, n);
    }
    {
        uint32_t detailAt = call(APIManager_getInstance, {}) + 0x84;
        uint32_t detail = ld32(detailAt);
        uint8_t inParty = 0;
        if (detail) inParty = sbRead(detail, 0x10, 1) ? 1 : 0;
        rw::set<uint8_t>(s, OFF_HAS_PARTY, inParty);
        call(M_initMovePoints, {self});

        uint32_t shade = findNode(s->root, S_SHADE, 5);
        shiftY(findNode(shade, S_SHADE_TOP, 9), true);
        shiftY(findNode(shade, S_SHADE_BOTTOM, 12), false);
        call(SceneManager_showHeader, {call(SceneManager_getInstance, {})});

        // the ticker: the first notice message
        uint32_t list = ld32(call(APIManager_getInstance, {}) + 0x314);
        uint32_t it = ld32(list), end = ld32(list + 4);
        for (int32_t i = 0; it != end; i--, it += 0x14) {
            Frame f;
            uint32_t entry = f.alloc(0x14);
            st32(entry, ld32(it));
            st32(entry + 4, ld32(it + 4));
            uint32_t msg = entry + 8;
            if (strIsLong(it + 8)) {
                call(fn::string_init, {msg, ld32(it + 0x10), ld32(it + 0xC)});
            } else {
                st32(msg, ld32(it + 8));
                st32(msg + 4, ld32(it + 0xC));
                st32(msg + 8, ld32(it + 0x10));
            }
            if (i == 0) {
                uint32_t sm = call(SceneManager_getInstance, {});
                uint32_t text = strIsLong(msg) ? ld32(msg + 8) : msg + 1;
                uint32_t str = newStr(f, text);
                call(SceneManager_changeTickerMessage, {sm, str});
                strFree(str);
            }
            strFree(msg);
        }
        call(SceneManager_removeBackground, {call(SceneManager_getInstance, {})});
        call(M_initLeftButtons, {self});
        call(M_initRightButtons, {self});
        call(M_updateInfoIcon, {self});

        uint32_t jewel = findNode(s->root, S_JEWEL_BIG, 12);
        vcall(jewel, node::setVisible, {jewel, 0});
        uint32_t wording = findNode(jewel, S_TXT_WORDING1, 12);
        call(SceneUtil_setBlackOutline, {wording});
        setTextId(wording, 101000023);
        uint32_t panel1 = findNode(jewel, S_PANEL_1, 7);
        uint32_t flag = sbRead(ld32(detailAt), 0x13, 1);   // (no null check, as the original)
        vcall(panel1, node::setVisible, {panel1, flag ? 0u : 1u});
    }

    s->rightSolo = findNode(s->root, S_RIGHT_SOLO, 12);
    vcall(s->rightSolo, node::setVisible, {s->rightSolo, 0});
    s->jewelSolo = findNode(s->root, S_JEWEL_SOLO, 13);
    vcall(s->jewelSolo, node::setVisible, {s->jewelSolo, 0});
    s->rightParty = findNode(s->root, S_RIGHT_PARTY, 13);
    vcall(s->rightParty, node::setVisible, {s->rightParty, 0});
    s->centerParty = findNode(s->root, S_CENTER_PARTY, 14);
    vcall(s->centerParty, node::setVisible, {s->centerParty, 0});
    uint32_t rankSolo1 = findNode(s->root, S_RANK_SOLO1, 10);
    vcall(rankSolo1, node::setVisible, {rankSolo1, 0});
    uint32_t rankSolo2 = findNode(s->root, S_RANK_SOLO2, 10);
    vcall(rankSolo2, node::setVisible, {rankSolo2, 0});
    {
        float x = bf(vcall(s->rightSolo, node::getPositionX, {s->rightSolo}));
        stf(self + OFF_SOLO_X, x);
        stf(self + OFF_SOLO_X_OUT, x + -350.0f);
        float x2 = bf(vcall(rankSolo2, node::getPositionX, {rankSolo2}));
        stf(self + OFF_RANK_X, x2);
        stf(self + OFF_RANK_X_OUT, x2 + -350.0f);
    }
    uint32_t party2 = findNode(s->centerParty, S_BUTTON_PARTY2, 13);
    addTouchLambda(party2, VT_init_1, {self, party2});
    shadowText(party2, S_TXT_PARTY, 9, 101000004);
    uint32_t centerRank = findNode(s->centerParty, S_CENTER_RANK, 13);
    uint32_t rank = findNode(centerRank, S_BUTTON_RANK, 11);
    addTouchLambda(rank, VT_init_2, {self, rank});
    shadowText(rank, S_TXT_RANK, 8, 100100020);
    uint32_t rank1 = findNode(rankSolo1, S_BUTTON_RANK, 11);
    addTouchLambda(rank1, VT_init_3, {self, rank});
    shadowText(rank1, S_TXT_RANK, 8, 100100020);
    uint32_t rank2 = findNode(rankSolo2, S_BUTTON_RANK, 11);
    addTouchLambda(rank2, VT_init_4, {self, rank});
    shadowText(rank2, S_TXT_RANK, 8, 100100020);
    uint32_t search = findNode(s->rightSolo, S_BUTTON_SEARCH, 18);
    addTouchLambda(search, VT_init_5, {self, search});
    shadowText(search, S_TXT_SEARCH, 15, 101000021);
    uint32_t search2 = findNode(s->jewelSolo, S_BUTTON_SEARCH, 18);
    addTouchLambda(search2, VT_init_6, {self, search2});
    shadowText(search2, S_TXT_SEARCH, 15, 101000021);
    shiftY(findNode(s->root, S_RIGHT_BANNER, 14), false);
    uint32_t update = findNode(s->root, S_BUTTON_UPDATE, 13);
    addTouchLambda(update, VT_init_7, {self});

    // two comment balloons on the field layer (hidden)
    for (int32_t i = 0; i < 2; i++) {
        uint32_t balloon = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), S_BALOON_JSON});
        float scale = ldf(self + OFF_SCALE) / ldf(self + OFF_SCALE_BASE);
        vcall(balloon, node::setScale, {balloon, fb(scale)});
        vcall(balloon, node::setVisible, {balloon, 0});
        uint32_t layer = s->fieldLayer;
        Frame f;
        uint32_t buf = stringBuffer(f, {S_FUKIDASHI, uint32_t(i)});
        uint32_t name = newStr(f, buf);
        vcall(layer, node::addChildZName, {layer, balloon, 9999, name});
        strFree(name);
        call(WidgetVector_pushBack, {self + OFF_BALOONS, s->fieldLayer});
        uint32_t no = f.alloc(4);
        st8(no, 0);
        call(BoolVector_pushBack, {self + OFF_BALOON_SHOWN, no});
    }
    call(M_initPartyMembers, {self});
    {
        // an invisible button over the field: taps on the avatars ($_8)
        Frame f;
        uint32_t empty = newStr(f, S_EMPTY, 0);
        uint32_t button = call(ImageButton_create, {empty, 0});
        strFree(empty);
        call(ImageButton_setTargetView, {button, s->fieldLayer, fb(1.0f)});
        vcall(centerBG, node::addChild, {centerBG, button});
        uint32_t fo = newFunction(f, VT_init_8, {self});
        call(ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    {
        // field touches: began $_9, moved $_10, ended / cancelled $_11
        uint32_t listener = call(smain::lib::EventListenerTouchOneByOne_create, {});
        call(smain::lib::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
        Frame f;
        uint32_t fo = f.alloc(24);
        st32(fo + 16, fo);
        st32(fo, VT_init_9);
        st32(fo + 4, self);
        call(wl::FunctionTouchB_swap, {fo, listener + 0x50});
        functionDestroy(fo);
        st32(fo + 16, fo);
        st32(fo, VT_init_10);
        st32(fo + 4, self);
        call(wl::FunctionTouchV_swap, {fo, listener + 0x68});
        functionDestroy(fo);
        uint32_t lambda = f.alloc(4);
        st32(lambda, self);
        call(0x63E3AD, {listener + 0x80, lambda});   // sub_63E3AC
        call(0x63E3AD, {listener + 0x98, lambda});
        uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + smain::lib::Director_eventDispatcher);
        call(smain::lib::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s->fieldLayer});
    }
    vcall(self, 0x300, {self});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_12, {self});
        call(smain::lib::FunctionVV_assign, {self + 0x210, fo});   // onEnter
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_13, {self});
        call(smain::lib::FunctionVV_assign, {self + 0x228, fo});   // onExit
        functionDestroy(fo);
    }
    call(M_initBannerArea, {self});
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t fo = newFunction(f, VT_init_14, {self});
        call(SceneManager_setOnChangeProfile, {sm, fo});
        functionDestroy(fo);
    }
    return 1;
}

RW_FUNCTION(0x48584C, "SceneMyPage::SceneMyPage", SceneMyPage::construct);
RW_FUNCTION(0x6397FC, "SceneMyPage::~SceneMyPage", SceneMyPage::destroy);
RW_FUNCTION(0x4857F8, "SceneMyPage::create", SceneMyPage::create);
RW_FUNCTION(0x6397D8, "SceneMyPage::createScene", SceneMyPage::createScene);
RW_FUNCTION(0x639A4C, "SceneMyPage::cancelShopHandle", SceneMyPage::cancelShopHandle);
RW_FUNCTION(0x639AE0, "SceneMyPage::init", SceneMyPage::init);
RW_FUNCTION(0x63E3AC, "SceneMyPage::init::sub_63E3AC (function = $_11)", SceneMyPage::assignTouchEnded);
