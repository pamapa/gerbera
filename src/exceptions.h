/*MT*

    MediaTomb - http://www.mediatomb.cc/

    exceptions.h - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>

    Copyright (C) 2006-2010 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>

    Copyright (C) 2016-2025 Gerbera Contributors

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

    $Id$
*/

/// \file exceptions.h

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <fmt/core.h>
#include <stdexcept>
#include <string>

#define LINE_MESSAGE fmt::format("[{}:{}] {}", __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define throw_std_runtime_error(...) throw std::runtime_error(fmt::format("[{}:{}] {} Error: {}", __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt::format(__VA_ARGS__)))
#define throw_fmt_system_error(...) throw fmt::system_error(errno, "[{}:{}] {} Error: {}", __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt::format(__VA_ARGS__))

class ConfigParseException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class UpnpException : public std::runtime_error {
protected:
    int errCode;

public:
    UpnpException(int errCode, const std::string& message);
    [[nodiscard]] int getErrorCode() const { return errCode; }
};

class DatabaseException : public std::runtime_error {
protected:
    std::string userMessage;

public:
    DatabaseException(std::string userMessage, const std::string& message);
    std::string getUserMessage() const { return (!userMessage.empty() ? userMessage : what()); }
};

class SearchParseException : public DatabaseException {
    using DatabaseException::DatabaseException;
};

class ObjectNotFoundException : public DatabaseException {
public:
    explicit ObjectNotFoundException(const std::string& message);
};

class ResourceNotFoundException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ServerShutdownException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class TryAgainException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif // __EXCEPTIONS_H__
