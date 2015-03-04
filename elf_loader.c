typedef int				INT32;
typedef long			INT64;
typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;
typedef unsigned long	UINT64;

typedef unsigned long	size_t;

#include "elf_common.h"
#include "elf64.h"


extern char _binary_elfimage_start[];
extern char _binary_elfimage_end[];
extern char _binary_elfimage_size[];

int load_elf(const void *elf_image, void **entry_point);

void main()
{
	void *entry;

	load_elf((void *)_binary_elfimage_start, &entry);

	goto *entry;
}

void *memset(void *s, int c, size_t n)
{
	size_t i;
	unsigned char *p = s;

	for(i=0; i<n; i++){
		p[i] = c;
	}

	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	size_t i;
	unsigned char *d = dest;
	const unsigned char *s = src;

	for(i=0; i<n; i++){
		d[i] = s[i];
	}

	return dest;
}

int load_elf(const void *elf_image, void **entry_point)
{
	Elf64_Ehdr *elf_hdr;
	unsigned char *program_hdr;
	Elf64_Phdr *program_hdr_ptr;
	unsigned long i;

	elf_hdr = (Elf64_Ehdr *)elf_image;
	program_hdr = (unsigned char *)elf_image + elf_hdr->e_phoff;

	// Load every loadable ELF segment into memory
	for (i=0; i < elf_hdr->e_phnum; i++){
		program_hdr_ptr = (Elf64_Phdr *)program_hdr;

		// Only consider PT_LOAD type segments
		if (program_hdr_ptr->p_type == PT_LOAD) {
			void *file_segment;
			void *mem_segment;
			void *extra_zeros;
			unsigned long extra_zeros_size;

			// Load the segment in memory
			file_segment = (void *)((unsigned long)elf_image + program_hdr_ptr->p_offset);
			mem_segment = (void *)program_hdr_ptr->p_vaddr;
			memcpy(mem_segment, file_segment, program_hdr_ptr->p_filesz);


			// Fill memory with zero for .bss section and ...
			extra_zeros_size = program_hdr_ptr->p_memsz - program_hdr_ptr->p_filesz;
			if (extra_zeros_size > 0) {
				extra_zeros = (unsigned char *)mem_segment + program_hdr_ptr->p_filesz;
				memset(extra_zeros, 0x00, extra_zeros_size);
			}
		}

		program_hdr += elf_hdr->e_phentsize;
	}

	entry_point = (void *)elf_hdr->e_entry;

	return 0;
}

