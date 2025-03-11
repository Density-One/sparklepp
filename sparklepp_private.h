#pragma once

class Sparkle::Private
{
public:
    Private (const juce::URL& appcastURL)
        : initialised (false),
          appcastURL (appcastURL)
    {
    }

    bool initialised;
    const juce::URL appcastURL;
private:
    Private operator= (Private) = delete;
};
