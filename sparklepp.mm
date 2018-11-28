/*
  ==============================================================================

    Sparkle.mm
    Created: 17 Dec 2015 5:45:43pm
    Author:  Steve Flower

  ==============================================================================
*/

#import <Sparkle/Sparkle.h>

#include "sparklepp.h"

@interface SparkleDelegate : NSObject <SUUpdaterDelegate> {
    Sparkle* delegateHandler;
}

- (id) init:(Sparkle*)handler;

/*!
 Called when a valid update is found by the update driver.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 */
- (void)updater:(SUUpdater *)updater didFindValidUpdate:(SUAppcastItem *)item;

/*!
 Called when a valid update is not found.
 
 \param updater The SUUpdater instance.
 */
- (void)updaterDidNotFindUpdate:(SUUpdater *)updater;

/*!
 Called after the specified update failed to download.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that failed to download.
 \param error The error generated by the failed download.
 */
- (void)updater:(SUUpdater *)updater failedToDownloadUpdate:(SUAppcastItem *)item error:(NSError *)error;

/*!
 Called when the user clicks the cancel button while and update is being downloaded.
 
 \param updater The SUUpdater instance.
 */
- (void)userDidCancelDownload:(SUUpdater *)updater;

/*!
 Called after an update is aborted due to an error.
 
 \param updater The SUUpdater instance.
 \param error The error that caused the abort
 */
- (void)updater:(SUUpdater *)updater didAbortWithError:(NSError *)error;


@end

@implementation SparkleDelegate
- (id) init:(Sparkle*)handler
{
    self = [super init];
    if (self) {
        delegateHandler = handler;
    }
    return self;
}

/*!
 Called when a valid update is found by the update driver.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that is proposed to be installed.
 */
- (void)updater:(SUUpdater *)updater didFindValidUpdate:(SUAppcastItem *)item
{
    delegateHandler->didFindValidUpdate();
}

/*!
 Called when a valid update is not found.
 
 \param updater The SUUpdater instance.
 */
- (void)updaterDidNotFindUpdate:(SUUpdater *)updater
{
    delegateHandler->updaterDidNotFindUpdate();
}

/*!
 Called after the specified update failed to download.
 
 \param updater The SUUpdater instance.
 \param item The appcast item corresponding to the update that failed to download.
 \param error The error generated by the failed download.
 */
- (void)updater:(SUUpdater *)updater failedToDownloadUpdate:(SUAppcastItem *)item error:(NSError *)error
{
    delegateHandler->updaterDidNotFindUpdate();
}

/*!
 Called when the user clicks the cancel button while and update is being downloaded.
 
 \param updater The SUUpdater instance.
 */
- (void)userDidCancelDownload:(SUUpdater *)updater
{
    delegateHandler->updaterDidNotFindUpdate();
}

/*!
 Called after an update is aborted due to an error.
 
 \param updater The SUUpdater instance.
 \param error The error that caused the abort
 */
- (void)updater:(SUUpdater *)updater didAbortWithError:(NSError *)error
{
    delegateHandler->updaterDidNotFindUpdate();
}
@end

class Sparkle::Private {
public:
    SUUpdater* updater;
    SparkleDelegate* delegateHandler;
};

Sparkle::Sparkle (const juce::URL& appcastURL)
{
    d = new Private;
    d->updater = [[SUUpdater sharedUpdater] retain];
    NSURL* url = [NSURL URLWithString:
                  [NSString stringWithUTF8String: appcastURL.toString(true).toRawUTF8()]];
    [d->updater setFeedURL: url];
    
    [d->updater setDelegate: [[SparkleDelegate alloc] init: this]];
}

Sparkle::~Sparkle()
{
    [d->updater release];
}

void Sparkle::checkForUpdateInBackground()
{
    [d->updater checkForUpdatesInBackground];
}

void Sparkle::checkForUpdateInformation()
{
    [d->updater checkForUpdateInformation];
}

void Sparkle::didFindValidUpdate()
{
    listeners.call (&Listener::didFindValidUpdate);
}

void Sparkle::updaterDidNotFindUpdate()
{
    listeners.call (&Listener::updaterDidNotFindUpdate);
}

void Sparkle::addListener (Listener* listener)
{
    listeners.add (listener);
}

void Sparkle::removeListener (Listener* listener)
{
    listeners.remove (listener);
}
