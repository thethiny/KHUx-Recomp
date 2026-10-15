// TextData: the localized text files (text/<type>/<id>.txt in the virtual file system), and the hole::text
// helpers that pick a text id. TextFormat::execute expands a text's ${...} placeholders printf-style.
#pragma once

#include "SystemCommon.hpp"

namespace textdata {

constexpr uint32_t TYPE_NAMES = 0xF555AC;    // const char* [Type]: the text/ sub-folder of each Type
constexpr uint32_t PATH_FORMAT = 0xDFA063;   // "text/%s/%d.txt"

// TextData::get(Type, id) -> shared_ptr<bg::Data> (sret)
void get(guest::SharedPtr *out, int32_t type, int32_t id);
// TextData::format(Type, id, ...) -> std::string (sret): the text with the arguments formatted in
void format(guest::String *out, int32_t type, int32_t id);

// hole::text
void getUnionName(guest::SharedPtr *out, int32_t unionId);
// "N days / hours / minutes left" and "N ... ago"
void createTimeLeftString(guest::String *out, int32_t seconds);
void createTimeAgoString(guest::String *out, int32_t seconds);

// TextFormat::execute(fmt, ...) -> std::string (sret)
void textFormatExecute(guest::String *out, uint32_t fmt);

}  // namespace textdata
