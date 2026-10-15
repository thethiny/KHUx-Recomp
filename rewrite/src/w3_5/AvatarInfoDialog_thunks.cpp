// AvatarInfoDialog: the mechanical std::function slots of its lambdas (heap / placement clone, destroy, delete, target,
// target_type), generated from the binary's __func vtables and checked against the original instructions by
// emulating them (same generator as scene_main/*_thunks.cpp). The lambda bodies (operator()) are written by hand
// next to the functions that create them.
#include "../scene_main/FunctionThunks.hpp"

namespace AvatarInfoDialog_g5thunks {

// AvatarInfoDialog::openPopUpReject::$_71 (captures a std::function at +16, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpReject_71 = thunk::LambdaFn<0xF57498, 40, 0xF86C0C, 0xE28AB0, 8, 16, 0x45C84D, thunk::cap(8, 4)>;
// AvatarInfoDialog::openPopUpReject::$_70 (captures a std::function at +8, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpReject_70 = thunk::LambdaFn<0xF574C4, 32, 0xF86C20, 0xE28B80, 8, 8, 0x45C84D>;
// AvatarInfoDialog::openPopUpReject::$_70::lambda(bool)
using openPopUpReject_70_lb = thunk::Lambda<0xF574F0, 8, 0xF86C34, 0xE28C50, 4>;
// AvatarInfoDialog::openPopUpInvitation::$_69 (captures a std::function at +16, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpInvitation_69 = thunk::LambdaFn<0xF5751C, 40, 0xF86C48, 0xE28D30, 8, 16, 0x45C84D, thunk::cap(8, 4)>;
// AvatarInfoDialog::openPopUpInvitation::$_68 (captures a std::function at +8, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpInvitation_68 = thunk::LambdaFn<0xF57548, 32, 0xF86C5C, 0xE28E00, 8, 8, 0x45C84D>;
// AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool)
using openPopUpInvitation_68_lb = thunk::Lambda<0xF57574, 8, 0xF86C70, 0xE28EE0, 4>;
// AvatarInfoDialog::openPopUpCancel::$_67 (captures a std::function at +16, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpCancel_67 = thunk::LambdaFn<0xF575A0, 40, 0xF86C84, 0xE28FC0, 8, 16, 0x45C84D, thunk::cap(8, 4)>;
// AvatarInfoDialog::openPopUpComment::$_25 (captures a std::function at +48, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpComment_25 = thunk::LambdaFn<0xF580CC, 72, 0xF87198, 0xE2CD00, 8, 48, 0x45C84D, thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4)>;
// AvatarInfoDialog::openPopUpComment::$_23::lambda()
using openPopUpComment_23_lv = thunk::Lambda<0xF580F8, 16, 0xF871AC, 0xE2CDE0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// AvatarInfoDialog::openPopUpComment::$_24
using openPopUpComment_24 = thunk::Lambda<0xF58124, 8, 0xF871C0, 0xE2CEC0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::openPopUpComment::$_22
using openPopUpComment_22 = thunk::Lambda<0xF58150, 8, 0xF871D4, 0xE2CFC0, 4>;
// AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)
using openPopUpComment_21_lb = thunk::Lambda<0xF581A8, 16, 0xF871FC, 0xE2D1C0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool)
using openPopUpComment_21_lb_lb = thunk::Lambda<0xF581D4, 8, 0xF87210, 0xE2D300, 4>;
// AvatarInfoDialog::openPopUpSubName::$_19
using openPopUpSubName_19 = thunk::Lambda<0xF58200, 40, 0xF87224, 0xE2D440, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_19::lambda(int)
using openPopUpSubName_19_li = thunk::Lambda<0xF5822C, 40, 0xF87238, 0xE2D540, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_18
using openPopUpSubName_18 = thunk::Lambda<0xF58258, 40, 0xF8724C, 0xE2D670, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_18::lambda(int)
using openPopUpSubName_18_li = thunk::Lambda<0xF58284, 40, 0xF87260, 0xE2D770, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_17
using openPopUpSubName_17 = thunk::Lambda<0xF582B0, 40, 0xF87274, 0xE2D8A0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_14::lambda()
using openPopUpSubName_14_lv = thunk::Lambda<0xF58308, 16, 0xF8729C, 0xE2DA50, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) (captures a std::function at +8, copied with _ZNSt3__18functionIFvbEEC2ERKS2_)
using openPopUpSubName_16_lb = thunk::LambdaFn<0xF58334, 32, 0xF872B0, 0xE2DB40, 8, 8, 0x45C84D>;
// AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool)
using openPopUpSubName_16_lb_lb = thunk::Lambda<0xF58360, 8, 0xF872C4, 0xE2DC30, 4>;
// AvatarInfoDialog::openPopUpSubName::$_15
using openPopUpSubName_15 = thunk::Lambda<0xF5838C, 8, 0xF872D8, 0xE2DD10, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::openPopUpSubName::$_13
using openPopUpSubName_13 = thunk::Lambda<0xF583B8, 8, 0xF872EC, 0xE2DE10, 4>;
// AvatarInfoDialog::close::$_12
using close_12 = thunk::Lambda<0xF583E4, 8, 0xF87300, 0xE2DEC0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_11
using createLayout_11 = thunk::Lambda<0xF58410, 8, 0xF87314, 0xE2DF80, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_10
using createLayout_10 = thunk::Lambda<0xF5843C, 8, 0xF87328, 0xE2E040, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_9
using createLayout_9 = thunk::Lambda<0xF58468, 12, 0xF8733C, 0xE2E0D0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3
using createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3 = thunk::Lambda<0xF584C0, 8, 0xF87364, 0xE2E260, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3
using createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_ = thunk::Lambda<0xF584EC, 8, 0xF87378, 0xE2E3D0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3
using createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__ = thunk::Lambda<0xF58518, 8, 0xF8738C, 0xE2E5C0, 4>;
// AvatarInfoDialog::createLayout::$_8::lambda(float,int)
using createLayout_8_lfi = thunk::Lambda<0xF58544, 12, 0xF873A0, 0xE2E740, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi)
using createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi = thunk::Lambda<0xF58570, 8, 0xF873B4, 0xE2E840, 4>;
// AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2
using createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2 = thunk::Lambda<0xF5859C, 40, 0xF873C8, 0xE2E960, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(bool)
using createLayout_8_lb = thunk::Lambda<0xF585C8, 36, 0xF873DC, 0xE2EA60, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)
using createLayout_8_lb_lb = thunk::Lambda<0xF585F4, 8, 0xF873F0, 0xE2EB40, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool)
using createLayout_8_lb_lb_lb = thunk::Lambda<0xF58620, 8, 0xF87404, 0xE2EC40, 4>;
// AvatarInfoDialog::createLayout::$_73
using createLayout_73 = thunk::Lambda<0xF5864C, 32, 0xF87418, 0xE2ED10, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)
using createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE = thunk::Lambda<0xF58678, 12, 0xF8742C, 0xE2EDF0, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
// AvatarInfoDialog::createLayout::$_7
using createLayout_7 = thunk::Lambda<0xF586A4, 8, 0xF87440, 0xE2EEE0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_6
using createLayout_6 = thunk::Lambda<0xF586D0, 56, 0xF87454, 0xE2EFA0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4), thunk::cap(44, 4), thunk::cap(48, 4), thunk::cap(52, 4)>;
// AvatarInfoDialog::createLayout::$_6::lambda(bool)
using createLayout_6_lb = thunk::Lambda<0xF586FC, 8, 0xF87468, 0xE2F070, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda()
using createLayout_6_lb_lv = thunk::Lambda<0xF58728, 8, 0xF8747C, 0xE2F190, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool)
using createLayout_6_lb_lb = thunk::Lambda<0xF58754, 8, 0xF87490, 0xE2F2C0, 4>;
// AvatarInfoDialog::createLayout::$_6::lambda()
using createLayout_6_lv = thunk::Lambda<0xF58780, 8, 0xF874A4, 0xE2F3E0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_5
using createLayout_5 = thunk::Lambda<0xF587AC, 56, 0xF874B8, 0xE2F4E0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4), thunk::cap(44, 4), thunk::cap(48, 4), thunk::cap(52, 4)>;
// AvatarInfoDialog::createLayout::$_72
using createLayout_72 = thunk::Lambda<0xF58804, 32, 0xF874E0, 0xE2F630, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4)>;
// AvatarInfoDialog::createLayout::$_3::lambda()
using createLayout_3_lv = thunk::Lambda<0xF5885C, 8, 0xF87508, 0xE2F7C0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)
using createLayout_3_lv_lb = thunk::Lambda<0xF58888, 8, 0xF8751C, 0xE2F8E0, 4, thunk::cap(4, 4)>;
// AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool)
using createLayout_3_lv_lb_lb = thunk::Lambda<0xF588B4, 8, 0xF87530, 0xE2FA20, 4>;
// AvatarInfoDialog::createLayout::$_2
using createLayout_2 = thunk::Lambda<0xF588E0, 56, 0xF87544, 0xE2FB30, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4), thunk::cap(16, 4), thunk::cap(20, 4), thunk::cap(24, 4), thunk::cap(28, 4), thunk::cap(32, 4), thunk::cap(36, 4), thunk::cap(40, 4), thunk::cap(44, 4), thunk::cap(48, 4), thunk::cap(52, 4)>;

}  // namespace AvatarInfoDialog_g5thunks

