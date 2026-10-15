// SceneTakeOver: the std::function slots of its lambdas, generated from the binary's __func vtables
// (each thunk's shape was checked by emulating the original instructions). The lambda bodies
// (operator()) are written by hand.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneTakeOver_w31_thunks {

// SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ (captures a std::function at +8, copied with _ZNSt3__18functionIFvPN7cocos2d4NodeEEEC2ERKS5_)
using openWebViewInfoPopup_9_UlvE_ = thunk::LambdaFn<0xF6B170, 48, 0xFA45A8, 0xE8F650, 8, 8, 0x474D99, thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4)>;
// SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_
using openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ = thunk::Lambda<0xF6B1F4, 8, 0xFA45E4, 0xE8FB00, 4>;
// SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_
using init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ = thunk::Lambda<0xF6B278, 8, 0xFA4620, 0xE8FF00, 4>;

}  // namespace SceneTakeOver_w31_thunks

RW_FUNCTION(0x7136F0, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ ~__func()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::destructor);
RW_FUNCTION(0x71372C, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ ~__func() (deleting)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::destructorDelete);
RW_FUNCTION(0x71376C, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ __clone()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::clone);
RW_FUNCTION(0x7137C8, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ __clone(__base*)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::cloneInto);
RW_FUNCTION(0x71380C, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ destroy()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::destroy);
RW_FUNCTION(0x71382C, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ destroy_deallocate()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::destroyDeallocate);
RW_FUNCTION(0x7138A0, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ target(const type_info&)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::target);
RW_FUNCTION(0x7138C0, "SceneTakeOver::openWebViewInfoPopup::$_9 UlvE_ target_type()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_9_UlvE_::targetType);
RW_FUNCTION(0x713BB4, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ ~__func() (deleting)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::deleteThis);
RW_FUNCTION(0x713BBC, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ __clone()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::clone);
RW_FUNCTION(0x713BE0, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ __clone(__base*)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::cloneInto);
RW_FUNCTION(0x713BFC, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ destroy()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::destroy);
RW_FUNCTION(0x713BFE, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ destroy_deallocate()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::deleteThis);
RW_FUNCTION(0x713C1C, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ target(const type_info&)", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::target);
RW_FUNCTION(0x713C3C, "SceneTakeOver::openWebViewInfoPopup::$_7 UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_ target_type()", SceneTakeOver_w31_thunks::openWebViewInfoPopup_7_UlPNS3_3RefENS3_2ui6Widget14TouchEventTypeEE_::targetType);
RW_FUNCTION(0x713E5C, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ ~__func() (deleting)", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::deleteThis);
RW_FUNCTION(0x713E64, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ __clone()", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::clone);
RW_FUNCTION(0x713E88, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ __clone(__base*)", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::cloneInto);
RW_FUNCTION(0x713EA4, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ destroy()", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::destroy);
RW_FUNCTION(0x713EA6, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ destroy_deallocate()", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::deleteThis);
RW_FUNCTION(0x713EEC, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ target(const type_info&)", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::target);
RW_FUNCTION(0x713F0C, "SceneTakeOver::init::$_5 UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_ target_type()", SceneTakeOver_w31_thunks::init_5_UlbN10BGFacebook15FacebookManager9ErrorCodeENS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEE_::targetType);
