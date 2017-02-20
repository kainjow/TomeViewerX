//
//  TomeDocument.h
//  TomeViewerX
//
//  Created by Kevin Wojniak on 6/6/10.
//  Copyright 2010-2011 Kevin Wojniak. All rights reserved.
//


#import <Cocoa/Cocoa.h>


@interface TomeDocument : NSDocument
{
	IBOutlet NSArrayController *arrayController;
	NSArray *files;
}

@end