RW_FUNCTION(0x53C8E4, "AvatarInfoDialog::openPopUpReject::$_71 ~__func()", AvatarInfoDialog_g5thunks::openPopUpReject_71::destructor);
RW_FUNCTION(0x53C920, "AvatarInfoDialog::openPopUpReject::$_71 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpReject_71::destructorDelete);
RW_FUNCTION(0x53C960, "AvatarInfoDialog::openPopUpReject::$_71 __clone()", AvatarInfoDialog_g5thunks::openPopUpReject_71::clone);
RW_FUNCTION(0x53C9B8, "AvatarInfoDialog::openPopUpReject::$_71 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpReject_71::cloneInto);
RW_FUNCTION(0x53C9F8, "AvatarInfoDialog::openPopUpReject::$_71 destroy()", AvatarInfoDialog_g5thunks::openPopUpReject_71::destroy);
RW_FUNCTION(0x53CA18, "AvatarInfoDialog::openPopUpReject::$_71 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpReject_71::destroyDeallocate);
RW_FUNCTION(0x53CB74, "AvatarInfoDialog::openPopUpReject::$_71 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpReject_71::target);
RW_FUNCTION(0x53CB98, "AvatarInfoDialog::openPopUpReject::$_71 target_type()", AvatarInfoDialog_g5thunks::openPopUpReject_71::targetType);
RW_FUNCTION(0x53CBAC, "AvatarInfoDialog::openPopUpReject::$_70 ~__func()", AvatarInfoDialog_g5thunks::openPopUpReject_70::destructor);
RW_FUNCTION(0x53CBE8, "AvatarInfoDialog::openPopUpReject::$_70 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpReject_70::destructorDelete);
RW_FUNCTION(0x53CC28, "AvatarInfoDialog::openPopUpReject::$_70 __clone()", AvatarInfoDialog_g5thunks::openPopUpReject_70::clone);
RW_FUNCTION(0x53CC78, "AvatarInfoDialog::openPopUpReject::$_70 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpReject_70::cloneInto);
RW_FUNCTION(0x53CCAC, "AvatarInfoDialog::openPopUpReject::$_70 destroy()", AvatarInfoDialog_g5thunks::openPopUpReject_70::destroy);
RW_FUNCTION(0x53CCCC, "AvatarInfoDialog::openPopUpReject::$_70 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpReject_70::destroyDeallocate);
RW_FUNCTION(0x53CE6C, "AvatarInfoDialog::openPopUpReject::$_70 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpReject_70::target);
RW_FUNCTION(0x53CE8C, "AvatarInfoDialog::openPopUpReject::$_70 target_type()", AvatarInfoDialog_g5thunks::openPopUpReject_70::targetType);
RW_FUNCTION(0x53CEA0, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::deleteThis);
RW_FUNCTION(0x53CEA8, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::clone);
RW_FUNCTION(0x53CECC, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::cloneInto);
RW_FUNCTION(0x53CEE8, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::destroy);
RW_FUNCTION(0x53CEEA, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::deleteThis);
RW_FUNCTION(0x53CEF4, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::target);
RW_FUNCTION(0x53CF14, "AvatarInfoDialog::openPopUpReject::$_70::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpReject_70_lb::targetType);
RW_FUNCTION(0x53CF28, "AvatarInfoDialog::openPopUpInvitation::$_69 ~__func()", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::destructor);
RW_FUNCTION(0x53CF64, "AvatarInfoDialog::openPopUpInvitation::$_69 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::destructorDelete);
RW_FUNCTION(0x53CFA4, "AvatarInfoDialog::openPopUpInvitation::$_69 __clone()", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::clone);
RW_FUNCTION(0x53CFFC, "AvatarInfoDialog::openPopUpInvitation::$_69 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::cloneInto);
RW_FUNCTION(0x53D03C, "AvatarInfoDialog::openPopUpInvitation::$_69 destroy()", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::destroy);
RW_FUNCTION(0x53D05C, "AvatarInfoDialog::openPopUpInvitation::$_69 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::destroyDeallocate);
RW_FUNCTION(0x53D1B8, "AvatarInfoDialog::openPopUpInvitation::$_69 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::target);
RW_FUNCTION(0x53D1DC, "AvatarInfoDialog::openPopUpInvitation::$_69 target_type()", AvatarInfoDialog_g5thunks::openPopUpInvitation_69::targetType);
RW_FUNCTION(0x53D1F0, "AvatarInfoDialog::openPopUpInvitation::$_68 ~__func()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::destructor);
RW_FUNCTION(0x53D22C, "AvatarInfoDialog::openPopUpInvitation::$_68 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::destructorDelete);
RW_FUNCTION(0x53D26C, "AvatarInfoDialog::openPopUpInvitation::$_68 __clone()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::clone);
RW_FUNCTION(0x53D2BC, "AvatarInfoDialog::openPopUpInvitation::$_68 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::cloneInto);
RW_FUNCTION(0x53D2F0, "AvatarInfoDialog::openPopUpInvitation::$_68 destroy()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::destroy);
RW_FUNCTION(0x53D310, "AvatarInfoDialog::openPopUpInvitation::$_68 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::destroyDeallocate);
RW_FUNCTION(0x53D4B0, "AvatarInfoDialog::openPopUpInvitation::$_68 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::target);
RW_FUNCTION(0x53D4D0, "AvatarInfoDialog::openPopUpInvitation::$_68 target_type()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68::targetType);
RW_FUNCTION(0x53D4E4, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::deleteThis);
RW_FUNCTION(0x53D4EC, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::clone);
RW_FUNCTION(0x53D510, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::cloneInto);
RW_FUNCTION(0x53D52C, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::destroy);
RW_FUNCTION(0x53D52E, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::deleteThis);
RW_FUNCTION(0x53D538, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::target);
RW_FUNCTION(0x53D558, "AvatarInfoDialog::openPopUpInvitation::$_68::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpInvitation_68_lb::targetType);
RW_FUNCTION(0x53D56C, "AvatarInfoDialog::openPopUpCancel::$_67 ~__func()", AvatarInfoDialog_g5thunks::openPopUpCancel_67::destructor);
RW_FUNCTION(0x53D5A8, "AvatarInfoDialog::openPopUpCancel::$_67 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpCancel_67::destructorDelete);
RW_FUNCTION(0x53D5E8, "AvatarInfoDialog::openPopUpCancel::$_67 __clone()", AvatarInfoDialog_g5thunks::openPopUpCancel_67::clone);
RW_FUNCTION(0x545120, "AvatarInfoDialog::openPopUpComment::$_25 target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_25::targetType);
RW_FUNCTION(0x545134, "AvatarInfoDialog::openPopUpComment::$_23::lambda() ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::deleteThis);
RW_FUNCTION(0x54513C, "AvatarInfoDialog::openPopUpComment::$_23::lambda() __clone()", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::clone);
RW_FUNCTION(0x54516C, "AvatarInfoDialog::openPopUpComment::$_23::lambda() __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::cloneInto);
RW_FUNCTION(0x545194, "AvatarInfoDialog::openPopUpComment::$_23::lambda() destroy()", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::destroy);
RW_FUNCTION(0x545196, "AvatarInfoDialog::openPopUpComment::$_23::lambda() destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::deleteThis);
RW_FUNCTION(0x5451C8, "AvatarInfoDialog::openPopUpComment::$_23::lambda() target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::target);
RW_FUNCTION(0x5451E8, "AvatarInfoDialog::openPopUpComment::$_23::lambda() target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_23_lv::targetType);
RW_FUNCTION(0x5451FC, "AvatarInfoDialog::openPopUpComment::$_24 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpComment_24::deleteThis);
RW_FUNCTION(0x545204, "AvatarInfoDialog::openPopUpComment::$_24 __clone()", AvatarInfoDialog_g5thunks::openPopUpComment_24::clone);
RW_FUNCTION(0x545230, "AvatarInfoDialog::openPopUpComment::$_24 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpComment_24::cloneInto);
RW_FUNCTION(0x545250, "AvatarInfoDialog::openPopUpComment::$_24 destroy()", AvatarInfoDialog_g5thunks::openPopUpComment_24::destroy);
RW_FUNCTION(0x545252, "AvatarInfoDialog::openPopUpComment::$_24 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpComment_24::deleteThis);
RW_FUNCTION(0x545264, "AvatarInfoDialog::openPopUpComment::$_24 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpComment_24::target);
RW_FUNCTION(0x545284, "AvatarInfoDialog::openPopUpComment::$_24 target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_24::targetType);
RW_FUNCTION(0x545298, "AvatarInfoDialog::openPopUpComment::$_22 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpComment_22::deleteThis);
RW_FUNCTION(0x5452A0, "AvatarInfoDialog::openPopUpComment::$_22 __clone()", AvatarInfoDialog_g5thunks::openPopUpComment_22::clone);
RW_FUNCTION(0x5452C4, "AvatarInfoDialog::openPopUpComment::$_22 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpComment_22::cloneInto);
RW_FUNCTION(0x5452E0, "AvatarInfoDialog::openPopUpComment::$_22 destroy()", AvatarInfoDialog_g5thunks::openPopUpComment_22::destroy);
RW_FUNCTION(0x5452E2, "AvatarInfoDialog::openPopUpComment::$_22 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpComment_22::deleteThis);
RW_FUNCTION(0x545300, "AvatarInfoDialog::openPopUpComment::$_22 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpComment_22::target);
RW_FUNCTION(0x545320, "AvatarInfoDialog::openPopUpComment::$_22 target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_22::targetType);
RW_FUNCTION(0x5459D8, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::deleteThis);
RW_FUNCTION(0x5459E0, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::clone);
RW_FUNCTION(0x545A10, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::cloneInto);
RW_FUNCTION(0x545A38, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::destroy);
RW_FUNCTION(0x545A3A, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::deleteThis);
RW_FUNCTION(0x545BF4, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::target);
RW_FUNCTION(0x545C14, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb::targetType);
RW_FUNCTION(0x545C28, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::deleteThis);
RW_FUNCTION(0x545C30, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::clone);
RW_FUNCTION(0x545C54, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::cloneInto);
RW_FUNCTION(0x545C70, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::destroy);
RW_FUNCTION(0x545C72, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::deleteThis);
RW_FUNCTION(0x545C7C, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::target);
RW_FUNCTION(0x545C9C, "AvatarInfoDialog::openPopUpComment::$_21::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpComment_21_lb_lb::targetType);
RW_FUNCTION(0x545CB0, "AvatarInfoDialog::openPopUpSubName::$_19 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_19::deleteThis);
RW_FUNCTION(0x545CB8, "AvatarInfoDialog::openPopUpSubName::$_19 __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_19::clone);
RW_FUNCTION(0x545CF8, "AvatarInfoDialog::openPopUpSubName::$_19 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_19::cloneInto);
RW_FUNCTION(0x545D2C, "AvatarInfoDialog::openPopUpSubName::$_19 destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_19::destroy);
RW_FUNCTION(0x545D2E, "AvatarInfoDialog::openPopUpSubName::$_19 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_19::deleteThis);
RW_FUNCTION(0x545E30, "AvatarInfoDialog::openPopUpSubName::$_19 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_19::target);
RW_FUNCTION(0x545E50, "AvatarInfoDialog::openPopUpSubName::$_19 target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_19::targetType);
RW_FUNCTION(0x545E64, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::deleteThis);
RW_FUNCTION(0x545E6C, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::clone);
RW_FUNCTION(0x545EAC, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::cloneInto);
RW_FUNCTION(0x545EE0, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::destroy);
RW_FUNCTION(0x545EE2, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::deleteThis);
RW_FUNCTION(0x545F30, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::target);
RW_FUNCTION(0x545F50, "AvatarInfoDialog::openPopUpSubName::$_19::lambda(int) target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_19_li::targetType);
RW_FUNCTION(0x545F64, "AvatarInfoDialog::openPopUpSubName::$_18 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_18::deleteThis);
RW_FUNCTION(0x545F6C, "AvatarInfoDialog::openPopUpSubName::$_18 __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_18::clone);
RW_FUNCTION(0x545FAC, "AvatarInfoDialog::openPopUpSubName::$_18 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_18::cloneInto);
RW_FUNCTION(0x545FE0, "AvatarInfoDialog::openPopUpSubName::$_18 destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_18::destroy);
RW_FUNCTION(0x545FE2, "AvatarInfoDialog::openPopUpSubName::$_18 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_18::deleteThis);
RW_FUNCTION(0x5460E4, "AvatarInfoDialog::openPopUpSubName::$_18 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_18::target);
RW_FUNCTION(0x546104, "AvatarInfoDialog::openPopUpSubName::$_18 target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_18::targetType);
RW_FUNCTION(0x54611A, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::deleteThis);
RW_FUNCTION(0x546124, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::clone);
RW_FUNCTION(0x546164, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::cloneInto);
RW_FUNCTION(0x546198, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::destroy);
RW_FUNCTION(0x54619A, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::deleteThis);
RW_FUNCTION(0x5461E8, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::target);
RW_FUNCTION(0x546208, "AvatarInfoDialog::openPopUpSubName::$_18::lambda(int) target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_18_li::targetType);
RW_FUNCTION(0x54621C, "AvatarInfoDialog::openPopUpSubName::$_17 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_17::deleteThis);
RW_FUNCTION(0x546224, "AvatarInfoDialog::openPopUpSubName::$_17 __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_17::clone);
RW_FUNCTION(0x546264, "AvatarInfoDialog::openPopUpSubName::$_17 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_17::cloneInto);
RW_FUNCTION(0x546298, "AvatarInfoDialog::openPopUpSubName::$_17 destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_17::destroy);
RW_FUNCTION(0x54629A, "AvatarInfoDialog::openPopUpSubName::$_17 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_17::deleteThis);
RW_FUNCTION(0x5462D8, "AvatarInfoDialog::openPopUpSubName::$_17 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_17::target);
RW_FUNCTION(0x5462F8, "AvatarInfoDialog::openPopUpSubName::$_17 target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_17::targetType);
RW_FUNCTION(0x546868, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::deleteThis);
RW_FUNCTION(0x546870, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::clone);
RW_FUNCTION(0x5468A0, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::cloneInto);
RW_FUNCTION(0x5468C8, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::destroy);
RW_FUNCTION(0x5468CA, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::deleteThis);
RW_FUNCTION(0x5468FC, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::target);
RW_FUNCTION(0x54691C, "AvatarInfoDialog::openPopUpSubName::$_14::lambda() target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_14_lv::targetType);
RW_FUNCTION(0x546930, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) ~__func()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::destructor);
RW_FUNCTION(0x54696C, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::destructorDelete);
RW_FUNCTION(0x5469AC, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::clone);
RW_FUNCTION(0x5469FC, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::cloneInto);
RW_FUNCTION(0x546A30, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::destroy);
RW_FUNCTION(0x546A50, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::destroyDeallocate);
RW_FUNCTION(0x546B50, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::target);
RW_FUNCTION(0x546B70, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb::targetType);
RW_FUNCTION(0x546B84, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::deleteThis);
RW_FUNCTION(0x546B8C, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::clone);
RW_FUNCTION(0x546BB0, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::cloneInto);
RW_FUNCTION(0x546BCC, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::destroy);
RW_FUNCTION(0x546BCE, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::deleteThis);
RW_FUNCTION(0x546BE4, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::target);
RW_FUNCTION(0x546C04, "AvatarInfoDialog::openPopUpSubName::$_16::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_16_lb_lb::targetType);
RW_FUNCTION(0x546C18, "AvatarInfoDialog::openPopUpSubName::$_15 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_15::deleteThis);
RW_FUNCTION(0x546C20, "AvatarInfoDialog::openPopUpSubName::$_15 __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_15::clone);
RW_FUNCTION(0x546C4C, "AvatarInfoDialog::openPopUpSubName::$_15 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_15::cloneInto);
RW_FUNCTION(0x546C6C, "AvatarInfoDialog::openPopUpSubName::$_15 destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_15::destroy);
RW_FUNCTION(0x546C6E, "AvatarInfoDialog::openPopUpSubName::$_15 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_15::deleteThis);
RW_FUNCTION(0x546C80, "AvatarInfoDialog::openPopUpSubName::$_15 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_15::target);
RW_FUNCTION(0x546CA0, "AvatarInfoDialog::openPopUpSubName::$_15 target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_15::targetType);
RW_FUNCTION(0x546CB4, "AvatarInfoDialog::openPopUpSubName::$_13 ~__func() (deleting)", AvatarInfoDialog_g5thunks::openPopUpSubName_13::deleteThis);
RW_FUNCTION(0x546CBC, "AvatarInfoDialog::openPopUpSubName::$_13 __clone()", AvatarInfoDialog_g5thunks::openPopUpSubName_13::clone);
RW_FUNCTION(0x546CE0, "AvatarInfoDialog::openPopUpSubName::$_13 __clone(__base*)", AvatarInfoDialog_g5thunks::openPopUpSubName_13::cloneInto);
RW_FUNCTION(0x546CFC, "AvatarInfoDialog::openPopUpSubName::$_13 destroy()", AvatarInfoDialog_g5thunks::openPopUpSubName_13::destroy);
RW_FUNCTION(0x546CFE, "AvatarInfoDialog::openPopUpSubName::$_13 destroy_deallocate()", AvatarInfoDialog_g5thunks::openPopUpSubName_13::deleteThis);
RW_FUNCTION(0x546D1C, "AvatarInfoDialog::openPopUpSubName::$_13 target(const type_info&)", AvatarInfoDialog_g5thunks::openPopUpSubName_13::target);
RW_FUNCTION(0x546D3C, "AvatarInfoDialog::openPopUpSubName::$_13 target_type()", AvatarInfoDialog_g5thunks::openPopUpSubName_13::targetType);
RW_FUNCTION(0x546D50, "AvatarInfoDialog::close::$_12 ~__func() (deleting)", AvatarInfoDialog_g5thunks::close_12::deleteThis);
RW_FUNCTION(0x546D58, "AvatarInfoDialog::close::$_12 __clone()", AvatarInfoDialog_g5thunks::close_12::clone);
RW_FUNCTION(0x546D84, "AvatarInfoDialog::close::$_12 __clone(__base*)", AvatarInfoDialog_g5thunks::close_12::cloneInto);
RW_FUNCTION(0x546DA4, "AvatarInfoDialog::close::$_12 destroy()", AvatarInfoDialog_g5thunks::close_12::destroy);
RW_FUNCTION(0x546DA6, "AvatarInfoDialog::close::$_12 destroy_deallocate()", AvatarInfoDialog_g5thunks::close_12::deleteThis);
RW_FUNCTION(0x546E14, "AvatarInfoDialog::close::$_12 target(const type_info&)", AvatarInfoDialog_g5thunks::close_12::target);
RW_FUNCTION(0x546E34, "AvatarInfoDialog::close::$_12 target_type()", AvatarInfoDialog_g5thunks::close_12::targetType);
RW_FUNCTION(0x546E48, "AvatarInfoDialog::createLayout::$_11 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_11::deleteThis);
RW_FUNCTION(0x546E50, "AvatarInfoDialog::createLayout::$_11 __clone()", AvatarInfoDialog_g5thunks::createLayout_11::clone);
RW_FUNCTION(0x546E7C, "AvatarInfoDialog::createLayout::$_11 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_11::cloneInto);
RW_FUNCTION(0x546E9C, "AvatarInfoDialog::createLayout::$_11 destroy()", AvatarInfoDialog_g5thunks::createLayout_11::destroy);
RW_FUNCTION(0x546E9E, "AvatarInfoDialog::createLayout::$_11 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_11::deleteThis);
RW_FUNCTION(0x546F3C, "AvatarInfoDialog::createLayout::$_11 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_11::target);
RW_FUNCTION(0x546F5C, "AvatarInfoDialog::createLayout::$_11 target_type()", AvatarInfoDialog_g5thunks::createLayout_11::targetType);
RW_FUNCTION(0x546F70, "AvatarInfoDialog::createLayout::$_10 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_10::deleteThis);
RW_FUNCTION(0x546F78, "AvatarInfoDialog::createLayout::$_10 __clone()", AvatarInfoDialog_g5thunks::createLayout_10::clone);
RW_FUNCTION(0x546FA4, "AvatarInfoDialog::createLayout::$_10 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_10::cloneInto);
RW_FUNCTION(0x546FC4, "AvatarInfoDialog::createLayout::$_10 destroy()", AvatarInfoDialog_g5thunks::createLayout_10::destroy);
RW_FUNCTION(0x546FC6, "AvatarInfoDialog::createLayout::$_10 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_10::deleteThis);
RW_FUNCTION(0x547064, "AvatarInfoDialog::createLayout::$_10 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_10::target);
RW_FUNCTION(0x547084, "AvatarInfoDialog::createLayout::$_10 target_type()", AvatarInfoDialog_g5thunks::createLayout_10::targetType);
RW_FUNCTION(0x547098, "AvatarInfoDialog::createLayout::$_9 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_9::deleteThis);
RW_FUNCTION(0x5470A0, "AvatarInfoDialog::createLayout::$_9 __clone()", AvatarInfoDialog_g5thunks::createLayout_9::clone);
RW_FUNCTION(0x5470D0, "AvatarInfoDialog::createLayout::$_9 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_9::cloneInto);
RW_FUNCTION(0x5470F4, "AvatarInfoDialog::createLayout::$_9 destroy()", AvatarInfoDialog_g5thunks::createLayout_9::destroy);
RW_FUNCTION(0x5470F6, "AvatarInfoDialog::createLayout::$_9 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_9::deleteThis);
RW_FUNCTION(0x547124, "AvatarInfoDialog::createLayout::$_9 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_9::target);
RW_FUNCTION(0x547144, "AvatarInfoDialog::createLayout::$_9 target_type()", AvatarInfoDialog_g5thunks::createLayout_9::targetType);
RW_FUNCTION(0x54A234, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::deleteThis);
RW_FUNCTION(0x54A23C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::clone);
RW_FUNCTION(0x54A268, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::cloneInto);
RW_FUNCTION(0x54A288, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::destroy);
RW_FUNCTION(0x54A28A, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::deleteThis);
RW_FUNCTION(0x54A33C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::target);
RW_FUNCTION(0x54A35C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3::targetType);
RW_FUNCTION(0x54A370, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::deleteThis);
RW_FUNCTION(0x54A378, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::clone);
RW_FUNCTION(0x54A3A4, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::cloneInto);
RW_FUNCTION(0x54A3C4, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::destroy);
RW_FUNCTION(0x54A3C6, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::deleteThis);
RW_FUNCTION(0x54A490, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::target);
RW_FUNCTION(0x54A4B0, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3_::targetType);
RW_FUNCTION(0x54A4C4, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::deleteThis);
RW_FUNCTION(0x54A4CC, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::clone);
RW_FUNCTION(0x54A4F0, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::cloneInto);
RW_FUNCTION(0x54A50C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::destroy);
RW_FUNCTION(0x54A50E, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::deleteThis);
RW_FUNCTION(0x54A518, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::target);
RW_FUNCTION(0x54A538, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#3 target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE3__::targetType);
RW_FUNCTION(0x54A550, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lfi::deleteThis);
RW_FUNCTION(0x54A558, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lfi::clone);
RW_FUNCTION(0x54A588, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lfi::cloneInto);
RW_FUNCTION(0x54A5AC, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lfi::destroy);
RW_FUNCTION(0x54A5AE, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lfi::deleteThis);
RW_FUNCTION(0x54A6D8, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lfi::target);
RW_FUNCTION(0x54A6F8, "AvatarInfoDialog::createLayout::$_8::lambda(float,int) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lfi::targetType);
RW_FUNCTION(0x54A70E, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::deleteThis);
RW_FUNCTION(0x54A718, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::clone);
RW_FUNCTION(0x54A73C, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::cloneInto);
RW_FUNCTION(0x54A758, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::destroy);
RW_FUNCTION(0x54A75A, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::deleteThis);
RW_FUNCTION(0x54A764, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::target);
RW_FUNCTION(0x54A784, "AvatarInfoDialog::createLayout::$_8::lambda(N9SceneUtil19MedalInfoButtonTypeEbi) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lN9SceneUtil19MedalInfoButtonTypeEbi::targetType);
RW_FUNCTION(0x54A798, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::deleteThis);
RW_FUNCTION(0x54A7A0, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::clone);
RW_FUNCTION(0x54A7E0, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::cloneInto);
RW_FUNCTION(0x54A814, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::destroy);
RW_FUNCTION(0x54A816, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::deleteThis);
RW_FUNCTION(0x54A92C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::target);
RW_FUNCTION(0x54A94C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE)#2 target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE2::targetType);
RW_FUNCTION(0x54A960, "AvatarInfoDialog::createLayout::$_8::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lb::deleteThis);
RW_FUNCTION(0x54A968, "AvatarInfoDialog::createLayout::$_8::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lb::clone);
RW_FUNCTION(0x54A9A0, "AvatarInfoDialog::createLayout::$_8::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lb::cloneInto);
RW_FUNCTION(0x54A9D4, "AvatarInfoDialog::createLayout::$_8::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lb::destroy);
RW_FUNCTION(0x54A9D6, "AvatarInfoDialog::createLayout::$_8::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lb::deleteThis);
RW_FUNCTION(0x54AB5C, "AvatarInfoDialog::createLayout::$_8::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lb::target);
RW_FUNCTION(0x54AB7C, "AvatarInfoDialog::createLayout::$_8::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lb::targetType);
RW_FUNCTION(0x54AC18, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::deleteThis);
RW_FUNCTION(0x54AC20, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::clone);
RW_FUNCTION(0x54AC4C, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::cloneInto);
RW_FUNCTION(0x54AC6C, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::destroy);
RW_FUNCTION(0x54AC6E, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::deleteThis);
RW_FUNCTION(0x54AD68, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::target);
RW_FUNCTION(0x54AD88, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb::targetType);
RW_FUNCTION(0x54AD9C, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::deleteThis);
RW_FUNCTION(0x54ADA4, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::clone);
RW_FUNCTION(0x54ADC8, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::cloneInto);
RW_FUNCTION(0x54ADE4, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::destroy);
RW_FUNCTION(0x54ADE6, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::deleteThis);
RW_FUNCTION(0x54ADFC, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::target);
RW_FUNCTION(0x54AE1C, "AvatarInfoDialog::createLayout::$_8::lambda(bool)::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lb_lb_lb::targetType);
RW_FUNCTION(0x54AE94, "AvatarInfoDialog::createLayout::$_73 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_73::deleteThis);
RW_FUNCTION(0x54AE9C, "AvatarInfoDialog::createLayout::$_73 __clone()", AvatarInfoDialog_g5thunks::createLayout_73::clone);
RW_FUNCTION(0x54AED8, "AvatarInfoDialog::createLayout::$_73 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_73::cloneInto);
RW_FUNCTION(0x54AF08, "AvatarInfoDialog::createLayout::$_73 destroy()", AvatarInfoDialog_g5thunks::createLayout_73::destroy);
RW_FUNCTION(0x54AF0A, "AvatarInfoDialog::createLayout::$_73 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_73::deleteThis);
RW_FUNCTION(0x54AF9C, "AvatarInfoDialog::createLayout::$_73 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_73::target);
RW_FUNCTION(0x54AFBC, "AvatarInfoDialog::createLayout::$_73 target_type()", AvatarInfoDialog_g5thunks::createLayout_73::targetType);
RW_FUNCTION(0x54AFD0, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::deleteThis);
RW_FUNCTION(0x54AFD8, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) __clone()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::clone);
RW_FUNCTION(0x54B008, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::cloneInto);
RW_FUNCTION(0x54B02C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) destroy()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::destroy);
RW_FUNCTION(0x54B02E, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::deleteThis);
RW_FUNCTION(0x54B09C, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::target);
RW_FUNCTION(0x54B0BC, "AvatarInfoDialog::createLayout::$_8::lambda(PNS4_3RefENS6_14TouchEventTypeE) target_type()", AvatarInfoDialog_g5thunks::createLayout_8_lPNS4_3RefENS6_14TouchEventTypeE::targetType);
RW_FUNCTION(0x54B2C2, "AvatarInfoDialog::createLayout::$_7 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_7::deleteThis);
RW_FUNCTION(0x54B2CC, "AvatarInfoDialog::createLayout::$_7 __clone()", AvatarInfoDialog_g5thunks::createLayout_7::clone);
RW_FUNCTION(0x54B2F8, "AvatarInfoDialog::createLayout::$_7 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_7::cloneInto);
RW_FUNCTION(0x54B318, "AvatarInfoDialog::createLayout::$_7 destroy()", AvatarInfoDialog_g5thunks::createLayout_7::destroy);
RW_FUNCTION(0x54B31A, "AvatarInfoDialog::createLayout::$_7 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_7::deleteThis);
RW_FUNCTION(0x54B440, "AvatarInfoDialog::createLayout::$_7 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_7::target);
RW_FUNCTION(0x54B460, "AvatarInfoDialog::createLayout::$_7 target_type()", AvatarInfoDialog_g5thunks::createLayout_7::targetType);
RW_FUNCTION(0x54B474, "AvatarInfoDialog::createLayout::$_6 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_6::deleteThis);
RW_FUNCTION(0x54B47C, "AvatarInfoDialog::createLayout::$_6 __clone()", AvatarInfoDialog_g5thunks::createLayout_6::clone);
RW_FUNCTION(0x54B4C8, "AvatarInfoDialog::createLayout::$_6 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_6::cloneInto);
RW_FUNCTION(0x54B508, "AvatarInfoDialog::createLayout::$_6 destroy()", AvatarInfoDialog_g5thunks::createLayout_6::destroy);
RW_FUNCTION(0x54B50A, "AvatarInfoDialog::createLayout::$_6 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_6::deleteThis);
RW_FUNCTION(0x54B72C, "AvatarInfoDialog::createLayout::$_6 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_6::target);
RW_FUNCTION(0x54B74C, "AvatarInfoDialog::createLayout::$_6 target_type()", AvatarInfoDialog_g5thunks::createLayout_6::targetType);
RW_FUNCTION(0x54B760, "AvatarInfoDialog::createLayout::$_6::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_6_lb::deleteThis);
RW_FUNCTION(0x54B768, "AvatarInfoDialog::createLayout::$_6::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_6_lb::clone);
RW_FUNCTION(0x54B794, "AvatarInfoDialog::createLayout::$_6::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_6_lb::cloneInto);
RW_FUNCTION(0x54B7B4, "AvatarInfoDialog::createLayout::$_6::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_6_lb::destroy);
RW_FUNCTION(0x54B7B6, "AvatarInfoDialog::createLayout::$_6::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_6_lb::deleteThis);
RW_FUNCTION(0x54B900, "AvatarInfoDialog::createLayout::$_6::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_6_lb::target);
RW_FUNCTION(0x54B920, "AvatarInfoDialog::createLayout::$_6::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_6_lb::targetType);
RW_FUNCTION(0x54B934, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::deleteThis);
RW_FUNCTION(0x54B93C, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() __clone()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::clone);
RW_FUNCTION(0x54B968, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::cloneInto);
RW_FUNCTION(0x54B988, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() destroy()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::destroy);
RW_FUNCTION(0x54B98A, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::deleteThis);
RW_FUNCTION(0x54BA2C, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::target);
RW_FUNCTION(0x54BA4C, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda() target_type()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lv::targetType);
RW_FUNCTION(0x54BA60, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::deleteThis);
RW_FUNCTION(0x54BA68, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::clone);
RW_FUNCTION(0x54BA8C, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::cloneInto);
RW_FUNCTION(0x54BAA8, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::destroy);
RW_FUNCTION(0x54BAAA, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::deleteThis);
RW_FUNCTION(0x54BAC0, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::target);
RW_FUNCTION(0x54BAE0, "AvatarInfoDialog::createLayout::$_6::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_6_lb_lb::targetType);
RW_FUNCTION(0x54BAF4, "AvatarInfoDialog::createLayout::$_6::lambda() ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_6_lv::deleteThis);
RW_FUNCTION(0x54BAFC, "AvatarInfoDialog::createLayout::$_6::lambda() __clone()", AvatarInfoDialog_g5thunks::createLayout_6_lv::clone);
RW_FUNCTION(0x54BB28, "AvatarInfoDialog::createLayout::$_6::lambda() __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_6_lv::cloneInto);
RW_FUNCTION(0x54BB48, "AvatarInfoDialog::createLayout::$_6::lambda() destroy()", AvatarInfoDialog_g5thunks::createLayout_6_lv::destroy);
RW_FUNCTION(0x54BB4A, "AvatarInfoDialog::createLayout::$_6::lambda() destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_6_lv::deleteThis);
RW_FUNCTION(0x54BBEC, "AvatarInfoDialog::createLayout::$_6::lambda() target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_6_lv::target);
RW_FUNCTION(0x54BC0C, "AvatarInfoDialog::createLayout::$_6::lambda() target_type()", AvatarInfoDialog_g5thunks::createLayout_6_lv::targetType);
RW_FUNCTION(0x54BC20, "AvatarInfoDialog::createLayout::$_5 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_5::deleteThis);
RW_FUNCTION(0x54BC28, "AvatarInfoDialog::createLayout::$_5 __clone()", AvatarInfoDialog_g5thunks::createLayout_5::clone);
RW_FUNCTION(0x54BC74, "AvatarInfoDialog::createLayout::$_5 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_5::cloneInto);
RW_FUNCTION(0x54BCB4, "AvatarInfoDialog::createLayout::$_5 destroy()", AvatarInfoDialog_g5thunks::createLayout_5::destroy);
RW_FUNCTION(0x54BCB6, "AvatarInfoDialog::createLayout::$_5 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_5::deleteThis);
RW_FUNCTION(0x54BEBC, "AvatarInfoDialog::createLayout::$_5 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_5::target);
RW_FUNCTION(0x54BEDC, "AvatarInfoDialog::createLayout::$_5 target_type()", AvatarInfoDialog_g5thunks::createLayout_5::targetType);
RW_FUNCTION(0x54C2A8, "AvatarInfoDialog::createLayout::$_72 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_72::deleteThis);
RW_FUNCTION(0x54C2B0, "AvatarInfoDialog::createLayout::$_72 __clone()", AvatarInfoDialog_g5thunks::createLayout_72::clone);
RW_FUNCTION(0x54C2EC, "AvatarInfoDialog::createLayout::$_72 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_72::cloneInto);
RW_FUNCTION(0x54C31C, "AvatarInfoDialog::createLayout::$_72 destroy()", AvatarInfoDialog_g5thunks::createLayout_72::destroy);
RW_FUNCTION(0x54C31E, "AvatarInfoDialog::createLayout::$_72 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_72::deleteThis);
RW_FUNCTION(0x54C3B4, "AvatarInfoDialog::createLayout::$_72 target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_72::target);
RW_FUNCTION(0x54C3D4, "AvatarInfoDialog::createLayout::$_72 target_type()", AvatarInfoDialog_g5thunks::createLayout_72::targetType);
RW_FUNCTION(0x54C678, "AvatarInfoDialog::createLayout::$_3::lambda() ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_3_lv::deleteThis);
RW_FUNCTION(0x54C680, "AvatarInfoDialog::createLayout::$_3::lambda() __clone()", AvatarInfoDialog_g5thunks::createLayout_3_lv::clone);
RW_FUNCTION(0x54C6AC, "AvatarInfoDialog::createLayout::$_3::lambda() __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_3_lv::cloneInto);
RW_FUNCTION(0x54C6CC, "AvatarInfoDialog::createLayout::$_3::lambda() destroy()", AvatarInfoDialog_g5thunks::createLayout_3_lv::destroy);
RW_FUNCTION(0x54C6CE, "AvatarInfoDialog::createLayout::$_3::lambda() destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_3_lv::deleteThis);
RW_FUNCTION(0x54C7F4, "AvatarInfoDialog::createLayout::$_3::lambda() target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_3_lv::target);
RW_FUNCTION(0x54C814, "AvatarInfoDialog::createLayout::$_3::lambda() target_type()", AvatarInfoDialog_g5thunks::createLayout_3_lv::targetType);
RW_FUNCTION(0x54C828, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::deleteThis);
RW_FUNCTION(0x54C830, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::clone);
RW_FUNCTION(0x54C85C, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::cloneInto);
RW_FUNCTION(0x54C87C, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::destroy);
RW_FUNCTION(0x54C87E, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::deleteThis);
RW_FUNCTION(0x54C978, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::target);
RW_FUNCTION(0x54C998, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb::targetType);
RW_FUNCTION(0x54C9AC, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::deleteThis);
RW_FUNCTION(0x54C9B4, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) __clone()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::clone);
RW_FUNCTION(0x54C9D8, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::cloneInto);
RW_FUNCTION(0x54C9F4, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) destroy()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::destroy);
RW_FUNCTION(0x54C9F6, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::deleteThis);
RW_FUNCTION(0x54CA00, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) target(const type_info&)", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::target);
RW_FUNCTION(0x54CA20, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool)::lambda(bool) target_type()", AvatarInfoDialog_g5thunks::createLayout_3_lv_lb_lb::targetType);
RW_FUNCTION(0x54CA34, "AvatarInfoDialog::createLayout::$_2 ~__func() (deleting)", AvatarInfoDialog_g5thunks::createLayout_2::deleteThis);
RW_FUNCTION(0x54CA3C, "AvatarInfoDialog::createLayout::$_2 __clone()", AvatarInfoDialog_g5thunks::createLayout_2::clone);
RW_FUNCTION(0x54CA88, "AvatarInfoDialog::createLayout::$_2 __clone(__base*)", AvatarInfoDialog_g5thunks::createLayout_2::cloneInto);
RW_FUNCTION(0x54CAC8, "AvatarInfoDialog::createLayout::$_2 destroy()", AvatarInfoDialog_g5thunks::createLayout_2::destroy);
RW_FUNCTION(0x54CACA, "AvatarInfoDialog::createLayout::$_2 destroy_deallocate()", AvatarInfoDialog_g5thunks::createLayout_2::deleteThis);
