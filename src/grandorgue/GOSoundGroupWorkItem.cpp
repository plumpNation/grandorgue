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

#include "GOSoundGroupWorkItem.h"

#include "GOSoundEngine.h"
#include "GOSoundWindchestWorkItem.h"
#include "mutex_locker.h"

GOSoundGroupWorkItem::GOSoundGroupWorkItem(GOSoundEngine& sound_engine, unsigned samples_per_buffer) :
	GOSoundBufferItem(samples_per_buffer, 2),
	m_engine(sound_engine),
	m_Condition(m_Mutex),
	m_ActiveCount(0),
	m_Done(0),
	m_Stop(false)
{
}

void GOSoundGroupWorkItem::Reset()
{
	GOMutexLocker locker(m_Mutex);
	m_Done = 0;
	m_ActiveCount = 0;
	m_Stop = false;
}

void GOSoundGroupWorkItem::Clear()
{
	m_Active.Clear();
	m_Release.Clear();
}

void GOSoundGroupWorkItem::Add(GO_SAMPLER* sampler)
{
	if (sampler->is_release)
		m_Release.Put(sampler);
	else
		m_Active.Put(sampler);
}

void GOSoundGroupWorkItem::ProcessList(GOSoundSamplerList& list, float* output_buffer)
{
	for (GO_SAMPLER* sampler = list.Get(); sampler; sampler = list.Get())
	{
		if (m_engine.ProcessSampler(output_buffer, sampler, m_SamplesPerBuffer, sampler->windchest->GetVolume()))
			Add(sampler);
	}
}

void GOSoundGroupWorkItem::ProcessReleaseList(GOSoundSamplerList& list, float* output_buffer)
{
	for (GO_SAMPLER* sampler = list.Get(); sampler; sampler = list.Get())
	{
		if (m_Stop && sampler->time + 2000 < m_engine.GetTime())
		{
			if (sampler->drop_counter++ > 3)
			{
				m_engine.ReturnSampler(sampler);
				continue;
			}
		}
		sampler->drop_counter = 0;
		if (m_engine.ProcessSampler(output_buffer, sampler, m_SamplesPerBuffer, sampler->windchest->GetVolume()))
			Add(sampler);
	}
}

unsigned GOSoundGroupWorkItem::GetGroup()
{
	return AUDIOGROUP;
}

unsigned GOSoundGroupWorkItem::GetCost()
{
	return m_Active.GetCount() + m_Release.GetCount();
}

bool GOSoundGroupWorkItem::GetRepeat()
{
	return true;
}

void GOSoundGroupWorkItem::Run()
{
	if (m_Done == 3)
		return;
	{
		GOMutexLocker locker(m_Mutex);
		if (m_Done == 0)
		{
			m_Active.Move();
			m_Release.Move();
			m_Done = 1;
		}
		else
		{ 
			if (!m_Active.Peek() && !m_Release.Peek())
				return;
		}
		m_ActiveCount++;
	}
	float buffer[m_SamplesPerBuffer * 2];
	memset(buffer, 0, m_SamplesPerBuffer * 2 * sizeof(float));
	ProcessList(m_Active, buffer);
	ProcessReleaseList(m_Release, buffer);
	{
		GOMutexLocker locker(m_Mutex);
		if (m_Done == 1)
		{
			memcpy(m_Buffer, buffer, m_SamplesPerBuffer * 2 * sizeof(float));
			m_Done = 2;
		}
		else
		{
			for(unsigned i = 0; i < m_SamplesPerBuffer * 2; i++)
				m_Buffer[i] += buffer[i];
		}
		m_ActiveCount--;
		if (!m_ActiveCount)
		{
			m_Done = 3;
			m_Condition.Broadcast();
		}
	}
}

void GOSoundGroupWorkItem::Exec()
{
	m_Stop = true;
	Run();
}

void GOSoundGroupWorkItem::Finish(bool stop)
{
	if (stop)
		m_Stop = true;
	Run();
	if (m_Done == 3)
		return;

	{
		GOMutexLocker locker(m_Mutex);
		if (m_Done != 3)
			m_Condition.Wait();
	}
}
