#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#define H 750			// 높이
#define W 750			// 너비
#define maxlevel 255
#define ROUND(f) (int)(f+0.5)
#define RAD(d) (double)(d*3.1416/180)

unsigned char Iarray[H][W][3];//for an interleaved color array
unsigned char garray[H][W][3];//for an interleaved color array

double* fractalx;
double* fractaly;

void initline();			// 배열 흰색으로 초기화
void drawline( FILE *fp );	// 배열 그리기
void lineDDA(int y1, int x1, int y2, int x2, int n, int l, double);		// 이진트리 LinDDA (점과 점을 연결)
void ranlineDDA(int y1, int x1, int y2, int x2, int n, int l, double d);	// 랜덤트리 LineDDA
void groundlineDDA(int y1, int x1, int y2, int x2);							// 잔디 LineDDA

void frac(int x, int y, int l1, int n, int d1);							// 다음 좌표 지정

int main(void){

	FILE* fp;

	int i , j;
	int n;
	int select;
	int l;
	int height = 140;
	double d;

	d = 90;			// 초기 각 90도로 지정
	l = 100;		// 초기 길이 100으로 지정

	double x[2]={50,50};
	double y[2]={200,300};

	fp=fopen("tree.ppm","wb"); //fp=fopen("test.ppm","w");
	fprintf(fp,"P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp,"%d %d\n",W,H); //file size 
	fprintf(fp,"%d\n",maxlevel); //max level 

	printf("1.이진트리   2. 랜덤트리\n생성할 나무 선택 :");
	scanf("%d", &select);
	
	while(1)
	{
		printf("차수를 입력 (7 ~ 12) : ");
		scanf("%d", &n);

		if(n > 6 && n < 13)
			break;
		else
			printf("지정된 범위의 수를 입력하시오!\n");
	}

	initline();		// 백색 초기화

	fractalx = (double*)malloc(n*sizeof(double));
	fractaly = (double*)malloc(n*sizeof(double));

	// 잔디 그리기
	for(i = 0; i < 5; i++)
	{
		groundlineDDA(0, height, 750, height);
		height+=30;
	}

	// 초기 좌표 지정
	fractalx[n - 1] = x[1] + (n * 25);
	fractaly[n - 1] = y[1];
	fractalx[n - 2] = x[0] + (n * 25);
	fractaly[n - 2] = y[0];

	/* 생성나무 1.이진트리   2.랜덤트리 */
	switch(select)
	{
	case 1:
		lineDDA(fractalx[n - 1], fractaly[n - 1], fractalx[n - 2], fractaly[n - 2],  n - 1, l, d);
		break;
	case 2:
		ranlineDDA(fractalx[n - 1], fractaly[n - 1], fractalx[n - 2], fractaly[n - 2],  n - 1, l, d);
		break;
	}

	
	
	// 배열에 있는 내용을 파일로 그림을 그려줌
	drawline(fp);
	fclose(fp);
	free(fractalx);
	free(fractaly);
	return 0;
}

// 흰색으로 초기화
void initline()
{
	int i, j, k;

	for(i = 0 ; i < H ; i++)
		for(j = 0 ; j < W ; j++)
			for(k = 0; k < 3; k++)
				Iarray[i][j][k] = maxlevel;
}

// 입력된 배열에 선을 그리는 작업을 해주는 함수
void drawline( FILE *fp )
{
	int i, j, k;

	for(i = 0 ; i < H ; i++)
		for(j = 0 ; j < W ; j++)
			for(k = 0; k < 3; k++)
				putc(Iarray[i][j][k],fp);
}

// 다음 좌표 지정 
void frac(int x, int y, int l1, int n, int d1)
{
	fractalx[n - 1] = x + (int) l1 * cos(RAD(d1));
	fractaly[n - 1] = y + (int) l1 * sin(RAD(d1));
}

// DDA 라인을 그려주는 함수 
// 사용자 입장으로 보기 편하게 하기 위해서
// x,y의 순서를 y먼저 받게 해줌
void lineDDA(int y1, int x1, int y2, int x2, int n, int l, double d)
{
	int dx=x2-x1, dy=y2-y1, step, i = 0, k = 0;
	int l1 = l * 0.8;
	double d1 = d;

	float xinc, yinc, x=(float)x1, y=(float)y1;

	if(abs(dx) > abs(dy)) 
		step = abs(dx);
	else 
		step = abs(dy);
	
	xinc = dx/(float)step;
	yinc = dy/(float)step;

	// 사용자 입장에서 보기 위해서 y축을 하단에서 시작하도록 바꿔줌
	for(i = 0; i < n; i++)
	{
		Iarray[H-x1][y1 + i][0] = 150;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][1] = 50;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][2] = 0;// start pixel marking(setpixel function)
	}
	
	for(k = 0; k < step; k++){
		x += xinc;
		y += yinc;
		
		// 차수가 0보다 클경우 가지 색 갈색, 차수가 0일경우 주변 점 나뭇잎 생성
		if(n > 0)
		{
			for(i = 0; i < n; i++)
			{
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][0]=150;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][1]=50;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][2]=0;// marking(setpixel function)
			}
		}
		else {
			for(i = 0; i < 15; i++){
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][0]=120;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][1]=210;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][2]=50;// marking(setpixel function)
			}
		}
	}

	// 차수가 0보다 클경우 재귀적 함수 실행
	if(n > 0)
	{
		frac(fractalx[n], fractaly[n], l1, n, d1 + 30);
		lineDDA(fractalx[n], fractaly[n] , fractalx[n - 1], fractaly[n - 1], n - 1, l1, d1 + 30);

		frac(fractalx[n], fractaly[n], l1, n, d1 - 30);
		lineDDA(fractalx[n], fractaly[n] , fractalx[n - 1], fractaly[n - 1], n - 1, l1, d1 - 30);
	}
}

