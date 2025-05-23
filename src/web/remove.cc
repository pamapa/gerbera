/*MT*

    MediaTomb - http://www.mediatomb.cc/

    web/remove.cc - this file is part of MediaTomb.

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

/// \file web/remove.cc
#define GRB_LOG_FAC GrbLogFacility::web

#include "pages.h" // API

#include "content/content.h"
#include "database/database.h"
#include "util/tools.h"

const std::string_view Web::Remove::PAGE = "remove";

bool Web::Remove::processPageAction(Json::Value& element, const std::string& action)
{
    int objectID = intParam("object_id");
    bool all = intParam("all");

    std::shared_ptr<CdsObject> obj;
    try {
        obj = database->loadObject(objectID);
    } catch (const std::runtime_error&) {
        log_error("Trying to remove an object ID which is no longer in the database! {}", objectID);
        return true;
    }
    content->removeObject(nullptr, obj, "", true, all);
    return true;
}
