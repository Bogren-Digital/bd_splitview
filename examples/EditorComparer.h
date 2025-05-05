/*******************************************************************
* Copyright         : 2025 Bogren Digital / FStreet Media
* Author            : Bence Kovács
* License           : GNU General Public License v3.0
/******************************************************************/

#pragma once

#include <bd_splitview/bd_splitview.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace BogrenDigital
{
    class EditorComparer : public juce::AudioProcessorEditor
    {
    public:
        EditorComparer (juce::AudioProcessor& proc, std::unique_ptr<juce::AudioProcessorEditor> editor1, std::unique_ptr<juce::AudioProcessorEditor> editor2)
            : juce::AudioProcessorEditor (proc), leftEditor (std::move (editor1)), rightEditor (std::move (editor2))
        {
            addAndMakeVisible (comparer);
            comparer.setDisplayedComponent (leftEditor.get(), true);
            comparer.setDisplayedComponent (rightEditor.get(), false);

            const auto minWidth = 500;
            const auto minHeight = std::ceil (minWidth * getAspectRatio());
            const auto maxWidth = minWidth * 2.39f;
            const auto maxHeight = std::ceil (maxWidth * getAspectRatio());
            const auto editors = std::array<juce::AudioProcessorEditor*, 3> { this, leftEditor.get(), rightEditor.get() };
            for (auto* editor : editors)
            {
                editor->setSize (minWidth, static_cast<int> (minHeight));
                editor->setResizeLimits (minWidth, static_cast<int> (minHeight), static_cast<int> (maxWidth), static_cast<int> (maxHeight));
                editor->setResizable (true, true);
            }
        }

        float getAspectRatio() const { return 0.7131687243f; }

        void resized() override
        {
            auto bounds = getLocalBounds();
            const auto targetHeight = static_cast<int> (std::ceil ((float) bounds.getWidth() * getAspectRatio()));
            if (bounds.getHeight() != targetHeight)
            {
                setSize (bounds.getWidth(), targetHeight);
            }
            else
            {
                comparer.setBounds (bounds);
            }
        }

    private:
        std::unique_ptr<juce::AudioProcessorEditor> leftEditor, rightEditor;
        SplitView comparer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorComparer)
    };
} // namespace BogrenDigital