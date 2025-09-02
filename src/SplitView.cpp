/*******************************************************************
* Copyright         : 2025 Bogren Digital / FStreet Media
* Author            : Bence Kovács
* License           : GNU General Public License v3.0
/******************************************************************/

namespace BogrenDigital
{
    SplitView::ComponentWrapper::ComponentWrapper()
    {
        setOpaque (false);
    }

    void SplitView::ComponentWrapper::setContent (juce::Component* component)
    {
        if (content)
            removeChildComponent (content);

        content = component;
        if (content)
        {
            addAndMakeVisible (content);
            resized();
        }
    }

    void SplitView::ComponentWrapper::resized()
    {
        if (content)
            content->setBounds (getLocalBounds());
    }

    SplitView::DividerComponent::DividerComponent (SplitView& parent)
        : owner (parent)
    {
        setMouseCursor (juce::MouseCursor::LeftRightResizeCursor);
    }

    void SplitView::DividerComponent::paint (juce::Graphics& g)
    {
        auto colour = (currentStyle == Style::Light) ? juce::Colours::white : juce::Colours::black;
        g.setColour (colour.withAlpha (0.5f));
        g.fillAll();
    }

    void SplitView::DividerComponent::mouseDown (const juce::MouseEvent&)
    {
        isDragging = true;
    }

    void SplitView::DividerComponent::mouseDrag (const juce::MouseEvent& e)
    {
        if (isDragging)
        {
            auto xPos = e.getEventRelativeTo (&owner).position.x;
            owner.setDividerPosition (xPos);
        }
    }

    void SplitView::DividerComponent::setStyle (Style style)
    {
        currentStyle = style;
    }

    SplitView::SplitView()
        : divider (*this)
    {
        setOpaque (false);
        addAndMakeVisible (divider);

        showLeftButton.setLookAndFeel (&radioButtonLookAndFeel);
        showRightButton.setLookAndFeel (&radioButtonLookAndFeel);
        showBothButton.setLookAndFeel (&radioButtonLookAndFeel);

        addAndMakeVisible (showLeftButton);
        addAndMakeVisible (showRightButton);
        addAndMakeVisible (showBothButton);

        showBothButton.setRadioGroupId (1);
        showLeftButton.setRadioGroupId (1);
        showRightButton.setRadioGroupId (1);

        showBothButton.setToggleState (true, juce::dontSendNotification);

        showLeftButton.onClick = [this]() { setViewMode (ViewMode::A); };
        showRightButton.onClick = [this]() { setViewMode (ViewMode::B); };
        showBothButton.onClick = [this]() { setViewMode (ViewMode::Split); };

        setViewMode (ViewMode::Split);

        addMouseListener (this, true);
    }

    void SplitView::resized()
    {
        updateChildBounds();

        if (currentMode == ViewMode::Split)
        {
            auto divX = (int) getDividerX();
            if (leftWrapper && leftEffect)
                leftEffect->updateBounds (getLocalBounds().withRight (divX));

            if (rightWrapper && rightEffect)
                rightEffect->updateBounds (getLocalBounds().withLeft (divX));
        }

        repaint();
    }

    void SplitView::paintOverChildren (juce::Graphics& g)
    {
        if (currentMode != ViewMode::Split)
        {
            return;
        }

        const auto dividerBounds = divider.getBounds();
        const auto centerX = dividerBounds.getCentreX();
        const auto centerY = dividerBounds.getCentreY();

        constexpr auto circleRadius = 16.0f;
        const juce::Rectangle circleBounds(centerX - circleRadius,
                                           centerY - circleRadius,
                                           circleRadius * 2,
                                           circleRadius * 2);

        const auto currentStyle = divider.getStyle();
        const auto bgColour = (currentStyle == Style::Light)
                        ? juce::Colours::darkgrey.withAlpha(0.8f)
                        : juce::Colours::lightgrey.withAlpha(0.8f);
        const auto arrowColour = (currentStyle == Style::Light)
                           ? juce::Colours::white
                           : juce::Colours::black;

        g.setColour(bgColour);
        g.fillEllipse(circleBounds);

        g.setColour(arrowColour);

        constexpr auto arrowSize = 5.0f;
        constexpr auto spacing = 6.0f;
        constexpr auto lineThickness = 2.0f;
        
        juce::Path leftArrow;
        leftArrow.startNewSubPath(centerX - spacing, centerY - arrowSize);
        leftArrow.lineTo(centerX - spacing - arrowSize, centerY);
        leftArrow.lineTo(centerX - spacing, centerY + arrowSize);
        g.strokePath(leftArrow, juce::PathStrokeType(lineThickness, 
                                                     juce::PathStrokeType::mitered,
                                                     juce::PathStrokeType::rounded));
        
        juce::Path rightArrow;
        rightArrow.startNewSubPath(centerX + spacing, centerY - arrowSize);
        rightArrow.lineTo(centerX + spacing + arrowSize, centerY);
        rightArrow.lineTo(centerX + spacing, centerY + arrowSize);
        g.strokePath(rightArrow, juce::PathStrokeType(lineThickness, 
                                                      juce::PathStrokeType::mitered,
                                                      juce::PathStrokeType::rounded));
    }

