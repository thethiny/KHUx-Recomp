// SceneSellOutMaterialItem: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace SceneSellOutMaterialItem_g5thunks {

// SceneSellOutMaterialItem::backScene::$_22
using backScene_22 = thunk::Lambda<0xF64944, 8, 0xF9C87C, 0xE70BB0, 4>;
// SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda()
using openSellCheckDialog_24_lv = thunk::Lambda<0xF6499C, 16, 0xF9C8A4, 0xE70D10, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool)
using openSellCheckDialog_17_lb = thunk::Lambda<0xF649F4, 8, 0xF9C8CC, 0xE70E90, 4>;
// SceneSellOutMaterialItem::openSellCheckDialog::$_20
using openSellCheckDialog_20 = thunk::Lambda<0xF64A20, 8, 0xF9C8E0, 0xE70F50, 4, thunk::cap(4, 4)>;
// SceneSellOutMaterialItem::openSellCheckDialog::$_19
using openSellCheckDialog_19 = thunk::Lambda<0xF64A4C, 8, 0xF9C8F4, 0xE71030, 4>;
// SceneSellOutMaterialItem::openSellCheckDialog::$_18
using openSellCheckDialog_18 = thunk::Lambda<0xF64A78, 8, 0xF9C908, 0xE71110, 4>;
// SceneSellOutMaterialItem::openSellDetailPopup::$_16
using openSellDetailPopup_16 = thunk::Lambda<0xF64AA4, 40, 0xF9C91C, 0xE711C0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;

}  // namespace SceneSellOutMaterialItem_g5thunks

RW_FUNCTION(0x688BD8, "SceneSellOutMaterialItem::backScene::$_22 ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::backScene_22::deleteThis);
RW_FUNCTION(0x688BE0, "SceneSellOutMaterialItem::backScene::$_22 __clone()", SceneSellOutMaterialItem_g5thunks::backScene_22::clone);
RW_FUNCTION(0x688C04, "SceneSellOutMaterialItem::backScene::$_22 __clone(__base*)", SceneSellOutMaterialItem_g5thunks::backScene_22::cloneInto);
RW_FUNCTION(0x688C20, "SceneSellOutMaterialItem::backScene::$_22 destroy()", SceneSellOutMaterialItem_g5thunks::backScene_22::destroy);
RW_FUNCTION(0x688C22, "SceneSellOutMaterialItem::backScene::$_22 destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::backScene_22::deleteThis);
RW_FUNCTION(0x688CD4, "SceneSellOutMaterialItem::backScene::$_22 target(const type_info&)", SceneSellOutMaterialItem_g5thunks::backScene_22::target);
RW_FUNCTION(0x688CF4, "SceneSellOutMaterialItem::backScene::$_22 target_type()", SceneSellOutMaterialItem_g5thunks::backScene_22::targetType);
RW_FUNCTION(0x689080, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::deleteThis);
RW_FUNCTION(0x689088, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() __clone()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::clone);
RW_FUNCTION(0x6890B8, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::cloneInto);
RW_FUNCTION(0x6890E0, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() destroy()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::destroy);
RW_FUNCTION(0x6890E2, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::deleteThis);
RW_FUNCTION(0x689114, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() target(const type_info&)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::target);
RW_FUNCTION(0x689134, "SceneSellOutMaterialItem::openSellCheckDialog::$_24::lambda() target_type()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_24_lv::targetType);
RW_FUNCTION(0x689F50, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::deleteThis);
RW_FUNCTION(0x689F58, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) __clone()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::clone);
RW_FUNCTION(0x689F7C, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::cloneInto);
RW_FUNCTION(0x689F98, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) destroy()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::destroy);
RW_FUNCTION(0x689F9A, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::deleteThis);
RW_FUNCTION(0x689FB0, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) target(const type_info&)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::target);
RW_FUNCTION(0x689FD0, "SceneSellOutMaterialItem::openSellCheckDialog::$_17::lambda(bool) target_type()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_17_lb::targetType);
RW_FUNCTION(0x689FE4, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::deleteThis);
RW_FUNCTION(0x689FEC, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 __clone()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::clone);
RW_FUNCTION(0x68A018, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::cloneInto);
RW_FUNCTION(0x68A038, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 destroy()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::destroy);
RW_FUNCTION(0x68A03A, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::deleteThis);
RW_FUNCTION(0x68A04C, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 target(const type_info&)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::target);
RW_FUNCTION(0x68A06C, "SceneSellOutMaterialItem::openSellCheckDialog::$_20 target_type()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_20::targetType);
RW_FUNCTION(0x68A080, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::deleteThis);
RW_FUNCTION(0x68A088, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 __clone()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::clone);
RW_FUNCTION(0x68A0AC, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::cloneInto);
RW_FUNCTION(0x68A0C8, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 destroy()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::destroy);
RW_FUNCTION(0x68A0CA, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::deleteThis);
RW_FUNCTION(0x68A0E8, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 target(const type_info&)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::target);
RW_FUNCTION(0x68A108, "SceneSellOutMaterialItem::openSellCheckDialog::$_19 target_type()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_19::targetType);
RW_FUNCTION(0x68A11C, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::deleteThis);
RW_FUNCTION(0x68A124, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 __clone()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::clone);
RW_FUNCTION(0x68A148, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::cloneInto);
RW_FUNCTION(0x68A164, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 destroy()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::destroy);
RW_FUNCTION(0x68A166, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::deleteThis);
RW_FUNCTION(0x68A184, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 target(const type_info&)", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::target);
RW_FUNCTION(0x68A1A4, "SceneSellOutMaterialItem::openSellCheckDialog::$_18 target_type()", SceneSellOutMaterialItem_g5thunks::openSellCheckDialog_18::targetType);
RW_FUNCTION(0x68A1B8, "SceneSellOutMaterialItem::openSellDetailPopup::$_16 ~__func() (deleting)", SceneSellOutMaterialItem_g5thunks::openSellDetailPopup_16::deleteThis);
RW_FUNCTION(0x68A1C0, "SceneSellOutMaterialItem::openSellDetailPopup::$_16 __clone()", SceneSellOutMaterialItem_g5thunks::openSellDetailPopup_16::clone);
RW_FUNCTION(0x68A200, "SceneSellOutMaterialItem::openSellDetailPopup::$_16 __clone(__base*)", SceneSellOutMaterialItem_g5thunks::openSellDetailPopup_16::cloneInto);
RW_FUNCTION(0x68A234, "SceneSellOutMaterialItem::openSellDetailPopup::$_16 destroy()", SceneSellOutMaterialItem_g5thunks::openSellDetailPopup_16::destroy);
RW_FUNCTION(0x68A236, "SceneSellOutMaterialItem::openSellDetailPopup::$_16 destroy_deallocate()", SceneSellOutMaterialItem_g5thunks::openSellDetailPopup_16::deleteThis);
