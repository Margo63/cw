#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define PNG_DEBUG 3
#include </usr/local/include/png.h>

struct My_png{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};
struct Point{
        int x;
        int y;
}typedef Point;

struct my_Color{
	int r;
	int g;
	int b;
	//int alpha;
}typedef my_Color;
void read_png_file(char *file_name, struct My_png *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep)header, 0, 8)){
       	puts("type_error");
	 // Some error handling: file is not recognized as a PNG
    }

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_read_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

//    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during read_image
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}


void write_png_file(char *file_name, struct My_png *image) {
    int x,y;
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_write_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing header
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing bytes
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during end of write
    }

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);
}

void draw_pixel(int x,int y,struct My_png *image,my_Color color){
	png_byte *row=image->row_pointers[y];
	png_byte *ptr =&(row[x*4]);
	ptr[0]=color.r;
	ptr[1]=color.g;
	ptr[2]=color.b;
	//ptr[3]=0,5;
}
void draw_line(struct My_png *image,my_Color color,int thinkness,struct Point p1,struct Point p2) {
    int x,y;
    if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        // Some error handling: input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA
    }
	if(thinkness<=0)return;
    //if (png_get_color_type(image->png_ptr, image->info_ptr) != PNG_COLOR_TYPE_
	int start_x=p1.x,start_y=p1.y;
	int end_x=p2.x,end_y=p2.y;
	//int thinkness=4;
	
	float delta_x=abs(start_x-end_x);
	float delta_y=abs(start_y-end_y);
	float error=0;
	float delta_err=(delta_y+1);
	float delta_err_x=(delta_x+1);
	int new_y=start_y;
	int diry=end_y-start_y;
	if(diry>0)diry=1;
	if(diry<0)diry=-1;

	int new_x=start_x;
	int dirx=end_x-start_x;
	if(dirx>0)dirx=1;
	if(dirx<0)dirx=-1;
//	printf("%d\n",diry);
	//printf("x=%f y=%f\n",delta_x,delta_y);
	if(delta_x>=delta_y){
		for(int now_x=start_x;now_x<end_x;now_x++){
			
			if(diry!=0){
				for(int k=0;k<thinkness;k++)
					draw_pixel(now_x+k,new_y,image,color);
			}
			if(diry==0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x,new_y+k,image,color);
			//	puts("zero");
			}
			error=error+delta_err;
		
			if(error>=(delta_x+1)){
				new_y=new_y+diry;
				error=error-(delta_x+1);		
			}
		}
		 for(int now_x=start_x;now_x>end_x;now_x--){
                                
                        if(diry!=0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x+k,new_y,image,color);
                        }
			if(diry==0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x,new_y+k,image,color);
			//	puts("zero2");
			}
			error=error+delta_err;
                
                        if(error>=(delta_x+1)){
                                new_y=new_y+diry;
                                error=error-(delta_x+1);
                        }
                }
	}
	
	if(delta_x<delta_y){
	for(int now_y=start_y;now_y<end_y;now_y++){
		
		if(diry!=0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x+k,now_y,image,color);
		}
		if(diry==0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x,now_y+k,image,color);
		//	puts("null");
		}
		error=error+delta_err_x;
		if(error>=(delta_y+1)){
			new_x=new_x+dirx;
			error=error-(delta_y+1);
		}
	}
	 for(int now_y=start_y;now_y>end_y;now_y--){

                if(diry!=0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x+k,now_y,image,color);
		}
		if(diry==0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x,now_y+k,image,color);
                //	puts("null2");
		}
		error=error+delta_err_x;
                if(error>=(delta_y+1)){
                        new_x=new_x+dirx;
                        error=error-(delta_y+1);
                }
        }
	}



}

void draw_triangle(struct My_png *image){
	my_Color color_1={234,22,12},color_2={255,255,25};
	Point point1={200,100},point2={100,200},point3={300,300};
	Point min_point,mid_point,max_point;
	if(point1.y<=point2.y){
		min_point=point1;
		if(point2.y<point3.y){
			mid_point=point2;
			max_point=point3;
		}else{
			mid_point=point3;
			max_point=point2;
		}
	}
	else{	
	if(point2.y<=point3.y){
		min_point=point2;
		 if(point3.y<point1.y){
                        mid_point=point3;
                        max_point=point1;
                }else{
                        mid_point=point1;
                        max_point=point3;
                }
	}else{
	if(point3.y<=point1.y){

	min_point=point3;
	 if(point1.y<point2.y){
                        mid_point=point1;
                        max_point=point2;
                }else{
                        mid_point=point2;
                        max_point=point1;
                }
	
	}}}
	Point right,left;
	if(mid_point.x>=max_point.x){
		right.x=mid_point.x;
		right.y=mid_point.y;
		left.x=max_point.x;
		left.y=max_point.y;
	}else{
		right.x=max_point.x;
		right.y=max_point.y;
		left.x=mid_point.x;
		left.y=mid_point.y;
	}
	
	//printf("min=%d left=%d right=%d ",min_point.y,left.y,right.y);
	int  a1,b1,a2,b2,a3,b3;
	//left line constants a1 b1
	//right line constants a2 b2
	//down line constants a3 b3
	a1=(abs(left.y-min_point.y)/abs(left.x-min_point.x));
	b1=abs(min_point.y-a1*min_point.x);
	a2=(abs(right.y-min_point.y)/abs(right.x-min_point.x));
        b2=abs(min_point.y-a2*min_point.x);
	a3=(abs(left.y-right.y)/abs(left.x-right.x));
        b3=abs(right.y-a3*right.x);
	//printf("min_x=%d min_y=%d");
	draw_line(image,color_1,4,min_point,left);
	draw_line(image,color_1,4,min_point,right);
	draw_line(image,color_1,4,right,left);
	printf("a1=%d b1=%d a2=%d b2=%d a3=%d b3=%d\n",a1,b1,a2,b2,a3,b3);
	for(int row=0;row<image->height;row++){
		for(int x=0;x<image->width;x++){
			if(row<=mid_point.y && row>min_point.y &&
				x>=abs(row-b1)/a1 ){
				draw_pixel(x,row,image,color_2);
			}
					
		}
	}
	
	//printf("min=%d mid=%d max=%d\n",min_point.y,mid_point.y,max_point.y);
}
int main(int argc, char **argv) {
    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
	return 0;
    }

    struct My_png image;
    read_png_file(argv[1], &image);
    //process_file(&image);
    draw_triangle(&image);
	my_Color c={255,0,255};
	Point p1={100,200},p2={300,200};
		
	//draw_line(&image,c,4,p1,p2);
    write_png_file(argv[2], &image);

    return 0;
}
