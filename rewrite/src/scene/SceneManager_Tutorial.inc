// SceneManager Tutorial: static member declarations (included inside the struct)
    // ---- tutorial checks and chains run from the menus (SceneManagerTutorial.cpp)
    static bool isTutorial12(SceneManager *self);
    static bool isTutorial15(SceneManager *self);
    static bool isTutorial36(SceneManager *self);
    static bool isTutorial18(SceneManager *self);
    static void Tutorial12Exec(SceneManager *self, Addr callback);   // callback: std::function<void()> by value
    static bool ChainOfTutorialShop(SceneManager *self, uint32_t id);
    static void Tutorial15Exec(SceneManager *self);
    static bool ChainOfTutorialDeckEdit(SceneManager *self, uint32_t id);
    static void Tutorial18Exec(SceneManager *self, Addr callback);
    static void tutorial_menu(SceneManager *self);
    static bool ChainOfTutorialAdventureSelect(SceneManager *self, uint32_t id);
    static bool ChainOfTutorialSphere(SceneManager *self, uint32_t id);

    // ---- misc (SceneManagerTutorial.cpp)
    static void openNoTapScreen(SceneManager *self, float delay);
    static void callCurrentSceneRunOutAnimation(SceneManager *self, Addr callback);
    static void addFaceImageUrl(SceneManager *self, Addr key, Addr url);
    static void getFaceImageUrl(Addr result, SceneManager *self, Addr key);
    static void destroyDelete(SceneManager *self);                   // D0
