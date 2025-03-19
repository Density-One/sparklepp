/*
  ==============================================================================

    sparklepp.cpp
    Created: 17 Dec 2015 5:45:43pm
    Author:  Steve Flower

  ==============================================================================
*/

#include "sparklepp.h"
#include <algorithm>

#if JUCE_WINDOWS
#include "Native/Win/include/winsparkle.h"

const char* DSAPubKey =
    "-----BEGIN PUBLIC KEY-----\n"
    "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\n"
    "vs3SR3fzNnYnC1ZJ5uUdy/FvcKlJJnU=\n"
    "-----END PUBLIC KEY-----";

Sparkle::UpdateFoundCallback Sparkle::s_updateFoundCallback = nullptr;
Sparkle::NoUpdateFoundCallback Sparkle::s_noUpdateFoundCallback = nullptr;
Sparkle::UpdateDialogDismissedCallback Sparkle::s_updateDismissedCallback = nullptr;
Sparkle::UpdateCancelledCallback Sparkle::s_updateCancelledCallback = nullptr;
Sparkle::UpdateSkippedCallback Sparkle::s_updateSkippedCallback = nullptr;
Sparkle::UpdatePostponedCallback Sparkle::s_updatePostponedCallback = nullptr;

#endif
#include "sparklepp_private.h"


Sparkle::Sparkle (const juce::URL& appcastUrl)
    : d (std::make_unique<Private> (appcastUrl))
{
#if JUCE_WINDOWS
    win_sparkle_set_dsa_pub_pem (DSAPubKey);
    win_sparkle_set_appcast_url (appcastUrl.toString (true).toUTF8());
    win_sparkle_set_automatic_check_for_updates (0);
    
#endif
}

Sparkle::~Sparkle()
{
#if JUCE_WINDOWS

    if (d->initialised)
    {
        win_sparkle_cleanup();
        d->initialised = false;
    }
#endif
}
#if JUCE_WINDOWS

bool Sparkle::isInitialized() const { return d->initialised; }



void Sparkle::forceCloseUpdateDialogs()
{
    if (d->initialised)
    {
        // Clean up Sparkle to close any active dialogs
        win_sparkle_cleanup();
        d->initialised = false;

        // Reinitialize for future use
        win_sparkle_init();
        d->initialised = true;

        Logger::writeToLog ("SPARKLE: Forced close of update dialogs");
    }
}

void Sparkle::staticUpdateDismissedCallback()
{
    Logger::writeToLog ("UPDATER: Update dialog dismissed");

    if (s_updateDismissedCallback)
        s_updateDismissedCallback();
}

void Sparkle::staticUpdateCancelledCallback()
{
    Logger::writeToLog ("UPDATER: Update cancelled by user");

    if (s_updateCancelledCallback)
        s_updateCancelledCallback();
}
void Sparkle::staticUpdateFoundCallback()
{
    Logger::writeToLog ("SPARKLE: Update found");

    // Call the application callback if registered
    if (s_updateFoundCallback)
        s_updateFoundCallback();
}

void Sparkle::staticNoUpdateFoundCallback()
{
    Logger::writeToLog ("SPARKLE: No update found");

    // Call the application callback if registered
    if (s_noUpdateFoundCallback)
        s_noUpdateFoundCallback();
}
void Sparkle::staticUpdateSkippedCallback()
{
    Logger::writeToLog ("SPARKLE: Update skipped by user");

    // Call the application callback if registered
    if (s_updateSkippedCallback)
        s_updateSkippedCallback();
}

void Sparkle::staticUpdatePostponedCallback()
{
    Logger::writeToLog ("SPARKLE: Update postponed by user");

    // Call the application callback if registered
    if (s_updatePostponedCallback)
        s_updatePostponedCallback();
}

