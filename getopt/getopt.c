
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define PNG_DEBUG 3
#include </usr/local/include/png.h>
#include <string.h>
#include <getopt.h>
#include<regex.h>


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
        float x;
        float y;
}typedef Point;

struct my_Color{
	int r;
	int g;
	int b;
	int alpha;
}typedef my_Color;

void read_png_file(char *file_name, struct My_png *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened

	puts("file could not be open r");
	fclose(fp);
	return;
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep)header, 0, 8)){
       	puts("type_error:file is not recognoized as a PNG");
	return;
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
	ptr[3]=color.alpha;
}
void draw_line(struct My_png *image,my_Color color,int thinkness,struct Point p1,struct Point p2) {
    
    if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        puts("input file's color type RGB but must be RGBA");
	return;
    }
	if(thinkness<=0)return;
	int start_x=p1.x,start_y=p1.y;
	int end_x=p2.x,end_y=p2.y;
	
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
	if(delta_x>=delta_y){
		for(int now_x=start_x;now_x<=end_x;now_x++){
			
			if(diry!=0){
				for(int k=0;k<thinkness;k++)
					draw_pixel(now_x+k,new_y,image,color);
			}
			if(diry==0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x,new_y+k,image,color);
			}
			error=error+delta_err;
		
			if(error>=(delta_x+1)){
				new_y=new_y+diry;
				error=error-(delta_x+1);		
			}
		}
		 for(int now_x=start_x;now_x>=end_x;now_x--){
                                
                        if(diry!=0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x+k,new_y,image,color);
                        }
			if(diry==0){
				for(int k=0;k<thinkness;k++)draw_pixel(now_x,new_y+k,image,color);
			}
			error=error+delta_err;
                
                        if(error>=(delta_x+1)){
                                new_y=new_y+diry;
                                error=error-(delta_x+1);
                        }
                }
	}
	
	if(delta_x<delta_y){
	for(int now_y=start_y;now_y<=end_y;now_y++){
		
		if(diry!=0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x+k,now_y,image,color);
		}
		if(diry==0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x,now_y+k,image,color);
		}
		error=error+delta_err_x;
		if(error>=(delta_y+1)){
			new_x=new_x+dirx;
			error=error-(delta_y+1);
		}
	}
	 for(int now_y=start_y;now_y>=end_y;now_y--){

                if(diry!=0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x+k,now_y,image,color);
		}
		if(diry==0){
			for(int k=0;k<thinkness;k++)draw_pixel(new_x,now_y+k,image,color);
		}
		error=error+delta_err_x;
                if(error>=(delta_y+1)){
                        new_x=new_x+dirx;
                        error=error-(delta_y+1);
                }
        }
	}



}

