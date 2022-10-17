#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define SQR(x) (x)*(x)
#define LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

char *output_file = NULL;
char *input_file = NULL;
char *palette_file = NULL;

void die(const char *fmt, ...);
void usage();
void read_args(int argc, char **argv);
uint32_t *create_palette(FILE *pfile, size_t *out_size);
void apply_palette(unsigned char *image, int image_width, int image_height, int image_components, uint32_t *palette, size_t palette_size);

int main(int argc, char **argv)
{
	read_args(argc, argv);

	int image_width, image_height, image_components;
	unsigned char *image = stbi_load(input_file, &image_width, &image_height, &image_components, 0);
	if (!image)
		die("Failed to load image");
	if (image_components < 3)
		die("Only works with images that have 3 or more components");

	FILE *pfile = fopen(palette_file, "r");
	if (!pfile)
		die("Failed to load palette file");
	size_t palette_size;
	uint32_t *palette = create_palette(pfile, &palette_size);
	if (!palette)
		die("Palette cannot be empty");

	apply_palette(image, image_width, image_height, image_components, palette, palette_size);
	stbi_write_jpg(output_file, image_width, image_height, image_components, image, 90);

	free(palette);
	fclose(pfile);
	free(image);

	return 0;
}

void die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(1);
}

void read_args(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			if ((++i) == argc)
				die("Expected an output file");
			output_file = argv[i];
		} else if (!strcmp(argv[i], "-i")) {
			if ((++i) == argc)
				die("Expected an input file");
			input_file = argv[i];
		} else if (!strcmp(argv[i], "-p")) {
			if ((++i) == argc)
				die("Expected a palette file");
			palette_file = argv[i];
		} else {
			usage();
		}
	}

	if (!input_file)
		die("You need to specify an input file");
	if (!output_file)
		die("You need to specify an output file");
	if (!palette_file)
		die("You need to specify an output file");
}

void usage()
{
	die("usage: themize [-i input_file] [-o output_file] [-p palette_file]");
}

uint32_t *create_palette(FILE *pfile, size_t *out_size)
{
	size_t palette_size = 0;
	uint32_t *palette = NULL;

	char line[512];
	while (fgets(line, LENGTH(line), pfile)) {
		palette_size++;
		if (!(palette = realloc(palette, palette_size * sizeof(*palette))))
			die("Out of memory");
		char *num_line = line[0] == '#' ? line+1 : line;
		long number = strtol(num_line, NULL, 16);
		palette[palette_size-1] = number;
	}

	*out_size = palette_size;
	return palette;
}

void apply_palette(unsigned char *input_image, int image_width, int image_height, int image_components, uint32_t *palette, size_t palette_size)
{
	for (int i = 0; i < image_width * image_height * image_components; i += image_components) {
		int image_red = input_image[i];
		int image_green = input_image[i+1];
		int image_blue = input_image[i+2];

		unsigned char new_red = 0x00;
		unsigned char new_green = 0x00;
		unsigned char new_blue = 0x00;
		size_t best_distance = SIZE_MAX;
		for (int j = 0; j < palette_size; j++) {
			int palette_red = (palette[j] >> 16) & 0xff;
			int palette_green = (palette[j] >> 8) & 0xff;
			int palette_blue = palette[j] & 0xff;

			size_t distance = SQR(palette_red-image_red) + SQR(palette_green-image_green) + SQR(palette_blue-image_blue);
			if (distance < best_distance) {
				best_distance = distance;
				new_red = palette_red;
				new_green = palette_green;
				new_blue = palette_blue;
			}
		}
		input_image[i] = new_red;
		input_image[i+1] = new_green;
		input_image[i+2] = new_blue;
	}
}