// Update setupUpdater method
void Sparkle::setupUpdater (
    UpdateFoundCallback updateFoundCB,
    NoUpdateFoundCallback noUpdateCB,
    UpdateDialogDismissedCallback dismissedCB,
    UpdateCancelledCallback cancelledCB,
    UpdateSkippedCallback skippedCB,
    UpdatePostponedCallback postponedCB)
{
    // Store callbacks
    s_updateFoundCallback = updateFoundCB;
    s_noUpdateFoundCallback = noUpdateCB;
    s_updateDismissedCallback = dismissedCB;
    s_updateCancelledCallback = cancelledCB;
    s_updateSkippedCallback = skippedCB;
    s_updatePostponedCallback = postponedCB;
    

    if (! d->initialised)
    {
        // Set the application details
        win_sparkle_set_app_details (
            String (ProjectInfo::companyName).toWideCharPointer(),
            String (ProjectInfo::projectName).toWideCharPointer(),
            String (ProjectInfo::versionString).toWideCharPointer());

        // Set up the WinSparkle callbacks
        win_sparkle_set_did_find_update_callback (staticUpdateFoundCallback);
        win_sparkle_set_did_not_find_update_callback (staticNoUpdateFoundCallback);
        win_sparkle_set_update_dismissed_callback (staticUpdateDismissedCallback);
        win_sparkle_set_update_cancelled_callback (staticUpdateCancelledCallback);
        win_sparkle_set_update_skipped_callback (staticUpdateSkippedCallback);
        win_sparkle_set_update_postponed_callback (staticUpdatePostponedCallback);

        // Configure auto-update settings
        win_sparkle_set_update_check_interval (86400); // Daily

        // Initialize WinSparkle
        win_sparkle_init();
        d->initialised = true;
    }
}

void Sparkle::checkForUpdatesWithoutUI()
{
    if (! d->initialised)
        setupUpdater();

    win_sparkle_check_update_without_ui();
}

void Sparkle::checkForUpdatesWithUI()
{
    if (! d->initialised)
        setupUpdater();

    win_sparkle_check_update_with_ui();
}

void Sparkle::checkForUpdatesWithUIAndInstall()
{
    if (! d->initialised)
        setupUpdater();

    win_sparkle_check_update_with_ui_and_install();
}

#endif


void Sparkle::checkForUpdateInBackground()
{
#if JUCE_WINDOWS

    if (! d->initialised)
    {
        win_sparkle_init();
        d->initialised = true;
    }
#endif
}

static int isVersionNumberGreater (const String& firstVersionNumber, const String& secondVersionNumber)
{
    auto first = StringArray::fromTokens (firstVersionNumber, "vV.", "");
    auto second = StringArray::fromTokens (secondVersionNumber, "vV.", "");

    first.removeString ("");
    second.removeString ("");

    for (int i = 0; i < first.size(); ++i)
    {
        if (first[i].getIntValue() > second[i].getIntValue())
        {
            return -1;
        }
        else if (first[i].getIntValue() < second[i].getIntValue())
        {
            return 1;
        }
    }

    return 0;
}

class VersionNumberDescendingComparitor
{
public:
    VersionNumberDescendingComparitor() {}

    int compareElements (XmlElement* first, XmlElement* second)
    {
        auto firstEnclosure = first->getChildByName ("enclosure");
        auto firstVersionNumber = firstEnclosure->getStringAttribute ("sparkle:version", juce::String());

        auto secondEnclosure = second->getChildByName ("enclosure");
        auto secondVersionNumber = secondEnclosure->getStringAttribute ("sparkle:version", juce::String());

        return isVersionNumberGreater (firstVersionNumber, secondVersionNumber);
    }
};

