// SceneManager HeaderBuild: static member declarations (included inside the struct)
    // ---- header / global menu / title header construction (SceneManagerHeaderBuild.cpp)
    static Addr createHeader(SceneManager *self);                    // returns the header root node
    static void initGlobalMenu(SceneManager *self);
    static bool closeGlobalMenu(SceneManager *self);                 // false: the menu was not open
    static Addr createTitleHeader(SceneManager *self, uint32_t main, uint32_t sub1, uint32_t sub2);
