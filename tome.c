/*
 *  tome.c
 *  TomeViewerX
 *
 *  Created by Kevin Wojniak on 6/6/10.
 *  Copyright 2010-2011 Kevin Wojniak. All rights reserved.
 *
 */

#include "tome.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define TA_ERROR_AND_EXIT(e) err = e; goto exit;


#define TAArchiveHeaderMagic	0x6B630001

#pragma pack(push, 1)

// header = 36 bytes
struct TAArchiveHeader
{
	uint32_t	magic;
	uint8_t		f1[24];
	uint32_t	count;
	uint32_t	f2;
};
typedef struct TAArchiveHeader TAArchiveHeader;

// section = 128 bytes
struct TAArchiveFileSection
{
	uint32_t	f1;
	uint16_t	id;
	uint8_t		name_length;
	uint8_t		name_data[31];
	uint32_t	type;
	uint32_t	creator;
	uint32_t	date_created;
	uint32_t	date_modified;
	uint16_t	version;
	uint32_t	finder_flags;
	uint32_t	data_size;
	uint32_t	data_offset;
	uint32_t	data_compressed_size;
	uint32_t	f3; // data fork checksum?
	uint32_t	rsrc_size;
	uint32_t	rsrc_offset;
	uint32_t	rsrc_compressed_size;
	uint32_t	f4; // resource fork checksum?
	uint8_t		f5[36];
};
typedef struct TAArchiveFileSection TAArchiveFileSection;

#pragma pack(pop)


struct tome_file_t {
	uint16_t id;
	uint8_t name_length;
	char name_data[32];
	tome_fourcc_t type;
	tome_fourcc_t creator;
	uint32_t full_size;
	uint32_t compressed_size;
	uint16_t version;
};


struct tome_t {
	uint32_t count;
	tome_file_t *files;
};


static void TASwapHeader(TAArchiveHeader *header)
{
	header->magic = ntohl(header->magic);
	header->count = ntohl(header->count);
}


static void TASwapSection(TAArchiveFileSection *section)
{
	section->id = ntohs(section->id);
	section->type = ntohl(section->type);
	section->creator = ntohl(section->creator);
	section->date_created = ntohl(section->date_created);
	section->date_modified = ntohl(section->date_modified);
	section->version = ntohs(section->version);
	section->data_size = ntohl(section->data_size);
	section->data_offset = ntohl(section->data_offset);
	section->data_compressed_size = ntohl(section->data_compressed_size);
	section->rsrc_size = ntohl(section->rsrc_size);
	section->rsrc_offset = ntohl(section->rsrc_offset);
	section->rsrc_compressed_size = ntohl(section->rsrc_compressed_size);
}


tome_t* tome_create(const char *path, tome_error_t *error)
{
	tome_t *tome = NULL;
	tome_error_t ret;
	FILE *file = NULL;
	
	if (!path) {
		ret = tome_error_invalid_argument;
		goto cleanup;
	}
	
	file = fopen(path, "rb");
	if (!file)
	{
		ret = tome_error_invalid_file;
		goto cleanup;
	}
	
	TAArchiveHeader header;
	if (fread(&header, 1, sizeof(header), file) != sizeof(header))
	{
		ret = tome_error_invalid_file;
		goto cleanup;
	}
	
	TASwapHeader(&header);
	if (header.magic != TAArchiveHeaderMagic)
	{
		ret = tome_error_invalid_file;
		goto cleanup;
	}
	
	tome = calloc(1, sizeof(tome_t));
	if (!tome)
	{
		ret = tome_error_out_of_memory;
		goto cleanup;
	}
	
	tome->files = calloc(1, sizeof(tome_file_t) * header.count);
	if (!tome->files)
	{
		ret = tome_error_out_of_memory;
		goto cleanup;
	}
	
	tome->count = header.count;
	
	int i;
	for (i=0; i<header.count; i++)
	{
		TAArchiveFileSection section;
		if (fread(&section, 1, sizeof(section), file) != sizeof(section))
		{
			ret = tome_error_invalid_file;
			goto cleanup;
		}
		
		TASwapSection(&section);
		
		tome_file_t *fileRef = tome->files + i;
		
		fileRef->id = section.id;
		fileRef->name_length = section.name_length;
		memcpy(fileRef->name_data, section.name_data, sizeof(section.name_data));
		fileRef->type = section.type;
		fileRef->creator = section.creator;
		fileRef->full_size = section.data_size + section.rsrc_size;
		fileRef->compressed_size = section.data_compressed_size + section.rsrc_compressed_size;
		fileRef->version = section.version;

		//printf("%s: %u, %u\n", fileRef->name_data, section.data_offset, section.data_compressed_size);
	}
	
	ret = tome_error_success;
	
cleanup:
	if (file)
		fclose(file);
	if (ret != tome_error_success && tome)
	{
		free(tome);
		tome = NULL;
	}
	
	if (error)
		*error = ret;
	return tome;
}