static String getLatestVersionNumber (XmlElement* xml)
{
    if (xml == nullptr)
    {
        return String();
    }

    if (xml->hasTagName ("rss") == false)
    {
        Logger::outputDebugString ("getLatestVersionNumber: RSS tag not found");
        return String();
    }

    auto channel = xml->getChildByName ("channel");

    if (channel == nullptr)
    {
        Logger::outputDebugString ("getLatestVersionNumber: Channel not found");
        return String();
    }

    auto item = channel->getChildByName ("item");

    if (item == nullptr)
    {
        Logger::outputDebugString ("getLatestVersionNumber: No items found");
        return String();
    }

    std::unique_ptr<juce::Array<juce::XmlElement*>> items = std::make_unique<juce::Array<XmlElement*>>();

    while (item != nullptr)
    {
        items->add (item);
        item = item->getNextElementWithTagName ("item");
    }

    VersionNumberDescendingComparitor versionNumberDescendingComparitor;
    items->sort (versionNumberDescendingComparitor);

    auto latestRelease = items->getFirst()->getChildByName ("enclosure");

    if (latestRelease == nullptr)
    {
        Logger::outputDebugString ("getLatestVersionNumber: Latest release not found");
        return String();
    }

    return latestRelease->getStringAttribute ("sparkle:version", juce::String());
}

static String getCurrentInstalledVersion()
{
    return JUCEApplicationBase::getInstance()->getApplicationVersion();
}

void Sparkle::checkForUpdateInformation()
{
    if (d == nullptr)
        return;

    auto xml = d->appcastURL.readEntireXmlStream();

    if (xml == nullptr)
    {
        updaterDidNotFindUpdate();
        return;
    }

    String latestReleaseVersion = getLatestVersionNumber (xml.get());

    if ((latestReleaseVersion.isEmpty()) || isVersionNumberGreater (getCurrentInstalledVersion(), latestReleaseVersion) != 1)
    {
        updaterDidNotFindUpdate();
    }
    else
    {
        didFindValidUpdate (latestReleaseVersion);
    }
}

void Sparkle::addListener (Listener* listener)
{
    listeners.add (listener);
}

void Sparkle::removeListener (Listener* listener)
{
    listeners.remove (listener);
}

void Sparkle::didFindValidUpdate (const juce::String& version)
{
    listeners.call ([=] (Listener& l)
                    {
                        l.didFindValidUpdate (version);
                    });
}

void Sparkle::updaterDidNotFindUpdate()
{
    listeners.call (&Listener::updaterDidNotFindUpdate);
}

class AppcaspParserTests : public UnitTest
{
public:
    AppcaspParserTests()
        : UnitTest ("Appcasp Parser testing")
    {
        oneReleaseFixture = XmlDocument::parse (oneReleaseFixtureContent);
        twoReleaseFixture = XmlDocument::parse (twoReleaseFixtureContent);
    }

    void runTest() override
    {
        isVersionNumberGreaterTest();

        appcastParsingTest();
    }

    void isVersionNumberGreaterTest()
    {
        beginTest ("isVersionNumberGreater");
        expect (isVersionNumberGreater ("v1.0.0", "v0.0.1") == -1);
        expect (isVersionNumberGreater ("1.0.0", "0.0.1") == -1);
        expect (isVersionNumberGreater ("V1.0.0", "V0.0.1") == -1);
        expect (isVersionNumberGreater ("v1.0.1", "v1.0.0") == -1);
        expect (isVersionNumberGreater ("1.1.1", "v1.1.0") == -1);

        expect (isVersionNumberGreater ("v0.0.1", "v1.0.0") == 1);
        expect (isVersionNumberGreater ("0.0.1", "1.0.0") == 1);
        expect (isVersionNumberGreater ("V0.0.1", "V1.0.0") == 1);
        expect (isVersionNumberGreater ("v1.0.0", "v1.0.1") == 1);
        expect (isVersionNumberGreater ("1.1.0", "v1.1.1") == 1);

        expect (isVersionNumberGreater ("v0.0.1", "v0.0.1") == 0);
        expect (isVersionNumberGreater ("1.0.0", "1.0.0") == 0);
        expect (isVersionNumberGreater ("1.0.0", "V1.0.0") == 0);
    }

