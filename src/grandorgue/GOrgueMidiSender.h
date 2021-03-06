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

#ifndef GORGUEMIDISENDER_H
#define GORGUEMIDISENDER_H

#include "GOrgueMidiSenderData.h"

#include <wx/string.h>

class GOrgueConfigReader;
class GOrgueConfigWriter;
class GOrgueMidiMap;
class GrandOrgueFile;
struct IniFileEnumEntry;

class GOrgueMidiSender : public GOrgueMidiSenderData
{
private:
	static const struct IniFileEnumEntry m_MidiTypes[];
	GrandOrgueFile* m_organfile;
	int m_ElementID;

public:
	GOrgueMidiSender(GrandOrgueFile* organfile, MIDI_SENDER_TYPE type);
	~GOrgueMidiSender();

	void SetElementID(int id);

	void Load(GOrgueConfigReader& cfg, wxString group, GOrgueMidiMap& map);
	void Save(GOrgueConfigWriter& cfg, wxString group, GOrgueMidiMap& map);

	void SetDisplay(bool state);
	void SetKey(unsigned key, unsigned velocity);
	void ResetKey();
	void SetValue(unsigned value);
	void SetLabel(wxString text);

	bool HasChannel(midi_send_message_type type);
	bool HasKey(midi_send_message_type type);
	bool HasLowValue(midi_send_message_type type);
	bool HasHighValue(midi_send_message_type type);
	unsigned KeyLimit(midi_send_message_type type);
	unsigned LowValueLimit(midi_send_message_type type);
	unsigned HighValueLimit(midi_send_message_type type);

	void Assign(const GOrgueMidiSenderData& data);
};

#endif
