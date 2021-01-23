//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2020 MuseScore BVBA and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================
#ifndef MU_AUDIO_SOUNDFONTSPROVIDER_H
#define MU_AUDIO_SOUNDFONTSPROVIDER_H

#include <map>
#include <mutex>

#include "isoundfontsprovider.h"

#include "modularity/ioc.h"
#include "iaudioconfiguration.h"
#include "isynthesizersregister.h"
#include "iglobalconfiguration.h"
#include "system/ifilesystem.h"
#include "async/asyncable.h"

namespace mu::audio::synth {
class SoundFontsProvider : public ISoundFontsProvider, public async::Asyncable
{
    INJECT(audio, IAudioConfiguration, configuration)
    INJECT(audio, ISynthesizersRegister, synthRegister)
    INJECT(audio, system::IFileSystem, fileSystem)

public:

    std::vector<io::path> soundFontPathsForSynth(const SynthName& synthName) const override;
    async::Notification soundFontPathsForSynthChanged(const SynthName& synth) const override;

    std::vector<io::path> soundFontPaths(SoundFontFormats formats) const override;

private:
    mutable std::mutex m_mutex;
    mutable std::map<SynthName, async::Notification > m_soundFontPathsForSynthChangedMap;
};
}

#endif // MU_AUDIO_SOUNDFONTSPROVIDER_H