    void appcastParsingTest()
    {
        beginTest ("getLatestVersionNumber");
        expect (getLatestVersionNumber (oneReleaseFixture.get()) == String ("2.0"));

        expect (getLatestVersionNumber (twoReleaseFixture.get()) == String ("2.1"));
    }
private:
    std::unique_ptr<XmlElement> oneReleaseFixture;
    std::unique_ptr<XmlElement> twoReleaseFixture;

    const String oneReleaseFixtureContent = { "<?xml version=\"1.0\" encoding=\"utf-8\"?><rss version=\"2.0\" xmlns:sparkle=\"http://www.andymatuschak.org/xml-namespaces/sparkle\"  xmlns:dc=\"http://purl.org/dc/elements/1.1/\"><channel><title>Sparkle Test App Changelog</title><link>http://sparkle-project.org/files/sparkletestcast.xml</link><description>Most recent changes with links to updates.</description><language>en</language><item><title>Version 2.0</title><description><![CDATA[<ul><li>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</li><li>Suspendisse sed felis ac ante ultrices rhoncus.Etiam quis elit vel nibh placerat facilisis in id leo.</li><li>Vestibulum nec tortor odio, nec malesuada libero.Cras vel convallis nunc.</li><li>Suspendisse tristique massa eget velit consequat tincidunt.Praesent sodales hendrerit pretium.</li></ul>]]></description><pubDate>Sat, 26 Jul 2014 15:20 : 11 + 0000</pubDate><enclosure url=\"https://sparkle-project.org/files/Sparkle%20Test%20App.zip\" sparkle:version=\"2.0\" length=\"107758\" type=\"application/octet-stream\" sparkle:dsaSignature=\"MCwCFCdoW13VBGJWIfIklKxQVyetgxE7AhQTVuY9uQT0KOV1UEk21epBsGZMPg==\"/></item></channel></rss>" };
    const String twoReleaseFixtureContent = { "<?xml version=\"1.0\" encoding=\"utf-8\"?><rss version=\"2.0\" xmlns:sparkle=\"http://www.andymatuschak.org/xml-namespaces/sparkle\"  xmlns:dc=\"http://purl.org/dc/elements/1.1/\"><channel><title>Sparkle Test App Changelog</title><link>http://sparkle-project.org/files/sparkletestcast.xml</link><description>Most recent changes with links to updates.</description><language>en</language><item><title>Version 2.0</title><description><![CDATA[<ul><li>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</li><li>Suspendisse sed felis ac ante ultrices rhoncus.Etiam quis elit vel nibh placerat facilisis in id leo.</li><li>Vestibulum nec tortor odio, nec malesuada libero.Cras vel convallis nunc.</li><li>Suspendisse tristique massa eget velit consequat tincidunt.Praesent sodales hendrerit pretium.</li></ul>]]></description><pubDate>Sat, 26 Jul 2014 15:20 : 11 + 0000</pubDate><enclosure url=\"https://sparkle-project.org/files/Sparkle%20Test%20App.zip\" sparkle:version=\"2.0\" length=\"107758\" type=\"application/octet-stream\" sparkle:dsaSignature=\"MCwCFCdoW13VBGJWIfIklKxQVyetgxE7AhQTVuY9uQT0KOV1UEk21epBsGZMPg==\"/></item><item><title>Version 2.1</title><description><![CDATA[<ul><li>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</li><li>Suspendisse sed felis ac ante ultrices rhoncus.Etiam quis elit vel nibh placerat facilisis in id leo.</li><li>Vestibulum nec tortor odio, nec malesuada libero.Cras vel convallis nunc.</li><li>Suspendisse tristique massa eget velit consequat tincidunt.Praesent sodales hendrerit pretium.</li></ul>]]></description><pubDate>Tues, 29 Jul 2014 15:20 : 11 + 0000</pubDate><enclosure url=\"https://sparkle-project.org/files/Sparkle%20Test%20App.zip\" sparkle:version=\"2.1\" length=\"107758\" type=\"application/octet-stream\"/></item></channel></rss>" };
};

static AppcaspParserTests appcastParserTests;
