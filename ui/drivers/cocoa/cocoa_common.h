/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2013-2014 - Jason Fetters
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __COCOA_COMMON_H
#define __COCOA_COMMON_H

#include <Foundation/Foundation.h>

#ifdef HAVE_MENU
#include "../../menu/menu_setting.h"
#include "../../menu/menu_driver.h"
#endif

#ifdef HAVE_CORELOCATION
#include <CoreLocation/CoreLocation.h>
#endif

typedef enum apple_view_type {
   APPLE_VIEW_TYPE_NONE,
   APPLE_VIEW_TYPE_OPENGL_ES,
   APPLE_VIEW_TYPE_OPENGL,
   APPLE_VIEW_TYPE_VULKAN,
   APPLE_VIEW_TYPE_METAL,
} apple_view_type_t;

@protocol PlatformDelegate
#ifdef HAVE_METAL
@optional
- (void)viewDidUpdateFrame:(NSRect)rect;
#endif
@end

@protocol ApplePlatform

@property (readwrite,retain) id<PlatformDelegate> delegate;

/*!
 @brief viewHandle returns an appropriate handle for the current view type
 */
@property (readonly) id viewHandle;

/*! @brief renderView returns the current render view based on the viewType */
@property (readonly) id renderView;

/*! @brief isActive returns true if the application has focus */
@property (readonly) bool hasFocus;

@property (readwrite) apple_view_type_t viewType;

/*! @brief setVideoMode adjusts the video display to the specified mode */
- (void)setVideoMode:(gfx_ctx_mode_t)mode;

/*! @brief setCursorVisible specifies whether the cursor is visible */
- (void)setCursorVisible:(bool)v;

@end

extern id<ApplePlatform> apple_platform;

#if defined(HAVE_COCOATOUCH)
#include <UIKit/UIKit.h>

#ifdef HAVE_AVFOUNDATION
#import <AVFoundation/AVCaptureOutput.h>
#endif


/*********************************************/
/* RAMenuBase                                */
/* A menu class that displays RAMenuItemBase */
/* objects.                                  */
/*********************************************/
@interface RAMenuBase : UITableViewController
@property (nonatomic) NSMutableArray* sections;
@property (nonatomic) BOOL hidesHeaders;
@property (nonatomic) RAMenuBase* last_menu;
@property (nonatomic) UILabel *osdmessage;

- (id)initWithStyle:(UITableViewStyle)style;
- (id)itemForIndexPath:(NSIndexPath*)indexPath;

@end

typedef struct
{
   char orientations[32];
   unsigned orientation_flags;
   char bluetooth_mode[64];
} apple_frontend_settings_t;
extern apple_frontend_settings_t apple_frontend_settings;

@interface CocoaView : UIViewController<CLLocationManagerDelegate,
AVCaptureAudioDataOutputSampleBufferDelegate>
+ (CocoaView*)get;
@end

@interface RetroArch_iOS : UINavigationController<UIApplicationDelegate,
UINavigationControllerDelegate, ApplePlatform>

@property (nonatomic) UIWindow* window;
@property (nonatomic) NSString* documentsDirectory;
@property (nonatomic) RAMenuBase* mainmenu;
@property (nonatomic) int menu_count;

+ (RetroArch_iOS*)get;

- (void)showGameView;
- (void)toggleUI;
- (void)supportOtherAudioSessions;

- (void)refreshSystemConfig;
- (void)mainMenuPushPop: (bool)pushp;
- (void)mainMenuRefresh;
@end

void get_ios_version(int *major, int *minor);

#elif defined(HAVE_COCOA)
#include <AppKit/AppKit.h>

@interface CocoaView : NSView
#ifdef HAVE_CORELOCATION
<CLLocationManagerDelegate>
#endif

+ (CocoaView*)get;
#if !defined(HAVE_COCOA)
- (void)display;
#endif

@end

#endif

#define BOXSTRING(x) [NSString stringWithUTF8String:x]
#define BOXINT(x)    [NSNumber numberWithInt:x]
#define BOXUINT(x)   [NSNumber numberWithUnsignedInt:x]
#define BOXFLOAT(x)  [NSNumber numberWithDouble:x]

#if __has_feature(objc_arc)
#define RELEASE(x)   x = nil
#define BRIDGE       __bridge
#define UNSAFE_UNRETAINED __unsafe_unretained
#else
#define RELEASE(x)   [x release]; \
   x = nil
#define BRIDGE
#define UNSAFE_UNRETAINED
#endif

#endif
