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

const char* dsa_pub_pem =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIIBvzCCATMGByqGSM44BAEwggEmAoGBAPQzfmI0SoWzc660HFFDUSaeYDiNZhVN\n"
    "YSy4JjYZAipaq/6BQp7VYMx+E6lna46hXJ0tYDeYA4KhskcM7FVFZmP4eHW9Bh45\n"
    "HVcxRrunptsNx3UHVkwIvVB8X64Qj+BW51k6B1j3374AVNHpSck8ptfGw+TdBYgm\n"
    "xoR+pJFN48llAh0AjQ725yPqxiPUIC7RN0Vzze6Vhmgt6Pb2dsrKcQKBgFAUKjpF\n"
    "NL6axapDwAIVqzG5lCextTcC11J8lU+pCp0EMV4U9NppXd5MXkJiSUbJUWkmNbSG\n"
    "Y6XaAbqpgdKF9ni8YPOXbVhhxVClb2qS5dAcR/WMmSvSKkNAcPNphKD5F/+zwSeN\n"
    "rlC+XB15+MY2k7gIcA8NvxObcwKwSdMnQKyHA4GFAAKBgQDncrBdpFsqWvXQMao8\n"
    "tgK4ZuvpcHF6vN9dqYlufcirsRbj+Q1BpCxyBDl/G28z93dIlfgDfwSU4nsSqCaJ\n"
    "0RFhB65tjVF8OnlikaD7lnjfkRRmywJQpbehQZhfHXHk95kX376SBodrNG+zez8u\n"
    "133sHrsFmVIqfG9xmWKYz5z3sg==\n"
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
    win_sparkle_set_dsa_pub_pem (dsa_pub_pem);
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
