/*
 * Copyright (c) 2013 Pavel Shramov <shramov@mexmat.net>
 *
 * json2pb is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <errno.h>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include "MJJson2pb.h"

#include <stdexcept>

namespace MJ
{
std::string b64_encode(std::string const& s)
{
    return s;
}
std::string b64_decode(std::string const& s)
{
    return s;
}

using google::protobuf::Message;
using google::protobuf::MessageFactory;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::EnumDescriptor;
using google::protobuf::EnumValueDescriptor;
using google::protobuf::Reflection;

static Json::Value _pb2json(const Message& msg);
static Json::Value _field2json(const Message& msg, const FieldDescriptor* field, size_t index)
{
    const Reflection* ref = msg.GetReflection();
    const bool repeated = field->is_repeated();
    switch (field->cpp_type())
    {
#define _CONVERT(type, ctype, jtype, sfunc, afunc)                                               \
    case FieldDescriptor::type: {                                                                \
        const ctype value = (repeated) ? ref->afunc(msg, field, index) : ref->sfunc(msg, field); \
        return Json::Value((jtype)value);                                                        \
        break;                                                                                   \
    }

        _CONVERT(CPPTYPE_DOUBLE, double, double, GetDouble, GetRepeatedDouble);
        _CONVERT(CPPTYPE_FLOAT, double, double, GetFloat, GetRepeatedFloat);
        _CONVERT(CPPTYPE_INT64, int64_t, Json::Int, GetInt64, GetRepeatedInt64);
        _CONVERT(CPPTYPE_UINT64, uint64_t, Json::UInt, GetUInt64, GetRepeatedUInt64);
        _CONVERT(CPPTYPE_INT32, int32_t, Json::Int, GetInt32, GetRepeatedInt32);
        _CONVERT(CPPTYPE_UINT32, uint32_t, Json::UInt, GetUInt32, GetRepeatedUInt32);
        _CONVERT(CPPTYPE_BOOL, bool, Json::Int, GetBool, GetRepeatedBool);
#undef _CONVERT
    case FieldDescriptor::CPPTYPE_STRING: {
        std::string scratch;
        auto& value = repeated ? ref->GetRepeatedStringReference(msg, field, index, &scratch) : ref->GetStringReference(msg, field, &scratch);
        if (field->type() == FieldDescriptor::TYPE_BYTES)
        {
            return Json::Value(b64_encode(value).c_str());
        }
        else
        {
            return Json::Value(value.c_str());
        }
        break;
    }
    case FieldDescriptor::CPPTYPE_MESSAGE: {
        const Message& mf = (repeated) ? ref->GetRepeatedMessage(msg, field, index) : ref->GetMessage(msg, field);
        return _pb2json(mf);
        break;
    }
    case FieldDescriptor::CPPTYPE_ENUM: {
        const EnumValueDescriptor* ef = (repeated) ? ref->GetRepeatedEnum(msg, field, index) : ref->GetEnum(msg, field);
        return Json::Value::Int(ef->number());
        break;
    }
    default:
        return Json::Value();
        break;
    }
}

static Json::Value _pb2json(const Message& msg)
{
    const Descriptor* d = msg.GetDescriptor();
    const Reflection* ref = msg.GetReflection();
    if (!d || !ref)
    {
        throw j2pb_error("error msg");
    }

    Json::Value root;

    std::vector<const FieldDescriptor*> fields;
    ref->ListFields(msg, &fields);

    for (size_t i = 0; i != fields.size(); i++)
    {
        const FieldDescriptor* field = fields[i];

        Json::Value jf;
        if (field->is_repeated())
        {
            size_t count = ref->FieldSize(msg, field);
            if (!count)
                continue;

            for (size_t j = 0; j < count; j++)
            {
                jf.append(_field2json(msg, field, j));
            }
        }
        else if (ref->HasField(msg, field))
        {
            jf = _field2json(msg, field, 0);
        }
        else

            continue;

        const std::string& name = (field->is_extension()) ? field->full_name() : field->name();
        root[name] = jf;
    }
    return root;
}

static void _json2pb(Message& msg, Json::Value& root);
static void _json2field(Message& msg, const FieldDescriptor* field, Json::Value& jf)
{
    const Reflection* ref = msg.GetReflection();
    const bool repeated = field->is_repeated();

    switch (field->cpp_type())
    {
#define _SET_OR_ADD(sfunc, afunc, value)    \
    do                                      \
    {                                       \
        if (repeated)                       \
            ref->afunc(&msg, field, value); \
        else                                \
            ref->sfunc(&msg, field, value); \
    } while (0)

#define _CONVERT(type, ctype, fmt, sfunc, afunc) \
    case FieldDescriptor::type: {                \
        ctype value = fmt();                     \
        _SET_OR_ADD(sfunc, afunc, value);        \
        break;                                   \
    }

        _CONVERT(CPPTYPE_DOUBLE, double, jf.asDouble, SetDouble, AddDouble);
        _CONVERT(CPPTYPE_FLOAT, float, jf.asDouble, SetFloat, AddFloat);
        _CONVERT(CPPTYPE_INT64, Json::Int, jf.asInt, SetInt64, AddInt64);
        _CONVERT(CPPTYPE_UINT64, Json::UInt, jf.asUInt, SetUInt64, AddUInt64);
        _CONVERT(CPPTYPE_INT32, Json::Int, jf.asInt, SetInt32, AddInt32);
        _CONVERT(CPPTYPE_UINT32, Json::UInt, jf.asUInt, SetUInt32, AddUInt32);
        _CONVERT(CPPTYPE_BOOL, int, jf.asBool, SetBool, AddBool);

    case FieldDescriptor::CPPTYPE_STRING: {
        auto value = jf.asString();
        if (field->type() == FieldDescriptor::TYPE_BYTES)
        {
            _SET_OR_ADD(SetString, AddString, b64_decode(value));
        }
        else
        {
            _SET_OR_ADD(SetString, AddString, value);
        }
        break;
    }
    case FieldDescriptor::CPPTYPE_MESSAGE: {
        Message* mf = (repeated) ? ref->AddMessage(&msg, field) : ref->MutableMessage(&msg, field);
        _json2pb(*mf, jf);
        break;
    }
    case FieldDescriptor::CPPTYPE_ENUM: {
        const EnumDescriptor* ed = field->enum_type();
        const EnumValueDescriptor* ev = 0;

        if (jf.isNumeric())
        {
            ev = ed->FindValueByNumber(jf.asInt());
        }
        else if (jf.isString())
        {
            ev = ed->FindValueByName(jf.asString());
        }
        else
        {
            throw j2pb_error(field->full_name() + ":Not an integer or string");
        }
        if (!ev)
        {
            throw j2pb_error(field->full_name() + ":Enum value not found");
        }
        _SET_OR_ADD(SetEnum, AddEnum, ev);
        break;
    }
    default:
        break;
    }
}

static void _json2pb(Message& msg, Json::Value& root)
{
    const Descriptor* d = msg.GetDescriptor();
    const Reflection* ref = msg.GetReflection();
    if (!d || !ref)
    {
        throw j2pb_error("No descriptor or reflection");
    }

    auto names = root.getMemberNames();

    for (auto const& name : names)
    {
        auto& jf = root[name];

        const FieldDescriptor* field = d->FindFieldByName(name);
        if (!field)
            field = ref->FindKnownExtensionByName(name);
        //field = d->file()->FindExtensionByName(name);

        if (!field)
            throw j2pb_error("Unknown field: " + std::string(name));

        int r = 0;
        if (field->is_repeated())
        {
            if (!jf.isArray())
            {
                throw j2pb_error(field->full_name() + ":Not array");
            }
            for (size_t j = 0; j < jf.size(); j++)
            {
                _json2field(msg, field, jf[(unsigned)j]);
            }
        }
        else
        {
            _json2field(msg, field, jf);
        }
    }
}

void json2pb(google::protobuf::Message& msg, std::string const& s)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(s, root);
    _json2pb(msg, root);
}

Json::Value pb2json(const Message& msg)
{
    return _pb2json(msg);
}
}  // namespace MJ
