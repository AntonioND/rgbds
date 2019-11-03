/*
 * This file is part of RGBDS.
 *
 * Copyright (c) 1997-2019, Carsten Sorensen and RGBDS contributors.
 *
 * SPDX-License-Identifier: MIT
 */

/* Declarations manipulating symbols */
#ifndef RGBDS_LINK_SECTION_H
#define RGBDS_LINK_SECTION_H

/* GUIDELINE: external code MUST NOT BE AWARE of the data structure used!! */

#include <stdint.h>
#include <stdbool.h>

#include "link/main.h"

#include "linkdefs.h"

struct AttachedSymbol {
	struct Symbol *symbol;
	struct AttachedSymbol *next;
};

struct Patch {
	char *fileName;
	int32_t lineNo;
	int32_t offset;
	enum PatchType type;
	int32_t rpnSize;
	uint8_t *rpnExpression;
};

struct Section {
	/* Info contained in the object files */
	char *name;
	uint16_t size;
	enum SectionType type;
	bool isAddressFixed;
	uint16_t org;
	bool isBankFixed;
	uint32_t bank;
	bool isAlignFixed;
	uint16_t alignMask;
	uint8_t *data; /* Array of size `size`*/
	uint32_t nbPatches;
	struct Patch *patches;
	/* Extra info computed during linking */
	struct Symbol **fileSymbols;
	uint32_t nbSymbols;
	struct Symbol const **symbols;
};

extern uint16_t startaddr[];
extern uint16_t maxsize[];
extern uint32_t bankranges[][2];
extern char const * const typeNames[SECTTYPE_INVALID];

/**
 * Computes a memory region's end address (last byte), eg. 0x7FFF
 * @return The address of the last byte in that memory region
 */
static inline uint16_t endaddr(enum SectionType type)
{
	return startaddr[type] + maxsize[type] - 1;
}

/**
 * Computes a memory region's number of banks
 * @return The number of banks, 1 for regions without banking
 */
static inline uint32_t nbbanks(enum SectionType type)
{
	return bankranges[type][1] - bankranges[type][0] + 1;
}

/*
 * Execute a callback for each section currently registered.
 * This is to avoid exposing the data structure in which sections are stored.
 * @param callback The function to call for each structure;
 *                 the first argument will be a pointer to the structure,
 *                 the second argument will be the pointer `arg`.
 * @param arg A pointer which will be passed to all calls to `callback`.
 */
void sect_ForEach(void (*callback)(struct Section *, void *), void *arg);

/**
 * Registers a section to be processed.
 * @param section The section to register.
 */
void sect_AddSection(struct Section *section);

/**
 * Finds a section by its name.
 * @param name The name of the section to look for
 * @return A pointer to the section, or NULL if it wasn't found
 */
struct Section *sect_GetSection(char const *name);

/**
 * `free`s all section memory that was allocated.
 */
void sect_CleanupSections(void);

/**
 * Checks if all sections meet reasonable criteria, such as max size
 */
void sect_DoSanityChecks(void);

#endif /* RGBDS_LINK_SECTION_H */
