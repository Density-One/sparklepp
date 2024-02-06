/*
  ==============================================================================

    sparklepp.h
    Created: 28 Sep 2015 12:22:34pm
    Author:  Stephen Flower

  ==============================================================================
*/

/*******************************************************************************
The block below describes the properties of this module, and is read by
the Projucer to automatically generate project code that uses it.
For details about the syntax and how to create or use a module, see the
JUCE Module Format.txt file.


BEGIN_JUCE_MODULE_DECLARATION

ID:               sparklepp
vendor:           
version:          0.1.0
name:             Sparkle
description:      Juce module for the Sparkle update library.
website:          juce_core
license:          https://github.com/sparkle-project/Sparkle/blob/master/LICENSE

dependencies:     
OSXFrameworks:    Foundation Sparkle
iOSFrameworks:    

END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#ifndef SPARKLEPP_H_INCLUDED
#define SPARKLEPP_H_INCLUDED

#if SPARKLE_UPDATER_ENABLE

#include "JuceHeader.h"
#include <vector>
#include <string>

#ifdef __OBJC__
@class SparkleDelegate;
#endif

class Sparkle
{
public:
    Sparkle();
    Sparkle (std::vector<std::string> channels);
    ~Sparkle();

    /* This will asynchronously launch an update GUI if an update is available */
    void checkForUpdateInBackground();

    /* This will asynchronously check if an update is available.
     * If an update is available the didFindValidUpdate method will becalled on 
     * listeners.
     * If an update is not available the updaterDidNotFindUpdate method will 
     * be called on listeners.
     */
    void checkForUpdateInformation();

    class Listener
    {
    public:
        virtual ~Listener() = default;

        virtual void didFindValidUpdate (const juce::String& version) = 0;
        virtual void updaterDidNotFindUpdate() = 0;
        virtual void didAbortWithError (const juce::String& error) = 0;
        virtual void failedToDownload (const juce::String& version, const juce::String& explanation) = 0;
    };

    void addListener (Listener* listener);
    void removeListener (Listener* listener);

    /* internal */
    void didFindValidUpdate (const juce::String& version);
    void updaterDidNotFindUpdate();
    void didAbortWithError (const juce::String& error);
    void failedToDownload (const juce::String& version, const juce::String& explanation);
    void setChannels (std::vector<std::string> allowedChannels)
    {
        channels = std::move (allowedChannels);
    }
    [[nodiscard]] std::vector<std::string> allowedChannelsForUpdater() const
    {
        return channels;
    }

private:
#ifdef __OBJC__
    // Expose ObjC type only to updater_sparkle.mm. This allows ARC to properly track its lifetime.
    SparkleDelegate* updaterDelegate;
#else
    void* updaterDelegate;
#endif

    std::vector<std::string> channels;
    juce::ListenerList<Listener> listeners;
};

#endif
#endif
