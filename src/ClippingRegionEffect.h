/*******************************************************************
* Copyright         : 2025 Bogren Digital / FStreet Media
* Author            : Bence Kovács
* License           : GNU General Public License v3.0
/******************************************************************/

#pragma once

namespace BogrenDigital
{

    class ClippingRegionEffect : public juce::ImageEffectFilter
    {
    public:
        explicit ClippingRegionEffect (juce::Rectangle<int> clipBounds);

        void applyEffect (juce::Image& sourceImage,
            juce::Graphics& destContext,
            float scaleFactor,
            float alpha) override;

        void updateBounds (juce::Rectangle<int> newBounds);

    private:
        juce::Rectangle<int> bounds;
    };

} // namespace BogrenDigital