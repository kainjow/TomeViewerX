//
//  TomeFile.h
//  TomeViewerX
//
//  Created by Kevin Wojniak on 2/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface TomeFile : NSObject {
	NSImage *icon;
	NSUInteger id;
	NSString *name;
	NSString *type;
	NSString *creator;
	NSUInteger fullSize;
	NSUInteger compressedSize;
	CGFloat saved;
	NSString *version;
}

@property (retain) NSImage *icon;
@property NSUInteger id;
@property (retain) NSString *name;
@property (retain) NSString *type;
@property (retain) NSString *creator;
@property NSUInteger fullSize;
@property NSUInteger compressedSize;
@property CGFloat saved;
@property (retain) NSString *version;

@end