void draw_triangle(struct My_png *image,my_Color color_1,my_Color color_2,Point point1,Point point2,Point point3,int thinkness,int full){
	
	if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
     		puts("input file's color type RGB but must be RGBA");
        	return;
    	}
	
	Point min_point,mid_point,max_point;
	if(point1.y<=point2.y&&point1.y<=point3.y){
		min_point=point1;
		if(point2.y>point3.y){
			max_point=point2;
			mid_point=point3;
		}else{
			max_point=point3;
                        mid_point=point2;
		}
	}
	if(point2.y<=point1.y && point2.y<=point3.y){
		min_point=point2;
		if(point1.y>point3.y){
			max_point=point1;
			mid_point=point3;
		}
		else{
			max_point=point3;
			mid_point=point1;
		}
	}
	if(point3.y<=point2.y && point3.y<=point1.y){
		min_point=point3;
		if(point2.y>point1.y){
			max_point=point2;
			mid_point=point1;
		}else{
			max_point=point1;
			mid_point=point2;
		}
	}/*	
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
	
	}}}*/
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
	

	float  a1,b1,a2,b2,a3,b3;
	//left line constants a1 b1
	//right line constants a2 b2
	//down line constants a3 b3
	//printf("%f\n",left.x-right.x);
	if(full!=0){
	a1=(left.y-min_point.y)/(left.x-min_point.x);
        b1=min_point.y-a1*min_point.x;
        a2=(right.y-min_point.y)/(right.x-min_point.x);
        b2=(min_point.y-a2*min_point.x);
        a3=(left.y-right.y)/(left.x-right.x);
        b3=(right.y-a3*right.x);
	int check_py=0,check_3=0;
	if(fabs(left.x-right.x)<0.0001){
		check_3=1;
		a3=1;
                b3=left.x;
		//puts("3");
	}
	 if(fabs(min_point.x-right.x)<0.0001){	
		//puts("2");	
		check_py++;
		a2=1;
        	b2=right.x;
		//puts("ko");
	}
	if(fabs(min_point.x-left.x)<0.0001){
		check_py++;
                a1=1;
                b1=left.x;
		//puts("ok");
        }

//	printf("a1=%f b1=%f a2=%f b2=%f a3=%f b3=%f",a1,b1,a2,b2,a3,b3);
//	printf("\nmin=(%f, %f) left=(%f, %f) right=(%f, %f)\n",min_point.x,min_point.y,left.x,left.y,right.x,right.y);	
	for(int row=0;row<image->height;row++){
		for(int x=0;x<image->width;x++){
			if(row<=mid_point.y && row>=min_point.y){
				if(check_py==0 && left.x<(left.y-b2)/a2 && x>=(row-b1)/a1 && x<=(row-b2)/a2){
					draw_pixel(x,row,image,color_2);
				}
				if(check_py==0 && left.x>(left.y-b2)/a2 && x<=(row-b1)/a1 && x>=(row-b2)/a2){
					draw_pixel(x,row,image,color_2);
				}
				if(check_py!=0 && min_point.x<right.x && x<=(row-b2)/a2 && x>=b1){
					draw_pixel(x,row,image,color_2);
					continue;
				}
				if(check_py!=0 && min_point.x>left.x && x<=b2 && x>=(row-b1)/a1){
					draw_pixel(x,row,image,color_2);
                                        continue;
				}
				/*if(check_3!=0 && x>=b1 && x<=(row-b2)/a2 && min_point.x<right.x){

					draw_pixel(x,row,image,color_2);
                                        continue;
				}
				if(check_3!=0 && x<=b2 && x>=(row-b1)/a1 && min_point.x>left.x){
					draw_pixel(x,row,image,color_2);
                                        continue;
				}*/

			}
			
			if(row<=max_point.y && row>=mid_point.y){
				
				if(check_3==0&&check_py==0&&left.x<(left.y-b2)/a2&&x>=(row-b3)/a3 && x<=(row-b2)/a2 && left.y<=right.y){
					draw_pixel(x,row,image,color_2);
					continue;
				
				}
				if(check_3==0 &&check_py==0 && left.x<(left.y-b2)/a2&& x<=(row-b3)/a3 && x>=(row-b1)/a1 && left.y>=right.y){
					draw_pixel(x,row,image,color_2);
					continue;
				}
				if(check_3==0&&check_py==0&&left.x>(left.y-b2)/a2 && x<=(row-b3)/a3 && x>=(row-b2)/a2 && left.y<=right.y){
					draw_pixel(x,row,image,color_2);
					continue;
				}
				if(check_3==0&&check_py==0&&left.x>(left.y-b2)/a2 && x<=(row-b1)/a1 && x>=(row-b3)/a3 && left.y>=right.y){
                                        draw_pixel(x,row,image,color_2);
                                        continue;
                                }
				if(check_3!=0&& check_py==0 &&x<=(row-b1)/a1 && x>=b3&& right.y<=left.y&& min_point.x>right.x ){
					draw_pixel(x,row,image,color_2);
					continue;
				}
				if(check_3!=0  &&check_py==0&&x>=(row-b1)/a1 && x<=b3 && right.y<=left.y && min_point.x<right.x){
					draw_pixel(x,row,image,color_2);
                                        continue;
				}
				if(check_py!=0 && min_point.x>left.x) {
					//puts("here");
					if( x<=(row-b3)/a3 && x>=(row-b1)/a1&& left.y>=right.y){
						draw_pixel(x,row,image,color_2);
                                        	continue;
					}
					if(x<=b2 && x>=(row-b3)/a3 && left.y<right.y){
						draw_pixel(x,row,image,color_2);
                                                continue;
					}
				}
				if(check_py!=0 && min_point.x<right.x){
					 if( x<=(row-b3)/a3 && x>=b1&& left.y>right.y){
						draw_pixel(x,row,image,color_2);
                                        	continue;
					}
					if(left.y<=right.y && x>=(row-b3)/a3 && x<=(row-b2)/a2){
						draw_pixel(x,row,image,color_2);
                                                continue;
					}
				}
			}
		
		}
		
	}}
	int old_x1=min_point.x,old_x2=left.x,old_y2=left.y,old_y1=min_point.y,old_x3=right.x,old_y3=right.y;
	for(int q=0;q<thinkness;q++){
		if(min_point.x-left.x<0){
			
		//	draw_line(image,color_1,1,min_point,left);
		}
		//draw_line(image,color_1,1+q,min_point,left);
		//draw_line(image,color_1,1+q,min_point,right);
        	//draw_line(image,color_1,1+q,right,left);
		//if(min_point.x>right.x && min_point.x>left.x){
			//min_point.y--;
			//min_point.x--;
			//left.y--;
			//left.x--;
			

		//}
	}
	//min_point.x=old_x1;
	//min_point.y=old_y1;
	//left.x=old_x2;
	//left.y=old_y2;
	draw_line(image,color_1,thinkness,min_point,left);
	draw_line(image,color_1,thinkness,min_point,right);
        draw_line(image,color_1,thinkness,right,left);
}

