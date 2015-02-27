#include <stdio.h>

typedef int				INT32;
typedef long			INT64;
typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;
typedef unsigned long	UINT64;

#include "elf_common.h"
#include "elf64.h"

extern char _binary_test_o_start[];
extern char _binary_test_o_end[];
extern char _binary_test_o_size[];

void dump_elf64(const Elf64_Ehdr *p)
{
	int i;

	// ident
	for(i=0; i<EI_NIDENT; i++){
		printf("%x ", p->e_ident[i]);
	}
	puts("");

	printf("type=%hx\n", p->e_type);
	printf("machine=%hx\n", p->e_machine);
	printf("version=%x\n", p->e_version);
	printf("entry=%p\n", p->e_entry);
	printf("phoff=%lx\n", p->e_phoff);
	//printf("=%x\n", p->e_);


}

int load_elf(const void *elf_image, void **entry_point)
{
	Elf64_Ehdr *elf_hdr;
	unsigned char *program_hdr;
	Elf64_Phdr *program_hdr_ptr;
	unsigned long i;


	elf_hdr = (Elf64_Ehdr *)elf_image;
	program_hdr = (unsigned char *)elf_image + elf_hdr->e_phoff;

	printf("elf_image=0x%p, elf_hdr->e_phoff=0x%lx\n",
			elf_image, elf_hdr->e_phoff);
	
	printf("is_elf = %d\n", IS_ELF(*elf_hdr));

	dump_elf64(elf_hdr);
	{
		unsigned long i;
		int b;
		for(i=0; 1; i++){
			//printf("%lu %p\n", i, (char *)elf_hdr+i);
			b = (int)&(*(elf_hdr+i));
			if(i>119746688L) break;
		}
	}

	// Load every loadable ELF segment into memory
	for (i=0; i < elf_hdr->e_phnum; i++){
		program_hdr_ptr = (Elf64_Phdr *)program_hdr;

		printf("[%lu] program_hdr_ptr=0x%p\n", i, program_hdr_ptr);

		// Only consider PT_LOAD type segments
		if (program_hdr_ptr->p_type == PT_LOAD) {
			void *file_segment;
			void *mem_segment;
			void *extra_zeros;
			unsigned long extra_zeros_size;

			// Load the segment in memory
			puts("J");
			file_segment = (void *)((unsigned long)elf_image + program_hdr_ptr->p_offset);
			mem_segment = (void *)program_hdr_ptr->p_vaddr;
			printf("mem_segment=0x%p, file_segment=0x%p, filesize=%lu\n",
					mem_segment, file_segment, program_hdr_ptr->p_filesz);

			//memcpy(mem_segment, file_segment, program_hdr_ptr->p_filesz);


			// Fill memory with zero for .bss section and ...
			extra_zeros_size = program_hdr_ptr->p_memsz - program_hdr_ptr->p_filesz;
			if (extra_zeros_size > 0) {
				extra_zeros = (unsigned char *)mem_segment + program_hdr_ptr->p_filesz;
				printf("extra_zeros=0x%p, size=%lu\n",
						extra_zeros, program_hdr_ptr->p_filesz);
				//memset(extra_zeros, 0x00, extra_zeros_size);
			}
		}

		program_hdr += elf_hdr->e_phentsize;
	}

	entry_point = (void *)elf_hdr->e_entry;

	return 0;
}

int main()
{
	void *program = _binary_test_o_start;
	int size = (int)_binary_test_o_size;

	void *entry;

	printf("%p, %p, %d\n",
			_binary_test_o_start,
			_binary_test_o_end,
			size);

	load_elf(program, &entry);

	puts("Loaded");

	goto *entry;

	puts("Finish");
}

