// SceneUtil MyPage: static member declarations (included inside the struct)
    // ---- MyPage's status panels (SceneUtilMyPage.cpp)
    // HP (UserPoint, shared_ptr*), attack power and defense of the deck, with their labels.
    static void setMyPageDeckStatus(Addr root, Addr userPoint, int32_t power, int32_t defense);
    // The player panel: union emblem, party name, player name and rank, crowns (with showCrowns), the title plate
    // and the Facebook photo (user, detail = shared_ptr*, party = Party&, downloader = ImageDownloader*).
    static void setMyPageTeamStatus(Addr root, Addr user, Addr detail, Addr party, Addr downloader,
                                    uint32_t showCrowns);
