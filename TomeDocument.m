//
//  TomeDocument.m
//  TomeViewerX
//
//  Created by Kevin Wojniak on 6/6/10.
//  Copyright 2010-2011 Kevin Wojniak. All rights reserved.
//

#import "TomeDocument.h"
#import "TomeFile.h"
#include "tome.h"


@implementation TomeDocument

- (NSString *)formatFourCharCode:(tome_fourcc_t)code
{
	char buf[5];
	tome_fourcc_to_string(code, buf, sizeof(buf));
	return [[[NSString alloc] initWithBytes:buf length:sizeof(buf) encoding:NSMacOSRomanStringEncoding] autorelease];
}

- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError
{
	tome_error_t err;
	tome_t *tome = tome_create([[absoluteURL path] fileSystemRepresentation], &err);
	if (err != tome_error_success) {
		NSString *errorStr = [NSString stringWithUTF8String:tome_error_to_string(err)];
		NSDictionary *userInfo = [NSDictionary dictionaryWithObject:errorStr forKey:NSLocalizedDescriptionKey];
        if (outError) {
            *outError = [NSError errorWithDomain:@"TAReaderErrorDomain" code:err userInfo:userInfo];
        }
		return NO;
	}
	
	NSMutableArray *tempFiles = [NSMutableArray array];
	for (uint32_t i=0; i<tome_get_file_count(tome); i++) {
		tome_file_t *tome_file = tome_get_file(tome, i);
		TomeFile *file = [[[TomeFile alloc] init] autorelease];
		file.id = TAArchiveFileGetID(tome_file);
		file.name = [[[NSString alloc] initWithBytes:TAArchiveFileGetName(tome_file) length:TAArchiveFileGetNameLength(tome_file) encoding:NSMacOSRomanStringEncoding] autorelease];
		NSImage *img = [[NSWorkspace sharedWorkspace] iconForFileType:NSFileTypeForHFSTypeCode(TAArchiveFileGetType(tome_file))];
		[img setSize:NSMakeSize(16.0, 16.0)];
		file.icon = img;
		file.type = [self formatFourCharCode:TAArchiveFileGetType(tome_file)];
		file.creator = [self formatFourCharCode:TAArchiveFileGetCreator(tome_file)];
		file.fullSize = TAArchiveFileGetFullSize(tome_file);
		file.compressedSize = TAArchiveFileGetFullCompressedSize(tome_file);
		file.saved = 1.0 - ((CGFloat)TAArchiveFileGetFullCompressedSize(tome_file) / (CGFloat)TAArchiveFileGetFullSize(tome_file));
		char buf[6];
		TAArchiveFileGetVersionString(tome_file, buf);
		file.version = [NSString stringWithUTF8String:buf];
		[tempFiles addObject:file];
	}

	tome_free(tome);
	[self setValue:tempFiles forKey:@"files"];
	
	return YES;
}

- (void)dealloc
{
	[files release];
	[super dealloc];
}

- (NSString *)windowNibName
{
    return @"TomeDocument";
}

- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)anItem
{
	if ([anItem action] == @selector(expand:) || [anItem action] == @selector(showInfo:)) {
		return [[arrayController selectedObjects] count] > 0 ? YES : NO;
	}
	return [super validateUserInterfaceItem:anItem];
}

- (void)expand:(id)sender
{
}

- (void)showInfo:(id)sender
{
}

@end
