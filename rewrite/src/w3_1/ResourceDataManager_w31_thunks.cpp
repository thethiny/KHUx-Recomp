// ResourceDataManager: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace ResourceDataManager_w31_thunks {

// ResourceDataManager::startNextDownloadThread::$_2
using startNextDownloadThread_2 = thunk::Lambda<0xF6EC20, 8, 0xFAA8B4, 0xEA7F80, 4, thunk::cap(4, 4)>;

}  // namespace ResourceDataManager_w31_thunks

RW_FUNCTION(0x7754D8, "ResourceDataManager::startNextDownloadThread::$_2 ~__func()", ResourceDataManager_w31_thunks::startNextDownloadThread_2::destructor);
RW_FUNCTION(0x7754DA, "ResourceDataManager::startNextDownloadThread::$_2 ~__func() (deleting)", ResourceDataManager_w31_thunks::startNextDownloadThread_2::deleteThis);
RW_FUNCTION(0x7754E4, "ResourceDataManager::startNextDownloadThread::$_2 __clone()", ResourceDataManager_w31_thunks::startNextDownloadThread_2::clone);
RW_FUNCTION(0x775510, "ResourceDataManager::startNextDownloadThread::$_2 __clone(__base*)", ResourceDataManager_w31_thunks::startNextDownloadThread_2::cloneInto);
RW_FUNCTION(0x775530, "ResourceDataManager::startNextDownloadThread::$_2 destroy()", ResourceDataManager_w31_thunks::startNextDownloadThread_2::destroy);
RW_FUNCTION(0x775532, "ResourceDataManager::startNextDownloadThread::$_2 destroy_deallocate()", ResourceDataManager_w31_thunks::startNextDownloadThread_2::deleteThis);
RW_FUNCTION(0x775560, "ResourceDataManager::startNextDownloadThread::$_2 target(const type_info&)", ResourceDataManager_w31_thunks::startNextDownloadThread_2::target);
RW_FUNCTION(0x775580, "ResourceDataManager::startNextDownloadThread::$_2 target_type()", ResourceDataManager_w31_thunks::startNextDownloadThread_2::targetType);