    void SplitView::setDividerPosition (float xPos)
    {
        dividerPosition = juce::jlimit (0.01f, 0.99f, xPos / static_cast<float> (getWidth()));

        auto divX = (int) getDividerX();
        if (leftEffect)
            leftEffect->updateBounds (getLocalBounds().withRight (divX));
        if (rightEffect)
            rightEffect->updateBounds (getLocalBounds().withLeft (divX));
        updateDividerBounds();
        repaint();
    }

    float SplitView::getDividerX() const
    {
        return dividerPosition * static_cast<float> (getWidth());
    }

    void SplitView::setButtonText (ViewMode mode, const juce::String& text)
    {
        switch (mode)
        {
            case ViewMode::A:
                showLeftButton.setButtonText (text);
                break;
            case ViewMode::B:
                showRightButton.setButtonText (text);
                break;
            case ViewMode::Split:
                showBothButton.setButtonText (text);
                break;
        }
    }

    void SplitView::setButtonStyle (Style style)
    {
        radioButtonLookAndFeel.setStyle (style);
        showLeftButton.repaint();
        showRightButton.repaint();
        showBothButton.repaint();
    }

    void SplitView::setDividerStyle (Style style)
    {
        divider.setStyle (style);
        divider.repaint();
    }

    void SplitView::setDisplayedComponent (juce::Component* component, bool isLeft)
    {
        auto& wrapper = isLeft ? leftWrapper : rightWrapper;
        if (!wrapper)
        {
            wrapper = std::make_unique<ComponentWrapper>();
            addAndMakeVisible (wrapper.get());
            wrapper->toBehind (&divider);
        }
        wrapper->setContent (component);
        updateChildBounds();
        setViewMode (currentMode);
    }

    void SplitView::mouseMove (const juce::MouseEvent& e)
    {
        if (currentMode == ViewMode::Split && leftWrapper && rightWrapper)
        {
            auto mouseScreenX = e.getScreenPosition().x;
            auto dividerScreenX = divider.getScreenX() + DividerComponent::width / 2.0f;

            if (mouseScreenX < dividerScreenX)
            {
                rightWrapper->toBack();
            }
            else
            {
                leftWrapper->toBack();
            }
        }
    }

    void SplitView::setViewMode (ViewMode newMode)
    {
        currentMode = newMode;
        const auto toggleComp = [] (juce::Component* comp, bool shouldBeOn) {
            if (comp)
            {
                comp->setVisible (shouldBeOn);
                comp->setEnabled (shouldBeOn);
            }
        };

        // Reset all effects first
        if (leftWrapper)
            leftWrapper->setComponentEffect (nullptr);
        if (rightWrapper)
            rightWrapper->setComponentEffect (nullptr);

        switch (currentMode)
        {
            case ViewMode::A:
                toggleComp (leftWrapper.get(), true);
                toggleComp (rightWrapper.get(), false);
                toggleComp (&divider, false);
                break;

            case ViewMode::B:
                toggleComp (leftWrapper.get(), false);
                toggleComp (rightWrapper.get(), true);
                toggleComp (&divider, false);
                break;

            case ViewMode::Split:
                auto divX = (int) getDividerX();
                toggleComp (leftWrapper.get(), true);
                toggleComp (rightWrapper.get(), true);
                toggleComp (&divider, true);

                if (leftWrapper)
                {
                    if (!leftEffect)
                        leftEffect.reset (new ClippingRegionEffect (getLocalBounds().withRight (divX)));
                    leftWrapper->setComponentEffect (leftEffect.get());
                }

                if (rightWrapper)
                {
                    if (!rightEffect)
                        rightEffect.reset (new ClippingRegionEffect (getLocalBounds().withLeft (divX)));
                    rightWrapper->setComponentEffect (rightEffect.get());
                }
                break;
        }

        if ((leftWrapper != nullptr && leftWrapper->getWidth() != getWidth())
            || (rightWrapper != nullptr && rightWrapper->getWidth() != getWidth()))
        {
            resized();
        }
        repaint();
    }

    void SplitView::updateChildBounds()
    {
        const auto updateCompBounds = [this] (juce::Component* comp) {
            if (comp && comp->isVisible() && comp->getWidth() != getWidth())
            {
                comp->setBounds (getLocalBounds());
            }
        };

        updateCompBounds (leftWrapper.get());
        updateCompBounds (rightWrapper.get());

        const int buttonHeight = 24;
        const int buttonWidth = 60;
        const int spacing = 5;
        const int totalWidth = buttonWidth * 3 + spacing * 2;
        const int startX = (getWidth() - totalWidth) / 2;
        const int buttonY = 5;

        showLeftButton.setBounds (startX, buttonY, buttonWidth, buttonHeight);
        showBothButton.setBounds (startX + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight);
        showRightButton.setBounds (startX + (buttonWidth + spacing) * 2, buttonY, buttonWidth, buttonHeight);

        updateDividerBounds();
    }

    void SplitView::updateDividerBounds()
    {
        auto divX = getDividerX();
        divider.setBounds (divX - DividerComponent::width / 2.0f, 0, DividerComponent::width, getHeight());
    }

} // namespace BogrenDigital