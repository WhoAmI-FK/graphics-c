#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

int main()
{
	char filename[] = "image1.jpg";
	FILE* jinput;
	struct jpeg_decompress_struct dcinfo;
	struct jpeg_error_mgr dcjerr;
	unsigned char* raw_image;
	JSAMPROW row_pointer[1];
	int row_stride, width, height, bytes_per_pixel;

	jinput = fopen(filename,"rb");
	if(jinput == NULL)
	{
		fprintf(stderr,"Unable to open %s\n",filename);
		exit(1);
	}
	printf("File %s opened\n",filename);

	dcinfo.err = jpeg_std_error(&dcjerr);
	jpeg_create_decompress(&dcinfo);
	jpeg_stdio_src(&dcinfo,jinput);
	jpeg_read_header(&dcinfo,TRUE);
	
	jpeg_start_decompress(&dcinfo);

	width = dcinfo.output_width;
	height = dcinfo.output_height;
	bytes_per_pixel = dcinfo.output_components;
	printf("Image is %d pixels wide by %d pixels tall\n",width,height);
	printf("Image has %d bytes per pixel\n",bytes_per_pixel);
	raw_image = (unsigned char*) malloc(bytes_per_pixel*width*height);
	if(raw_image==NULL)
	{
		fprintf(stderr,"Unable to allocate memory for image\n");
		fclose(jinput);
		exit(1);
	}
	printf("Buffer created\n");
	
	row_stride = width*bytes_per_pixel;
	while(dcinfo.output_scanline < height)
	{
		row_pointer[0] = &raw_image[dcinfo.output_scanline*row_stride];
		jpeg_read_scanlines(&dcinfo,row_pointer,1);
	}
	printf("Scanlines read\n");
	jpeg_finish_decompress(&dcinfo);
	jpeg_destroy_decompress(&dcinfo);
	fclose(jinput);
	free(raw_image);
	printf("Clean up and close\n");
	return(0);
}
