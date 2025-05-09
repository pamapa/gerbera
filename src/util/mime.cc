/*GRB*

    Gerbera - https://gerbera.io/

    mime.cc - this file is part of Gerbera.

    Copyright (C) 2021-2025 Gerbera Contributors

    Gerbera is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    Gerbera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.

    $Id$
*/

/// \file mime.cc
#define GRB_LOG_FAC GrbLogFacility::util
#include "mime.h" // API

#include "config/config.h"
#include "config/config_val.h"
#include "exceptions.h"
#include "util/logger.h"
#include "util/tools.h"

Mime::Mime(const std::shared_ptr<Config>& config)
    : extension_map_case_sensitive(config->getBoolOption(ConfigVal::IMPORT_MAPPINGS_EXTENSION_TO_MIMETYPE_CASE_SENSITIVE))
    , ignore_unknown_extensions(config->getBoolOption(ConfigVal::IMPORT_MAPPINGS_IGNORE_UNKNOWN_EXTENSIONS))
    , extension_mimetype_map(config->getDictionaryOption(ConfigVal::IMPORT_MAPPINGS_EXTENSION_TO_MIMETYPE_LIST))
    , ignoredExtensions(config->getArrayOption(ConfigVal::IMPORT_MAPPINGS_IGNORED_EXTENSIONS))
{
    if (ignore_unknown_extensions && (extension_mimetype_map.empty())) {
        log_warning("Ignore unknown extensions set, but no mappings specified");
        log_warning("Please review your configuration!");
        ignore_unknown_extensions = false;
    }

#ifdef HAVE_MAGIC
    // init filemagic
    int magicFlags = config->getBoolOption(ConfigVal::IMPORT_FOLLOW_SYMLINKS) ? MAGIC_MIME_TYPE | MAGIC_SYMLINK : MAGIC_MIME_TYPE;
    magicCookie = magic_open(magicFlags);
    if (!magicCookie) {
        throw_std_runtime_error("magic_open failed");
    }

    std::string optMagicFile = config->getOption(ConfigVal::IMPORT_MAGIC_FILE);
    log_debug("magic '{}'", optMagicFile);
    const char* magicFile = !optMagicFile.empty() ? optMagicFile.c_str() : nullptr;
    if (magic_load(magicCookie, magicFile) == -1) {
        auto errMsg = magic_error(magicCookie);
        magic_close(magicCookie);
        magicCookie = nullptr;
        throw_std_runtime_error("magic_load failed: {}", errMsg);
    }
#endif // HAVE_MAGIC
}

#ifdef HAVE_MAGIC
Mime::~Mime()
{
    if (magicCookie) {
        magic_close(magicCookie);
        magicCookie = nullptr;
    }
}
#endif

#ifdef HAVE_MAGIC
std::string Mime::fileToMimeType(const fs::path& path, const std::string& defval)
{
    const std::lock_guard<std::mutex> lock(mime_mutex);
    const char* mimeType = magic_file(magicCookie, path.c_str());
    if (!mimeType || mimeType[0] == '\0') {
        return defval;
    }

    return mimeType;
}

std::string Mime::bufferToMimeType(const void* buffer, std::size_t length)
{
    return magic_buffer(magicCookie, buffer, length);
}
#endif

std::pair<bool, std::string> Mime::getMimeType(const fs::path& path, const std::string& defval)
{
    auto extension = path.extension().string();
    if (!extension.empty() && extension.at(0) == '.')
        extension.erase(0, 1); // remove leading .

    if (!extension_map_case_sensitive) {
        extension = toLower(extension);
    }

    if (std::find(ignoredExtensions.begin(), ignoredExtensions.end(), extension) != ignoredExtensions.end()) {
        log_debug("Ignoring file {} because of extension", path.string());
        return { true, "" };
    }
    std::string mimeType = getValueOrDefault(extension_mimetype_map, extension, "");
    if (mimeType.empty() && !ignore_unknown_extensions) {
#ifdef HAVE_MAGIC
        auto fileMime = fileToMimeType(path, defval);
        mimeType = fileMime.empty() ? extension : fileMime;
#else
        mimeType = defval.empty() ? extension : defval;
#endif
    }
    return { ignore_unknown_extensions, mimeType };
}
