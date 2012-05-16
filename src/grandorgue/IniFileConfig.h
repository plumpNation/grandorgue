/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2012 GrandOrgue contributors (see AUTHORS)
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
 * MA 02111-1307, USA.
 */

#ifndef INIFILECONFIG_H
#define INIFILECONFIG_H

#include <wx/wx.h>

class wxFileConfig;

#include "GOrgueConfigReader.h"

class IniFileConfig
{
private:
	wxFileConfig& m_ODFIni;
	wxString m_LastGroup;
	bool m_LastGroupExists;

public:
	IniFileConfig(wxFileConfig& iCfg);

	wxString ReadString(GOSettingType type, wxString group, wxString key, unsigned nmax, bool required, wxString defaultValue);
};

#endif