void ranlineDDA(int y1, int x1, int y2, int x2, int n, int l, double d)
{
	int dx=x2-x1, dy=y2-y1, step, i = 0, k = 0;
	int l1;
	double d1 = d;

	float xinc, yinc, x=(float)x1, y=(float)y1;

	if(abs(dx) > abs(dy)) 
		step = abs(dx);
	else 
		step = abs(dy);
	
	xinc = dx/(float)step;
	yinc = dy/(float)step;

	// 사용자 입장에서 보기 위해서 y축을 하단에서 시작하도록 바꿔줌
	for(i = 0; i < n; i++)
	{
		Iarray[H-x1][y1 + i][0] = 150;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][1] = 50;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][2] = 0;// start pixel marking(setpixel function)
	}
	for(k = 0; k < step; k++){
		x += xinc;
		y += yinc;
		
		// 차수가 0보다 클경우 가지 색 갈색, 차수가 0일경우 주변 점 나뭇잎 생성
		if(n > 0)
		{
			for(i = 0; i < n; i++)
			{
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][0]=150;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][1]=50;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x))][(int)(ROUND(y)) + i][2]=0;// marking(setpixel function)
			}
		}
		else {
			for(i = 0; i < 15; i++){
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][0]=120;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][1]=210;// marking(setpixel function)
				Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][2]=50;// marking(setpixel function)
			}
		}
	}

	// 차수가 0보다 클경우 재귀적 함수 실행
	if(n > 0)
	{
		l1 = l * (rand() / (32767 * 5.0) + 0.7);
		frac(fractalx[n], fractaly[n], l1, n, d1 + (rand()%40 + 20));
		ranlineDDA(fractalx[n], fractaly[n] , fractalx[n - 1], fractaly[n - 1], n - 1, l1, d1 + 30);

		l1 = l * (rand() / (32767 * 5.0) + 0.7);
		frac(fractalx[n], fractaly[n], l1, n, d1 - (rand()%40 + 20));
		ranlineDDA(fractalx[n], fractaly[n] , fractalx[n - 1], fractaly[n - 1], n - 1, l1, d1 - 30);
	}
}

void groundlineDDA(int y1, int x1, int y2, int x2)
{
	int dx=x2-x1, dy=y2-y1, step, i = 0, k = 0;

	float xinc, yinc, x=(float)x1, y=(float)y1;

	if(abs(dx) > abs(dy)) 
		step = abs(dx);
	else 
		step = abs(dy);
	
	xinc = dx/(float)step;
	yinc = dy/(float)step;
	
	for(k = 0; k < step; k++){
		x += xinc;
		y += yinc;

		for(i = 0; i < 15; i ++)
		{
			Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][0]=80;// marking(setpixel function)
			Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][1]=180;// marking(setpixel function)
			Iarray[H-(int)(ROUND(x)) + rand() % 30][(int)(ROUND(y)) + rand() % 30][2]=80;// marking(setpixel function)
		}
	}
}