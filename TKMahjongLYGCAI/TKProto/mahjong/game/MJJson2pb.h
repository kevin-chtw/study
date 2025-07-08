/*
 * Copyright (c) 2013 Pavel Shramov <shramov@mexmat.net>
 *
 * json2pb is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef __JSON2PB_H__
#define __JSON2PB_H__

#include <string>
#include <exception>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "json/json.h"

namespace MJ
{
class j2pb_error : public std::exception
{
    std::string _error;

public:
    j2pb_error(const std::string& e) : _error(e)
    {
    }
    virtual ~j2pb_error() throw(){};

    virtual const char* what() const throw()
    {
        return _error.c_str();
    }
};

void json2pb(google::protobuf::Message& msg, std::string const& s);

Json::Value pb2json(const google::protobuf::Message& msg);

}  // namespace MJ
#endif  //__JSON2PB_H__
