// SceneManager ChatPopups: static member declarations (included inside the struct)
    // ---- chat post / report / block popups (SceneManagerChatPopups.cpp)
    // message = ChatScrollView::ChatMessage* (passed by value: a pointer to the caller's copy, destroyed by the
    // caller); callback = std::function<void()>* / std::function<void(int)>*; block = ChatBlockListScrollView::ChatBlock*
    static void openDeletePostPopup(SceneManager *self, Addr message);
    static void openCheckPostPopup(SceneManager *self, Addr message);
    static void openBlockPostPopup(SceneManager *self, Addr message, Addr callback);
    static void openReportPopup(SceneManager *self, Addr message, Addr callback);
    static void openReportSelectPopup(SceneManager *self, int32_t selected, Addr callback);
    static void openBlockListPopup(SceneManager *self);
    static void openUnBlockPostPopup(SceneManager *self, Addr block, Addr callback);