void draw_collage(struct My_png *image,struct My_png *img){
	if (png_get_color_type(image->png_ptr, image->info_ptr) == PNG_COLOR_TYPE_RGB){
        	puts("input file's color type RGB but must be RGBA");
        	return;
    	}
	int repeat_X=5;
	int repeat_Y=4;
	int const_x=image->width/repeat_X;//ширина "кусочка"
	int const_y=image->height/repeat_Y;// длина "кусочка"	
	my_Color now_color[image->height][image->width];
	int kol_x=0,kol_y=0;
	//printf("%d %d %d %d \n",img->height,img->width,const_y,const_x);
	if(const_y<img->height && const_x<img->width ){
		int koef_x=img->width*repeat_X/image->width;	//коэффициент сжатия по ширине 
		int koef_y=img->height*repeat_Y/image->height;	//коэффициент сжатия по длине
		
		for(int row=0;row<img->height;row++){
			kol_x=0;
			if(row%koef_y==0)kol_y++;
              		for(int x=0;x<img->width;x++){
				if(x%koef_x==0){
				        png_byte *n_row=img->row_pointers[row];
                         		png_byte *ptr =&(n_row[x*4]);
                         		now_color[kol_y][kol_x].r=ptr[0];
                         		now_color[kol_y][kol_x].g=ptr[1];
                         		now_color[kol_y][kol_x].b=ptr[2];
                         		now_color[kol_y][kol_x].alpha=ptr[3];
			 		kol_x++;
				}	
		
			}	
		}	
		//printf("%d %d %d %d \n",img->height,img->width,const_y,const_x);
	
	}
	if(const_y>=img->height && const_x>=img->width){
		int koef_x=image->width/(repeat_X*img->width);
		int koef_y=image->height/(repeat_Y*img->height);
		for(int row=0;row<img->height;row++){
			kol_x=0;
			for(int x=0;x<img->height;x++){
				png_byte *n_row=img->row_pointers[row];
                         	png_byte *ptr =&(n_row[x*4]);
				int old_y=kol_y;
				for(int j=0;j<koef_y;j++){
					int old_x=kol_x;
					for(int i=0;i<koef_x;i++){
                                        	now_color[kol_y][kol_x].r=ptr[0];
                                        	now_color[kol_y][kol_x].g=ptr[1];
                                        	now_color[kol_y][kol_x].b=ptr[2];
                                        	now_color[kol_y][kol_x].alpha=ptr[3];
						kol_x++;
					}
					kol_x=old_x;
					kol_y++;
				
				
				}
				kol_y=old_y;	
				kol_x+=koef_x;
			
			}
			kol_y+=koef_y;
		}

	}
	if(const_y>img->height && const_x<img->height){
		int koef_x=img->width*repeat_X/image->width;    //коэффициент сжатия по ширине
                int koef_y=image->height/(repeat_Y*img->height);//коэффициент растяжения по длине
		
		  for(int row=0;row<img->height;row++){
                        kol_x=0;
                        //if(row%koef_y==0)kol_y++;
                        for(int x=0;x<img->width;x++){
                                if(x%koef_x==0){
                                        png_byte *n_row=img->row_pointers[row];
                                        png_byte *ptr =&(n_row[x*4]);
					int old_y=kol_y;
					for(int i=0;i<koef_y;i++){
                                        	now_color[kol_y][kol_x].r=ptr[0];
                                        	now_color[kol_y][kol_x].g=ptr[1];
                                        	now_color[kol_y][kol_x].b=ptr[2];
                                        	now_color[kol_y][kol_x].alpha=ptr[3];
						kol_y++;
					}
					kol_y=old_y;
                                        kol_x++;
                                }

                        }
			kol_y+=koef_y;
                }
	}
	if(const_y<img->height && const_x>img->height){
		int koef_x=image->width/(repeat_X*img->width);
		int koef_y=img->height*repeat_Y/image->height;
		for(int row=0;row<img->height;row++){
                        kol_x=0;
			if(row%koef_y==0)kol_y++;
                        for(int x=0;x<img->height;x++){
                                png_byte *n_row=img->row_pointers[row];
                                png_byte *ptr =&(n_row[x*4]);
                                                              
                                int old_x=kol_x;
                                for(int i=0;i<koef_x;i++){
                                        now_color[kol_y][kol_x].r=ptr[0];
                                	now_color[kol_y][kol_x].g=ptr[1];
                                        now_color[kol_y][kol_x].b=ptr[2];
                                        now_color[kol_y][kol_x].alpha=ptr[3];
                                        kol_x++;
                                }
                                kol_x=old_x;
                                kol_x+=koef_x;
                        
                        }
                       
                }

	}

	
	for(int row=0;row<const_y;row++){
		for(int x=0;x<const_x;x++){
			
			for(int j=0;j<repeat_Y;j++){
				for(int i=0;i<repeat_X;i++){
					png_byte *n_row=image->row_pointers[row+j*const_y];
					png_byte *ptr =(&(n_row[(x+i*const_x)*4]));
					ptr[0]=now_color[row][x].r;
        				ptr[1]=now_color[row][x].g;
        				ptr[2]=now_color[row][x].b;
					ptr[3]=now_color[row][x].alpha;
				}		
			}
	
		}
	//puts("");
	}
}
struct Rectangle{
	Point start;
	Point end;
	int width;
	int height;
}typedef Rectangle; 

