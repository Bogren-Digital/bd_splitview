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

            setEditorConstraints();
        }

        void setAspectRatio (float newAspectRatio)
        {
            if (aspectRatio != newAspectRatio)
            {
                aspectRatio = newAspectRatio;
                setEditorConstraints();
                resized();
            }
        }
        float getAspectRatio() const { return aspectRatio; }

        void setMinWidth (float newMinWidth)
        {
            if (minWidth != newMinWidth)
            {
                minWidth = newMinWidth;
                setEditorConstraints();
                resized();
            }
        }
        float getMinWidth() const { return minWidth; }
        
        void setMaxWidthMultiplier (float newMaxWidthMultiplier)
        {
            if (maxWidthMultiplier != newMaxWidthMultiplier)
            {
                maxWidthMultiplier = newMaxWidthMultiplier;
                setEditorConstraints();
                resized();
            }
        }
        float getMaxWidthMultiplier() const { return maxWidthMultiplier; }

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
        float aspectRatio = 0.7131687243f;
        float minWidth = 500.0f;
        float maxWidthMultiplier = 2.39f;

        void setEditorConstraints()
        {
            const auto minWidth = getMinWidth();
            const auto minHeight = std::ceil (minWidth * getAspectRatio());
            const auto maxWidth = minWidth * getMaxWidthMultiplier();
            const auto maxHeight = std::ceil (maxWidth * getAspectRatio());
            const auto editors = std::array<juce::AudioProcessorEditor*, 3> { this, leftEditor.get(), rightEditor.get() };
            for (auto* editor : editors)
            {
                editor->setSize (minWidth, static_cast<int> (minHeight));
                editor->setResizeLimits (minWidth, static_cast<int> (minHeight), static_cast<int> (maxWidth), static_cast<int> (maxHeight));
                editor->setResizable (true, true);
            }
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorComparer)
    };
} // namespace BogrenDigital