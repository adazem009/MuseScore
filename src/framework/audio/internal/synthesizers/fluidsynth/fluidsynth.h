/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MUSE_AUDIO_FLUIDSYNTH_H
#define MUSE_AUDIO_FLUIDSYNTH_H

#include <memory>
#include <optional>
#include <vector>

#include "global/modularity/ioc.h"

#include "../../abstractsynthesizer.h"
#include "fluidsequencer.h"

namespace muse::audio::synth {
struct Fluid;
class FluidSynth : public AbstractSynthesizer
{
public:
    FluidSynth(const audio::AudioSourceParams& params);

    Ret addSoundFonts(const std::vector<io::path_t>& sfonts);
    void setPreset(const std::optional<midi::Program>& preset);

    std::string name() const override;
    AudioSourceType type() const override;
    void setupSound(const mpe::PlaybackSetupData& setupData) override;
    void setupEvents(const mpe::PlaybackData& playbackData) override;
    void flushSound() override;

    bool isActive() const override;
    void setIsActive(const bool isActive) override;

    msecs_t playbackPosition() const override;
    void setPlaybackPosition(const msecs_t newPosition) override;

    void revokePlayingNotes() override; // all channels

    unsigned int audioChannelsCount() const override;
    samples_t process(float* buffer, samples_t samplesPerChannel) override;
    async::Channel<unsigned int> audioChannelsCountChanged() const override;
    void setSampleRate(unsigned int sampleRate) override;

    bool isValid() const override;

private:
    struct KeyTuning {
        std::vector<int> keys;
        std::vector<double> pitches;

        void add(int key, double tuning)
        {
            keys.push_back(key);
            pitches.push_back((key * 100.0) + tuning);
        }

        int size() const
        {
            return static_cast<int>(keys.size());
        }

        void reset()
        {
            keys.clear();
            pitches.clear();
        }

        bool isEmpty() const
        {
            return keys.empty() && pitches.empty();
        }
    };

    Ret init();
    void createFluidInstance();

    bool handleEvent(const midi::Event& event);

    void toggleExpressionController();

    int setExpressionLevel(int level);
    int setControllerValue(int channel, int ctrl, int value);
    int setPitchBend(int channel, int pitchBend);

    std::shared_ptr<Fluid> m_fluid = nullptr;

    async::Channel<unsigned int> m_streamsCountChanged;

    FluidSequencer m_sequencer;
    std::set<io::path_t> m_sfontPaths;
    std::optional<midi::Program> m_preset;

    KeyTuning m_tuning;
};

using FluidSynthPtr = std::shared_ptr<FluidSynth>;
}

#endif //MUSE_AUDIO_FLUIDSYNTH_H
