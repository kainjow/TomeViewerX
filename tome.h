/*
 *  tome.h
 *  TomeViewerX
 *
 *  Created by Kevin Wojniak on 6/6/10.
 *  Copyright 2010-2011 Kevin Wojniak. All rights reserved.
 *
 */

#ifndef TOME_H
#define TOME_H

#include <stdint.h>
#include <stdio.h>

enum tome_error_t {
	tome_error_success			= 0,
	tome_error_invalid_argument	= 1,
	tome_error_invalid_file		= 2,
	tome_error_out_of_memory	= 3,
};
typedef enum tome_error_t tome_error_t;

typedef struct tome_t tome_t;
typedef struct tome_file_t tome_file_t;
typedef uint32_t tome_fourcc_t;

// creating and destroying an archive
tome_t* tome_create(const char *path, tome_error_t *error);
void tome_free(tome_t *tome);

// tome files
uint32_t tome_get_file_count(tome_t *tome);
tome_file_t* tome_get_file(tome_t *tome, uint32_t index);

// file properties
unsigned short TAArchiveFileGetID(tome_file_t *archiveFile);
unsigned int TAArchiveFileGetNameLength(tome_file_t *archiveFile);
char* TAArchiveFileGetName(tome_file_t *archiveFile);
tome_fourcc_t TAArchiveFileGetType(tome_file_t *archiveFile);
tome_fourcc_t TAArchiveFileGetCreator(tome_file_t *archiveFile);
unsigned int TAArchiveFileGetFullSize(tome_file_t *archiveFile);
unsigned int TAArchiveFileGetFullCompressedSize(tome_file_t *archiveFile);
void TAArchiveFileGetVersionString(tome_file_t *archiveFile, char *buf);

// extracting files
#if 0
tome_error_t tome_extract_file(tome_t *tome, tome_file_t *file, const char *path);
#endif

// utilities
void tome_fourcc_to_string(tome_fourcc_t code, char *buf, size_t buf_size);
char* tome_error_to_string(tome_error_t error);

#endif
