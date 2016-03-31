/* mzh.c - Prints MZ Header for DOS MZ executable format
 * 
 * Jamie Bainbridge <jamie.bainbridge@gmail.com> 2016
 *
 * Thanks to DJ Delorie for the header description
 * http://www.delorie.com/djgpp/doc/exe/
 *
 * This work is contributed to the public domain
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct EXE {
  unsigned short signature; /* == 0x5a4D */
  unsigned short bytes_in_last_block;
  unsigned short blocks_in_file;
  unsigned short num_relocs;
  unsigned short header_paragraphs;
  unsigned short min_extra_paragraphs;
  unsigned short max_extra_paragraphs;
  unsigned short ss;
  unsigned short sp;
  unsigned short checksum;
  unsigned short ip;
  unsigned short cs;
  unsigned short reloc_table_offset;
  unsigned short overlay_number;
};

struct EXE_RELOC {
  unsigned short offset;
  unsigned short segment;
};

int main (int argc, char *argv[]) {
	int fd;
	int exe_data_start, extra_data_start;
	struct EXE exe;

	if (argc != 2) {
		printf("Usage: %s [mz binary]\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);

	if (fd == -1) {
		perror("Error opening file");
		exit(1);
	}

	read(fd, &exe, 26);

	exe_data_start = exe.header_paragraphs * 16L;

	extra_data_start = exe.blocks_in_file * 512L;
	if (exe.bytes_in_last_block)
		extra_data_start -= (512 - exe.bytes_in_last_block);

	printf("MZ Header for file %s\n\n"
		"Signature (0x5a4d)                             0x%04x\n"
		"Number of bytes used in last block:              %4d\n"
		"Number of 512b blocks in EXE file:               %4d\n"
		"No of relocation entries after header:           %4d\n"
		"No of 16b paragraphs in header:                  %4d\n"
		"No of 16b paragraphs of memory required:         %4d\n"
		"Max 16b paragraphs of mem to allocate:          %5d\n"
		"Stack Segment (SS) register value:             0x%04x\n"
		"Stack Pointer (SP) register value:             0x%04x\n"
		"Checksum (should be zero):                     0x%04x\n"
		"Instruction Pointer (IP) reg value:            0x%04x\n"
		"Code Segment (CS) register value:              0x%04x\n"
		"Offset of first relocation item:               0x%04x\n"
		"Overlay Number (usually zero):                 0x%04x\n"
		"\n",
		argv[1], exe.signature, exe.bytes_in_last_block,
		exe.blocks_in_file, exe.num_relocs, exe.header_paragraphs,
		exe.min_extra_paragraphs, exe.max_extra_paragraphs,
		exe.ss, exe.sp, exe.checksum, exe.ip, exe.cs,
		exe.reloc_table_offset, exe.overlay_number
	      );
	printf("Computed values\n\n"
		"Start of EXE data is at:                       0x%04x\n"
		"First byte after EXE data is at:             0x%06x\n"
		"Total size of EXE data:                        %6d\n"
		"\n",
		exe_data_start, extra_data_start, (extra_data_start - exe_data_start - 1)
	      );

	return 0;
}
