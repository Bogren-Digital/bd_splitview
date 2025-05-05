/*******************************************************************
* Copyright         : 2025 Bogren Digital / FStreet Media
* Author            : Bence Kovács
* License           : GNU General Public License v3.0
/******************************************************************/

#pragma once
namespace BogrenDigital
{
    /**
     * @brief This class provides a split view layout with two components that can
     * be toggled between different views. It allows the user to switch between
     * showing both components side-by-side, only the left component, or only the
     * right component.
     */
    class SplitView : public juce::Component
    {
    public:
        SplitView();
        ~SplitView() override = default;

        /**
         * @brief Sets the left or right components of the SplitView. The view does
         * not take ownership of the components.
         * @param component The component to set as the left or right child.
         * @param isLeft If true, sets the component as the left child; otherwise,
         * sets it as the right child.
         */
        void setDisplayedComponent (juce::Component* component, bool isLeft);

        /**
         * @brief Enum type that defines the view modes for the SplitView.
         * @details The SplitView can be in one of three modes:
         * - Split: Both components are visible.
         * - A: Only the left component is visible.
         * - B: Only the right component is visible.
         */
        enum class ViewMode { Split,
            A,
            B };

        /**
         * @brief Sets the text for the toggle buttons based on the view mode.
         * @param mode The view mode to set the text for.
         * @param text The text to set for the button.
         */
        void setButtonText (ViewMode mode, const juce::String& text);

    private:
        class DividerComponent : public juce::Component
        {
        public:
            explicit DividerComponent (SplitView& parent);
            void paint (juce::Graphics& g) override;
            void mouseDown (const juce::MouseEvent& e) override;
            void mouseDrag (const juce::MouseEvent& e) override;

            static constexpr float width = 4.0f;

        private:
            SplitView& owner;
            bool isDragging = false;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DividerComponent)
        };

        class RadioButtonLookAndFeel : public juce::LookAndFeel_V4
        {
        public:
            void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
            {
                auto bounds = button.getLocalBounds().toFloat();
                auto cornerSize = bounds.getHeight() * 0.2f;

                if (button.getToggleState())
                {
                    g.setColour (juce::Colours::white.withAlpha (0.2f));
                    g.fillRoundedRectangle (bounds, cornerSize);
                }

                g.setColour (button.getToggleState()
                                 ? juce::Colours::white
                                 : juce::Colours::white.withAlpha (0.7f));
                g.setFont (bounds.getHeight() * 0.6f);
                g.drawText (button.getButtonText(), bounds, juce::Justification::centred);

                if (shouldDrawButtonAsHighlighted)
                {
                    g.setColour (juce::Colours::lightgrey.withAlpha (0.2f));
                    g.fillRoundedRectangle (bounds, cornerSize);
                }
            }
        };

        class ComponentWrapper : public juce::Component
        {
        public:
            explicit ComponentWrapper();
            ~ComponentWrapper() override = default;

            void setContent (juce::Component* component);
            void resized() override;

        private:
            juce::Component* content = nullptr;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentWrapper)
        };

        void resized() override;
        void mouseMove (const juce::MouseEvent& e) override;
        void updateChildBounds();
        void updateDividerBounds();
        void setViewMode (ViewMode newMode);
        void setDividerPosition (float xPos);
        float getDividerX() const;

        RadioButtonLookAndFeel radioButtonLookAndFeel;
        juce::ToggleButton showLeftButton { "A" };
        juce::ToggleButton showRightButton { "B" };
        juce::ToggleButton showBothButton { "Split" };
        ViewMode currentMode = ViewMode::Split;

        std::unique_ptr<ComponentWrapper> leftWrapper, rightWrapper;
        std::unique_ptr<ClippingRegionEffect> leftEffect, rightEffect;
        DividerComponent divider;

        float dividerPosition = 0.5f; // 0.0 to 1.0

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplitView)
    };
} // namespace BogrenDigital