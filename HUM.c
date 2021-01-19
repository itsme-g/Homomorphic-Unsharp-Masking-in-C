#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
//#include "pgm.h"
#define SIZE 50	
//#define N 3

typedef struct PGM
 {
	char name[100];
	int width;
	int height;
	int max_value;
	int **matrix;
} PGM;

int** create_matrix(int height, int width);

void p2f(PGM* p, char *file_name) ;

int sort(int *a,int filt);

void apply_padding(PGM* p,int n,int global_intensity);

//void apply_median_filter(int** source, int** dest, int height, int width, int k, int filt);


int ** create_matrix(int height, int width)
{
	int **matrix = malloc(height*sizeof(int*)) ;
	
	if (matrix)
	{
		int i;
		for (i = 0; i < height; i++)
		{
			matrix[i] = malloc(width * sizeof(int)) ;
		}
	}
	return matrix;
}


float ** create_matrix1(int height, int width)
{
	
	float **matrix = malloc(height*sizeof(int*)) ;
	
	if (matrix)
	{
		int i;
		for (i = 0; i < height; i++)
		{
			matrix[i] =malloc(width * sizeof (int)) ;
		}
	}
	return matrix;
}



void skip_comments(FILE* fp, char* line, int size)
{
	do{
		fgets(line, size, fp);
		if (line[0] != '#') 
		{
			return ;
		}	
	} while(line!= NULL);
}

PGM* f2p(char *file_name) {

	FILE * fp;
	char c;
	char name[100];
	char line[SIZE];
	int height, width, max_value;

	fp = fopen(file_name, "r");

	if (fp == NULL)
		return NULL;

	PGM *p = malloc(sizeof(PGM));

	
	skip_comments(fp, line, SIZE);
	sscanf(line, "%s", p->name);
	
	skip_comments(fp, line, SIZE);
	sscanf(line, "%d %d", &width, &height);
	
	skip_comments(fp, line, SIZE);
	sscanf(line, "%d", &max_value);

	int **matrix = malloc(height*sizeof(int*)) ;
	if (matrix){
		int i,j;
				for (i = 0; i < height; i++) {
		
			matrix[i] = malloc(width * sizeof (int)) ;
				for (j = 0; j < width; j++) {
	
				fread(&matrix[i][j],sizeof(unsigned char),1,fp);  
			}
		}
		
		p->matrix = matrix;
		p->height = height;
		p->width = width;
		p->max_value = max_value;
		return p;
	}

	fclose(fp);
	return NULL;
}

void p2f(PGM* p, char *file_name) {
	FILE* fp = fopen(file_name, "w");
	fprintf(fp, "%s\n", p->name);
	fprintf(fp, "%d %d\n%d\n", p->width, p->height, p->max_value);
	int i,j;
	for (i = 0; i < p->height; i++) 
	{
		for (j = 0; j < p->width; j++)
		{
			fwrite(&p->matrix[i][j],sizeof(unsigned char),1,fp);
		}
	}
	fclose(fp);
}

float find_max(float **scale, int h, int w)
{
 int i, j;
 float largest=scale[0][0];
 
 
 for(i = 0; i < h; i++)
 	for(j = 0;j < w; j++)
        	{
     			if(scale[i][j]> largest)
     			largest = scale[i][j];
   		}
    
 return largest;
 
}

