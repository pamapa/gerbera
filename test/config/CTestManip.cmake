#*GRB*
#
#  Gerbera - https://gerbera.io/
#
#  CTestManip.cmake - this file is part of Gerbera.
#
#  Copyright (C) 2024-2025 Gerbera Contributors
#
#  Gerbera is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2
#  as published by the Free Software Foundation.
#
#  Gerbera is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Gerbera.  If not, see <http://www.gnu.org/licenses/>.
#
#  $Id$

set_tests_properties(${GRB_CONFIG_TESTS} PROPERTIES FIXTURES_REQUIRED GrbConfig)
MESSAGE("Copying Config Fixtures")
