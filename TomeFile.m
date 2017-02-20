//
//  TomeFile.m
//  TomeViewerX
//
//  Created by Kevin Wojniak on 2/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "TomeFile.h"


@implementation TomeFile

@synthesize icon;
@synthesize id;
@synthesize name;
@synthesize type;
@synthesize creator;
@synthesize fullSize;
@synthesize compressedSize;
@synthesize saved;
@synthesize version;

- (void)dealloc
{
	[icon release];
	[name release];
	[type release];
	[creator release];
	[version release];
	[super dealloc];
}

@end
