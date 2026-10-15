// SceneManager HeaderUpdate: static member declarations (included inside the struct)
    // ---- per-frame update, header contents, touch effects (SceneManagerHeaderUpdate.cpp)
    static void update(SceneManager *self, float dt);
    static void updateHeaderAp(SceneManager *self);
    static void changeTickerMessage(SceneManager *self, Addr message);       // const std::string&
    static void updateHeader(SceneManager *self);
    static void updateHeaderAvatarIcon(SceneManager *self, Addr node, int32_t avatarId);
    static void generateTouchEffect(SceneManager *self, Addr position);      // Vec2 by value (pointer)
    static void generateTouchMoveEffect(SceneManager *self, Addr position);
