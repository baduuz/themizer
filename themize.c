#include <stdio.h>
#include <strings.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "util.h"
#include "config.h"

struct palette {
	uint32_t *data;
	size_t size;
};

struct image {
	unsigned char *data;
	int width, height, components;
};

enum file_type {
	FILETYPE_JPEG,
	FILETYPE_PNG
};

static char *output_path = NULL;
static char *input_path = NULL;
static char *palette_path = NULL;
static enum file_type output_file_type = FILETYPE_PNG;

void usage();
void read_args(int argc, char **argv);
struct palette create_palette(FILE *palette_file);
void apply_palette(struct image image, struct palette palette);
void write_image(struct image image, enum file_type file_type);

int main(int argc, char **argv)
{
	read_args(argc, argv);

	struct image image;
	image.data = stbi_load(input_path, &image.width, &image.height, &image.components, 0);
	if (!image.data)
		die("Failed to load image");
	if (image.components < 3)
		die("Only works with images that have 3 or more components");

	FILE *palette_file = fopen(palette_path, "r");
	if (!palette_file)
		die("Failed to load palette file");
	struct palette palette = create_palette(palette_file);
	fclose(palette_file);
	if (!palette.data)
		die("Palette cannot be empty");

	apply_palette(image, palette);
	free(palette.data);

	write_image(image, output_file_type);
	free(image.data);


	return 0;
}

void usage()
{
	die("usage: themize [-i input_path] [-o output_path] [-p palette_path]");
}

void read_args(int argc, char **argv)
{
	char *output_extension = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			if ((++i) == argc)
				die("Expected an output file");
			output_path = argv[i];
		} else if (!strcmp(argv[i], "-i")) {
			if ((++i) == argc)
				die("Expected an input file");
			input_path = argv[i];
		} else if (!strcmp(argv[i], "-p")) {
			if ((++i) == argc)
				die("Expected a palette file");
			palette_path = argv[i];
		} else if (!strcmp(argv[i], "-t")) {
			if ((++i) == argc)
				die("Expected an output filetype");
			output_extension = argv[i];
		} else {
			usage();
		}
	}

	if (!input_path)
		die("You need to specify an input file");
	if (!output_path)
		die("You need to specify an output file");
	if (!palette_path)
		die("You need to specify an output file");

	if (!output_extension) {
		output_extension = extension(output_path);
		if (!output_extension)
			die("Couldn't determine filetype");
	}

	if (!strcasecmp(output_extension, "png"))
		output_file_type = FILETYPE_PNG;
	else if (!strcasecmp(output_extension, "jpg") || !strcasecmp(output_extension, "jpeg"))
		output_file_type = FILETYPE_JPEG;
	else
		die("Filetype not supported: %s", output_extension);

}

struct palette create_palette(FILE *palette_file)
{
	struct palette palette = {0};

	char line[512];
	while (fgets(line, LENGTH(line), palette_file)) {
		palette.size++;
		if (!(palette.data = realloc(palette.data, palette.size * sizeof(*palette.data))))
			die("Out of memory");
		char *num_line = line[0] == '#' ? line+1 : line;
		long number = strtol(num_line, NULL, 16);
		palette.data[palette.size-1] = number;
	}

	return palette;
}

void apply_palette(struct image image, struct palette palette)
{
	for (int i = 0; i < image.width * image.height * image.components; i += image.components) {
		int image_red = image.data[i];
		int image_green = image.data[i+1];
		int image_blue = image.data[i+2];

		unsigned char new_red = 0x00;
		unsigned char new_green = 0x00;
		unsigned char new_blue = 0x00;
		size_t best_distance = SIZE_MAX;
		for (int j = 0; j < palette.size; j++) {
			int palette_red = (palette.data[j] >> 16) & 0xff;
			int palette_green = (palette.data[j] >> 8) & 0xff;
			int palette_blue = palette.data[j] & 0xff;

			size_t distance = SQR(palette_red-image_red) + SQR(palette_green-image_green) + SQR(palette_blue-image_blue);
			if (distance < best_distance) {
				best_distance = distance;
				new_red = palette_red;
				new_green = palette_green;
				new_blue = palette_blue;
			}
		}
		image.data[i] = new_red;
		image.data[i+1] = new_green;
		image.data[i+2] = new_blue;
	}
}

void write_image(struct image image, enum file_type file_type)
{
	switch (file_type) {
	case FILETYPE_JPEG:
		if (!stbi_write_jpg(output_path, image.width, image.height, image.components, image.data, jpeg_quality))
			goto write_failed;
		return;
	case FILETYPE_PNG:
		if (!stbi_write_png(output_path, image.width, image.height, image.components, image.data, image.width * image.components))
			goto write_failed;
		return;
	}
write_failed:
	die("Failed to write to output image");
}