int color_check(int x,int y,my_Color color,struct My_png *image ){
	png_byte *row =image->row_pointers[y];
	png_byte *ptr =&(row[x*4]);
	if(ptr[0]==color.r && ptr[1]==color.g && ptr[2]==color.b)return 1;
	else return 0;
}
void find_rectangle(my_Color find_color,my_Color full_color,struct My_png *image){
 	struct Rectangle list_rec[image->height];
	int kol_rec=0;
	for (int y = 1; y < image->height-1; y++) {
           
                //png_byte *row = image->row_pointers[y];
                for (int x = 1; x < image->width-1; x++) {
                       // png_byte *ptr = &(row[x * 4]);
                        if(color_check(x,y,find_color,image)){
				if(color_check(x-1,y,find_color,image)==0 && color_check(x,y-1,find_color,image)==0){
					int now_x=x, now_y=y;
					int width=0,height=0;
					while(color_check(x,now_y,find_color,image)){
						now_y++;
						height++;
					}
					while(color_check(now_x,y,find_color,image)){
						now_x++;
						width++;		
					}
					
					int check_full=1;
					for(int i=y;i<y+height-1;i++){
						for(int j=x;j<x+width-1;j++){
							if(color_check(j,i,find_color,image)){
								check_full=1;
							}else{
								check_full=0;
								break;
							}
						}
						if(check_full==0)break;
					}
					int check_shape=1;
					int check_up=1,check_down=1,check_right=1,check_left=1;
					if(check_full){
						//Point p1={x,y};
                                        	//Point p2={x+width-1,y+height-1};
                                        	//list_rec[kol_rec].start=p1;
                                        	//list_rec[kol_rec].end=p2;
                                        	//kol_rec++;
						for(int i=y;i<y+height-1;i++){
                                                    	if(color_check(x-1,i,find_color,image)==0)check_left=1;
							else{
								check_left=0;
								break;
							}    
                                        	}
						for(int i=y;i<y+height-1;i++){
							if(color_check(x+width,i,find_color,image)==0)check_right=1;
							else{
								check_right=0;
								break;
							}
						}
						for(int j=x;j<x+width-1;j++){
							if(color_check(j,y-1,find_color,image)==0)check_up=1;
							else{
								check_up=0;
								break;
							}	
						}
						for(int j=x;j<x+width-1;j++){
							if(color_check(j,y+height,find_color,image)==0)check_down=1;
							else{
								check_down=0;
								break;
							}
						}	
						if(check_left && check_right && check_up&& check_down){
							Point p1={x,y};
                                                	Point p2={x+width-1,y+height-1};
                                                	list_rec[kol_rec].start=p1;
                                                	list_rec[kol_rec].end=p2;
							list_rec[kol_rec].width=width;
							list_rec[kol_rec].height=height;
                                                	kol_rec++;
						}
					}
					//if(width==1 || height==1){
							//int check=0;
							//for()
							//Point p1={x,y};
                                                        //Point p2={x+width-1,y+height-1};
                                                        //list_rec[kol_rec].start=p1;
                                                        //list_rec[kol_rec].end=p2;
                                                        //list_rec[kol_rec].width=width;
                                                        //list_rec[kol_rec].height=height;
                                                        //kol_rec++;

					//}
				}
			}
		}
	}
	Rectangle max=list_rec[0];
	for(int q=1;q<kol_rec;q++){
		if(max.width*max.height<list_rec[q].width*list_rec[q].height){
			max.height=list_rec[q].height;
			max.width=list_rec[q].width;
			max.start.x=list_rec[q].start.x;
			max.end.x=list_rec[q].end.x;
			max.start.y=list_rec[q].start.y;
			max.end.y=list_rec[q].end.y;
		}
		//draw_line(image,full_color,1,list_rec[q].start,list_rec[q].end);
		//printf("(%f, %f): (%f, %f)\n",list_rec[q].start.x,list_rec[q].start.y,list_rec[q].end.x,list_rec[q].end.y);
	}
	for(int i=max.start.y;i<=max.end.y;i++){
		for(int j=max.start.x;j<=max.end.x;j++){
			draw_pixel(j,i,image,full_color);
		}
	}

}/*
int main(int argc, char **argv) {
    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
	return 0;
    }

    struct My_png image;
	struct My_png img;
    read_png_file(argv[1], &image);
	char need[8];
	strcpy(need,"eng.png");	
	
    read_png_file(need,&img);	
   
    	//draw_collage(&image,&img);
	my_Color c={251,2,7,255};
	my_Color c2={0,255,0,255};
	my_Color cc={0,0,155,255};
	Point p1={500,10},p2={434,245},p3={489,350};
	//Point p1={100,100},p2={150,300},p3={300,200};
	draw_triangle(&image,c,c2,p3,p2,p1,4,1);		
//	draw_line(&image,cc,2,p1,p2);
	//find_rectangle(c,c2,&image);
    write_png_file(argv[2], &image);

    return 0;
}
*/
int isPngValid(char* name){
	char *regexp="\\.png$";
	regex_t regexComp;

	if(regcomp(&regexComp,regexp,REG_EXTENDED)){
		return 0;
	}

	return regexec(&regexComp, name, 0, NULL, 0) == 0;
}
int isOValid(char* name){
	
	char *regexp="\\.o?";
        regex_t regexComp;

        if(regcomp(&regexComp,regexp,REG_EXTENDED)){
	
                return 0;
        }
	//printf("%d",regexec(&regexComp, name, 0, NULL, 0) == 0);
        return regexec(&regexComp, name, 0, NULL, 0) == 0;
} 
int isColorValid(char* color){
	if(strcmp(color,"yellow")==0 ||strcmp(color,"red")==0 || strcmp(color,"green")==0 || 
		strcmp(color,"blue")==0 || strcmp(color,"white")==0 ||strcmp(color,"black")==0 ||strcmp(color,"none")==0){
		return 1;
	}
	else{
		return 0;
	}
}