void tome_free(tome_t *tome)
{
	if (tome) {
		free(tome->files);
		free(tome);
	}
}


uint32_t tome_get_file_count(tome_t *tome)
{
	if (tome)
		return tome->count;
	return 0;
}


tome_file_t* tome_get_file(tome_t *tome, uint32_t index)
{
	if (!tome)
		return NULL;
	if (index < 0 || index >= tome->count)
		return NULL;
	return &tome->files[index];
}


unsigned short TAArchiveFileGetID(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->id;
	return 0;
}


unsigned int TAArchiveFileGetNameLength(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->name_length;
	return 0xFFFF;
}


char* TAArchiveFileGetName(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->name_data;
	return NULL;
}


tome_fourcc_t TAArchiveFileGetType(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->type;
	return 0;
}


tome_fourcc_t TAArchiveFileGetCreator(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->creator;
	return 0;
}


unsigned int TAArchiveFileGetFullSize(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->full_size;
	return 0;
}


unsigned int TAArchiveFileGetFullCompressedSize(tome_file_t *archiveFile)
{
	if (archiveFile)
		return archiveFile->compressed_size;
	return 0;
}


void TAArchiveFileGetVersionString(tome_file_t *archiveFile, char *buf)
{
	if (!archiveFile || !buf)
		return;
	unsigned short v = archiveFile->version;
	unsigned char v0 = (v & 0x000F);
	unsigned char v1 = (v & 0x00F0) >> 4;
	unsigned char v2 = (v & 0xFF00) >> 8;
	if (v0 == 0 && v1 == 0 && v2 == 0)
		sprintf(buf, "n/a");
	else if (v0)
		sprintf(buf, "%d.%d.%d", v2, v1, v0);
	else
		sprintf(buf, "%d.%d", v2, v1);
}


#if 0
tome_error_t tome_extract_file(tome_t *tome, tome_file_t *file, const char *path)
{
	tome_error_t err = tome_error;
	
	if (!tome || !file || !path)
	{
		TA_ERROR_AND_EXIT(tome_error_invalid_argument);
	}
	
exit:
	
	return err;
}
#endif


void tome_fourcc_to_string(tome_fourcc_t code, char *buf, size_t buf_size)
{
	snprintf(buf, buf_size, "%c%c%c%c", (code & 0xFF000000) >> 24, (code & 0x00FF0000) >> 16, (code & 0x0000FF00) >> 8, (code & 0x000000FF));
}


char* tome_error_to_string(tome_error_t error)
{
	switch (error) {
		case tome_error_success:
			return "No error.";
		case tome_error_invalid_argument:
			return "Invalid argument.";
		case tome_error_invalid_file:
			return "Invalid file.";
		case tome_error_out_of_memory:
			return "Out of memory.";
	}
	return "Unknown error";
}
