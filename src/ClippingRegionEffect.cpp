/*******************************************************************
* Copyright         : 2025 Bogren Digital / FStreet Media
* Author            : Bence Kovács
* License           : GNU General Public License v3.0
/******************************************************************/

namespace BogrenDigital
{

    ClippingRegionEffect::ClippingRegionEffect (juce::Rectangle<int> clipBounds)
        : bounds (clipBounds)
    {
    }

    void ClippingRegionEffect::applyEffect (juce::Image& sourceImage,
        juce::Graphics& destContext,
        float scaleFactor,
        float alpha)
    {
        destContext.saveState();
        auto scaledBounds = bounds * scaleFactor;
        destContext.reduceClipRegion (scaledBounds);
        destContext.setOpacity (alpha);
        // For right component, we need to offset the image to align with the full width
        auto drawX = scaledBounds.getX();
        if (drawX > 0) // This means it's the right component
            drawX = 0;
        destContext.drawImageAt (sourceImage, drawX, scaledBounds.getY());
        destContext.restoreState();
    }

    void ClippingRegionEffect::updateBounds (juce::Rectangle<int> newBounds)
    {
        bounds = newBounds;
    }

} // namespace BogrenDigital