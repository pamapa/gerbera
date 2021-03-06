/*GRB*

    Gerbera - https://gerbera.io/

    metacontent_handler.h - this file is part of Gerbera.

    Copyright (C) 2020-2021 Gerbera Contributors

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

/// \file metacontent_handler.h
/// \brief Definition of the Metacontent, FanArt and Subtitle classes.

#ifndef __METADATA_CONTENT_H__
#define __METADATA_CONTENT_H__

#include <filesystem>
namespace fs = std::filesystem;

#include "metadata_handler.h"

/// \brief This class is responsible for populating filesystem based metadata
class MetacontentHandler : public MetadataHandler {
public:
    explicit MetacontentHandler(const std::shared_ptr<Context>& context);
    static bool caseSensitive;

protected:
    static fs::path getContentPath(const std::vector<std::string>& names, const std::shared_ptr<CdsObject>& item, bool isCaseSensitive, fs::path folder = "");
    static std::string expandName(const std::string& name, const std::shared_ptr<CdsObject>& item);
};

/// \brief This class is responsible for populating filesystem based album and fan art
class FanArtHandler : public MetacontentHandler {
public:
    explicit FanArtHandler(const std::shared_ptr<Context>& context);
    void fillMetadata(std::shared_ptr<CdsObject> obj) override;
    std::unique_ptr<IOHandler> serveContent(std::shared_ptr<CdsObject> obj, int resNum) override;

private:
    static std::vector<std::string> names;
    static bool initDone;
};

/// \brief This class is responsible for populating filesystem based album and fan art
class ContainerArtHandler : public MetacontentHandler {
public:
    explicit ContainerArtHandler(const std::shared_ptr<Context>& context);
    void fillMetadata(std::shared_ptr<CdsObject> obj) override;
    std::unique_ptr<IOHandler> serveContent(std::shared_ptr<CdsObject> obj, int resNum) override;

private:
    static std::vector<std::string> names;
    static bool initDone;
};

/// \brief This class is responsible for populating filesystem based subtitles
class SubtitleHandler : public MetacontentHandler {
public:
    explicit SubtitleHandler(const std::shared_ptr<Context>& context);
    void fillMetadata(std::shared_ptr<CdsObject> obj) override;
    std::unique_ptr<IOHandler> serveContent(std::shared_ptr<CdsObject> obj, int resNum) override;

private:
    static std::vector<std::string> names;
    static bool initDone;
};

/// \brief This class is responsible for reverse mapping filesystem based resources
class ResourceHandler : public MetacontentHandler {
public:
    explicit ResourceHandler(const std::shared_ptr<Context>& context);
    void fillMetadata(std::shared_ptr<CdsObject> obj) override;
    std::unique_ptr<IOHandler> serveContent(std::shared_ptr<CdsObject> obj, int resNum) override;

private:
    static std::vector<std::string> names;
    static bool initDone;
};

#endif // __METADATA_CONTENT_H__
