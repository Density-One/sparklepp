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
    Sparkle(const juce::URL& appcastUrl);
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

    public:
#if JUCE_WINDOWS
    // Callback types for update status
    using UpdateFoundCallback = std::function<void()>;
    using NoUpdateFoundCallback = std::function<void()>;
    using UpdateDialogDismissedCallback = std::function<void()>; 
    using UpdateCancelledCallback = std::function<void()>; 
    using UpdateSkippedCallback = std::function<void()>;
    using UpdatePostponedCallback = std::function<void()>;

    // Setup with expanded callback options
    void setupUpdater (
        UpdateFoundCallback updateFoundCB = nullptr,
        NoUpdateFoundCallback noUpdateCB = nullptr,
        UpdateDialogDismissedCallback dismissedCB = nullptr,
        UpdateCancelledCallback cancelledCB = nullptr,
        UpdateSkippedCallback skippedCB = nullptr,
        UpdatePostponedCallback postponedCB = nullptr
        );

    // Check for updates (returns immediately, results via callbacks)
    void checkForUpdatesWithoutUI();

    // Check for updates with UI prompt
    void checkForUpdatesWithUI();

    // Check and install if available
    void checkForUpdatesWithUIAndInstall();

    void forceCloseUpdateDialogs();
    bool isInitialized() const;
    

private:
    // Static callbacks for WinSparkle
    static void staticUpdateFoundCallback();
    static void staticNoUpdateFoundCallback();
    static void staticUpdateDismissedCallback(); 
    static void staticUpdateCancelledCallback(); 
    static void staticUpdateSkippedCallback();
    static void staticUpdatePostponedCallback();

    // Store callback functions as static to access from C callbacks
    static UpdateFoundCallback s_updateFoundCallback;
    static NoUpdateFoundCallback s_noUpdateFoundCallback;
    static UpdateDialogDismissedCallback s_updateDismissedCallback;
    static UpdateCancelledCallback s_updateCancelledCallback;
    static UpdateSkippedCallback s_updateSkippedCallback;
    static UpdatePostponedCallback s_updatePostponedCallback;
#endif



private:
#ifdef __OBJC__
    // Expose ObjC type only to updater_sparkle.mm. This allows ARC to properly track its lifetime.
    SparkleDelegate* updaterDelegate;
#else
    void* updaterDelegate;
#endif
    class Private;
    std::unique_ptr<Private> d;

    std::vector<std::string> channels;
    juce::ListenerList<Listener> listeners;
};

#endif
#endif