void apply_hum(int **source, int** dest,float** scale, int height, int width,int k,int filt,int global_intensity) 
{
	int i,j,l,m;
	float largest;
	for (i = k; i < height-k; i++)
	{
        	for (j = k; j < width-k ; j++)
		{
                	int sum1=0;
			int sum2=0;
                	for(m=0;m<filt;m++)
			{
                      
				for(l=0;l<filt;l++)
				{
					sum1=sum1+(source[i-k+m][j-k+l]);
                          		sum2=sum2+(pow(source[i-k+m][j-k+l],2));
					        
                      		}
                    	}
			if(sum1!=0)
			scale[i-k][j-k]=(float)(sum2/sum1);
			//printf("%f\n",scale[i-k][j-k]);
               	}
                  
                                  
        }
       
	
	/*for (i = n; i < height-n; i++)
	{
        	for (j = n; j < width-n ; j++)
        		{
                        	int sum=0;
                        	for(k = 0; k < filt; k++)
                                	{
                                       		for(l = 0; l < filt; l++)
                      				sum = sum + source[i-n+k][j-n+l];
                   			}
                  
                  		scale[i-n][j-n]=(sum/filt*filt);
                         }
                         
         }*/

      for (i = 0; i < height-2*k; i++)
         	{
			for (j = 0; j < width-2*k; j++)
				{
                      
                                   scale[i][j]= (global_intensity * source[i+k][j+k])/scale[i][j];
				}
		}
		 /*int new_height = height - 2*k;
	int new_width = width - 2*k;
	
        FILE* fp = fopen("/home/bibl04/Desktop/out/name111.pgm", "w");
	fprintf(fp, "P5\n");
	fprintf(fp, "%d %d\n255\n", new_width, new_height);
	
	for (i = 0; i < new_height; i++) 
	{
		for (j = 0; j < new_width; j++)
		{
			fwrite(&scale[i][j],sizeof(unsigned char),1,fp);
		}
	}
	fclose(fp);*/
         
	//largest =find_max(scale,height-2*k,width-2*k);
         
        /*for (i = 0; i < height-2*k; i++)
         	{
			for (j = 0; j < width-2*k; j++)
				{
                      
                                   scale[i][j]=((scale[i][j]*255)/largest);
               		 	}
         	}*/
    
    	 for (i = 0; i < height-2*k; i++)
         	{
			for (j = 0; j < width-2*k; j++)
				{
                      
                                   dest[i][j]=scale[i][j];
               		 	}
         	}
}




void apply_padding(PGM* p,int n, int global_intensity)
{
	int k;
	k=n/2;
	int new_height = p->height + 2*k;
	int new_width = p->width + 2*k;
	
	
	int **matrix = create_matrix(new_height, new_width);
	float **scale =  create_matrix1(p->height, p->width);
	
	if (matrix){
		int i,j;
		
		
		for (i = 0; i < new_height; i++)
		{
			for (j = 0; j < new_width; j++)
			{
		
		        	if((i<k) || (j<k) || i>(p->height) || j>(p->width))
		
		        		matrix[i][j] = 0;
		        }
		}
		
		
		for (i = k; i < new_height-k; i++)
		{
			for (j = k; j < new_width-k; j++) 
			{
				matrix[i][j] = p->matrix[i-k][j-k];	
			}
		}
		
		for (i = 0; i < p->height; i++)
		{
			for (j = 0; j < p->width; j++)
			{
				scale[i][j] = p->matrix[i][j];	
			}
	        }
	        
	       
		
		apply_hum(matrix,p->matrix, scale, p->height+2*k, p->width+2*k,k,n,global_intensity);

	       //apply_mean_filter(matrix,p->matrix, scale, p->height+2*k, p->width+2*k,k,n);
		
	}
	
}


int calc(PGM *p)
{

 int x,sum=0,i,j;
 for (i = 0; i < p->height; i++)
        for (j = 0; j < p->width; j++) {
                sum = sum+(p->matrix[i][j]);	
                }        	        	
        		
  printf("sum is %d",sum);
  x=(int)(sum/((p->height)*(p->width)));
  printf("Global Mean Intensity is %d",x);

return x;
}

int main(int argc, char *argv[]) 
{
	
	DIR *dp;
	struct dirent *ep;
	char *in_dir =  "/home/bibl04/Desktop/in/";
	char *out_dir = "/home/bibl04/Desktop/out/";
	dp = opendir (in_dir);
	
	if (dp != NULL)
	{
		while (ep = readdir (dp))
			{		
				char*file_name = ep->d_name;
				//ignoring hidden file
				
				if ('.' == file_name[0]) 
					{
 			   			continue;
					}
			
				char in_path[80];
				int x;
				strcpy (in_path, in_dir);
				strcat (in_path, file_name);
				puts (in_path);
				
				PGM *p = f2p(in_path);
				
				int global_intensity=calc(p);
				
				printf("global intensity is %d",global_intensity);      
				
				if(p->height < p->width)
					{
					x=( (2 *	p->height)/3);					
				        //apply_padding(p, x, global_intensity);
				        }
				else
				        {
				        x= ((2 *	p->width)/3);
					apply_padding(p, x, global_intensity);
					}
	         		//display(p);
				
				char out_path[80];
				strcpy (out_path, out_dir);
				strcat (out_path, file_name);
				puts(out_path);
        			p2f(p, out_path);
		      }
		      
		closedir (dp);
	}
	else
	{
		perror ("Couldn't open the directory");
	}
		
	return 0;
}


