// SceneManager Chat: static member declarations (included inside the struct)
    // ---- chat (SceneManagerChat.cpp, SceneManagerChat2.cpp); messages = std::vector<ChatManager::Message>*
    static void chatDataUpdate(SceneManager *self, float dt);
    static void openChatPopup(SceneManager *self);
    static void openChatStampListPopup(SceneManager *self);
    static void chatWindowUpdate(SceneManager *self, int32_t lines);
    // sret members: out = the returned std::string, then this
    static void chatNewLineUpdate(Addr out, SceneManager *self, Addr text, int32_t maxWidth, int32_t maxLines);
    static void chatLineUpdate(Addr out, SceneManager *self, Addr text, int32_t maxChars);
    static void getStringHtmlEncodeUtf(Addr out, SceneManager *self, Addr text);
    static bool chatUserBlockCheck(SceneManager *self, Addr userId);
    // the next three do not use `this` (callers pass unrelated values in r0)
    static bool getChatSpMessageCheck(SceneManager *self, Addr message);
    static bool getChatStampMessageCheck(SceneManager *self, Addr text);
    static int32_t getChatStampId(SceneManager *self, Addr text);
    static void setChatMessageScrollView(SceneManager *self);
    static void setChatMessageScrollViewFrom(SceneManager *self, Addr messages);
    static bool setChatMessageUpdate(SceneManager *self, Addr messages);
