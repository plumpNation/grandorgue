/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2017 GrandOrgue contributors (see AUTHORS)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GORGUECACHECLEANER_H
#define GORGUECACHECLEANER_H

class GOrgueSettings;
class wxArrayString;

class GOrgueCacheCleaner
{
private:
	GOrgueSettings& m_settings;

	wxArrayString GetOrganIDList();
	wxArrayString GetArchiveIDList();
	
public:
	GOrgueCacheCleaner(GOrgueSettings& settings);
	~GOrgueCacheCleaner();

	void Cleanup();
};

#endif