// -triangle -(x,y) -(x2,y2) -(x3,y3) -full_color -thinkness
// -rectangle -color_find -color_full
// -collage -N -M
// -line -(x,y) -(x2,y2) -thinkness -color
void printHelp(){
    //для рисования линии введите --line и необходимые аргументы: координаты начала и конца --point1 <x,y> --point2 <x,y> толщина линии --thinkness n
    // цвет линии --color_line color (*)
    // для рисования треугольника введите --triangle и необходимые аргументы: координаты точек треугольника --point1 <x,y> --point2 <x,y> --point3 <x,y>
    // цвет заливки --color_full <color> (если треугольник без заливки введите none) --thinkness n цвет линии --color_line color (*)
    // для создания коллажа введите --collage и необходимые аргументы: изображение для коллажа -img name.png; количества по оси X --x_axis и по оси Y --y_axis
    // для находения самого большого треугольника введите  --rectangle и необходимые аргументы: его цвет --color_find <color>; цвет для перекраски --color_full <color>

    printf("для рисования линии введите --line и необходимые аргументы: \nкоординаты начала и конца --point1 <x,y> --point2 <x,y> \nтолщина линии --thinkness <n> \nцвет линии --color_line <color> (*)");
    puts("");
    printf("\nдля рисования треугольника введите --triangle и необходимые аргументы: \nкоординаты точек треугольника --point1 <x,y> --point2 <x,y> --point3 <x,y>"
           "\nцвет заливки --color_full <color> (если треугольник без заливки введите none) \nтолщина линии --thinkness <n> \nцвет линии --color_line color (*)");
    puts("");
    printf("\nдля создания коллажа введите --collage и необходимые аргументы: \nизображение для коллажа --img <name.png> \nколичества по оси X --x_axis и по оси Y --y_axis");
    puts("");
    printf("\nдля находения самого большого прямоугольника введите  --rectangle и необходимые аргументы: \nего цвет --color_find <color> \nцвет для перекраски --color_full <color>\n");
    puts("");
    printf("* цвета: blue, red, green, yellow, white, black\n");
    //    printf("getopt example\n");
//    printf("-f <value> - final value\n");
//    printf("-r - reverse\n");
//    printf("-v --verbose - verbose\n");
//    printf("-h -? - help\n");
}
struct Configs{

