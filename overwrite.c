#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	char *dstfile = argv[1];
	char *srcfile = argv[2];
	size_t offset = atol(argv[3]);
	int dstfd;
	int srcfd;

	printf("dst=%s, src=%s, offset=%lu\n", dstfile, srcfile, offset);

	dstfd = open(dstfile, O_RDWR);
	srcfd = open(srcfile, O_RDONLY);

	if(dstfd < 0 || srcfd < 0){
		puts("Can't open file");
		return -1;
	}

	{
		struct stat dstinfo;
		struct stat srcinfo;
		char *dst_ptr;
		char *src_ptr;

		memset(&dstinfo, 0x00, sizeof(dstinfo));
		memset(&srcinfo, 0x00, sizeof(srcinfo));

		fstat(dstfd, &dstinfo);
		fstat(srcfd, &srcinfo);

		dst_ptr = mmap((caddr_t)0, dstinfo.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, dstfd, 0);
		src_ptr = mmap((caddr_t)0, srcinfo.st_size, PROT_READ, MAP_SHARED, srcfd, 0);

		if(dst_ptr == MAP_FAILED || src_ptr == MAP_FAILED){
			puts("Failed: memmap");
			printf("dst_ptr=%p, src_ptr=%p\n",
					dst_ptr, src_ptr);
			printf("dst_size=%lu, src_size=%lu\n",
					dstinfo.st_size,
					srcinfo.st_size);
			return -1;
		}

		printf("dst_ptr=%p, offset=%lu, src_ptr=%p, size=%lu\n",
				dst_ptr, offset, src_ptr, srcinfo.st_size);
		printf("dstsize = %lu\n", dstinfo.st_size);

		memcpy(dst_ptr + offset, src_ptr, srcinfo.st_size);

		munmap(dst_ptr, dstinfo.st_size);
		munmap(src_ptr, srcinfo.st_size);

		close(srcfd);
		close(dstfd);

	}


	return 0;
}