    char color_line[100];
    char color_find[100];
    char color_full[100];
    //int full_color;
    Point point1;
    Point point2;
    Point point3;
    int thinkness;
    int func;
    int axis_x;
    int axis_y;
    char img[100];
};

int main(int argc, char* argv[]){
    if(argc==1){
        printHelp();
        return 0;
    }
    if(!isOValid(argv[0])){
	puts("не введен исполняемы файл");	
	return 0;
    }
    struct Configs config = {0, 0, 0};
    //char *opts = "f:lutpcrvha";
    char *opts = "ltrcqabpsufxyhi";
    struct option longOpts[]={
            {"line", no_argument, NULL, 'l'},
            {"triangle",no_argument,NULL,'t'},
            {"rectangle",no_argument,NULL,'r'},
            {"collage",no_argument,NULL,'c'},

            {"color_line",1,NULL,'q'},
            {"point1",1,NULL,'a'},
            {"point2",1,NULL,'b'},
            {"point3",1,NULL,'p'},
            {"thinkness",1,NULL,'s'},
            {"color_full",1,NULL,'u'},

            {"color_find",1,NULL,'f'},
            {"x_axis",1,NULL,'x'},
            {"y_axis",1,NULL,'y'},
            {"img",1,NULL,'i'},
            {"help",no_argument,NULL,'h'},

	    {"info",no_argument,NULL,'o'},

            { NULL, 0, NULL, 0}
    };
    int opt;
    int longIndex;
    opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    char point[100];
    char* istr;
    struct Point p;
    while(opt!=-1){
        switch(opt){
            case 'l':
                config.func = 4;
                break;
            case 't':
                config.func=1;
                break;
            case 'r':
                config.func=2;
                break;
            case 'c':
                config.func=3;
                break;
            case 'a':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
		if(istr!=NULL && atoi(istr)>0)
                	p.x=atoi(istr);
		else{
			puts("point1 имеет не отрицательные аргументы");
			return 0;
		}
                istr=strtok(NULL,",\n ");
		if(istr!=NULL && atoi(istr)>0)
                	p.y= atoi(istr);
		else{
			puts("point1 имеет не отрицательные аргументы");
			return 0;
		}
		
                config.point1=p;
                
                break;
            case 'b':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
		if(istr!=NULL && atoi(istr)>0)
                	p.x=atoi(istr);
                else{
			puts("point2 имеет не отрицательные аргументы");
                        return 0;
		}
		istr=strtok(NULL,",\n ");
		
		if(istr!=NULL && atoi(istr)>0)
                	p.y= atoi(istr);
		else{
			puts("point2 имеет не отрицательные аргументы");
                        return 0;
		}
                config.point2=p ;
                break;
            case 'p':
                strcpy(point,optarg);
                istr= strtok(point,",\n ");
		if(istr!=NULL && atoi(istr)>0)
                	p.x=atoi(istr);
		else{
			puts("point3 имеет не отрицательные аргументы");
                        return 0;
		}
                istr=strtok(NULL,",\n ");
		if(istr!=NULL && atoi(istr)>0)
                	p.y= atoi(istr);
		else{
			puts("point3 имеет не отрицательные аргументы");
                        return 0;
		}
                config.point3=p ;
                break;

            case 's':
		if(!atoi(optarg) || atoi(optarg)<=0){
			puts("thinkness не отрицательное число");
			return 0;
		}
                config.thinkness= atoi(optarg);
                break;
            case 'q':
		if(isColorValid(optarg))
                	strcpy(config.color_line,optarg);
		else{
			puts("не правильный формат цвета (color_line)");
			return 0;
		}
                break;
            case 'u':
		if(isColorValid(optarg))
                	strcpy(config.color_full,optarg);
		else{
			puts("не правильный формат цвета (color_full)");
			return 0;	
		}
                break;
            case 'f':
		if(isColorValid(optarg))
                	strcpy(config.color_find,optarg);
		else{
			puts("не правильный формат цвета (color_find)");
			return 0;
		}
                break;
            case 'x':
		if(!atoi(optarg) || atoi(optarg)<=0){
			puts("аргумент --x_axis это не отрицательное число");

			return 0;
		}
                config.axis_x= atoi(optarg);
                break;
            case 'y':
		if(!atoi(optarg)  || atoi(optarg)<=0){
                        puts("аргумент --y_axis это не отрицательное число");

                        return 0;
                }
                config.axis_y= atoi(optarg);
                break;
            case 'i':
		if(isPngValid(optarg))
                strcpy(config.img,optarg);
		else{
			puts("ошибка изображения");
			return 0;
		}
                break;
	    case 'o':
		break;
            case 'h':

                printHelp();
                return 0;
            case 0:
		printHelp();
                //printf("->%s\n",longOpts[longIndex].name);
        }
        opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    }
    //struct My_png image;
    //read_png_file(argv[1], &image);

   
    if(strcmp(argv[1],"--line")==0 || strcmp(argv[1],"--rectangle")==0 || strcmp(argv[1],"--collage")==0 ||strcmp(argv[1],"--triangle")==0||
            strcmp(argv[1],"-l")==0 || strcmp(argv[1],"-r")==0 || strcmp(argv[1],"-c")==0 ||strcmp(argv[1],"-t")==0){
            argc -= optind;
            argv += optind;
	    char name_png[30];
		
            int kol=optind;
		//printf("%d\n",optind);
	   // for(int i=0; i<argc; i++)
	//	printf(">>%s\n", argv[i]);
		struct My_png image;
            if(argc==1){
		if(isPngValid(argv[0])){
			strcpy(name_png,argv[0]);
			//puts(name_png);
    			//read_png_file(name_png, &image);
		}else{
			printf("Не верный тип изображения");
			return 0;
		}
            }else{
                printf("Введите одно изображение\n");
		return 0;
            }
           if(kol==10&& config.func==4) {
               //line
		read_png_file(name_png,&image);
		my_Color ccc={255,0,0,0};
                draw_line(&image,ccc,config.thinkness,config.point1,config.point2);
                write_png_file(name_png,&image);
		
               //printf("p=%f,%f p=%f,%f t=%d c=%s\n",config.point1.x,config.point1.y,config.point2.x,config.point2.y,config.thinkness,config.color_line);
           } else
           if(kol==14 && config.func==1) {
               //trianngle
               //printf("p=%d,%d p=%d,%d p=%d,%d t=%d c=%s c_f=%s\n",config.point1.x,config.point1.y,config.point2.x,config.point2.y,config.point3.x,config.point3.y,config.thinkness,config.color_line,config.color_full);
		
           }else
           if(kol==8 &&config.func==3){
		puts("okokokkko");
               //printf("img=%s x=%d y=%d\n",config.img,config.axis_x,config.axis_y);
           }else
           if(kol==6 && config.func==2){
               //printf("c=%s c_find=%s\n",config.color_full,config.color_find);
           }

           else{
               printf("НЕ КОРРЕКТНЫЙ ВВОД ДАННЫХ\n\n");
               printHelp();
           }


    }else{
	
        printHelp();
        return 0;
    }


    return 0;
}
