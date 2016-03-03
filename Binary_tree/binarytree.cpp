#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#define H 750			// ����
#define W 750			// �ʺ�
#define maxlevel 255
#define ROUND(f) (int)(f+0.5)
#define RAD(d) (double)(d*3.1416/180)

unsigned char Iarray[H][W][3];//for an interleaved color array
unsigned char garray[H][W][3];//for an interleaved color array

double* fractalx;
double* fractaly;

void initline();			// �迭 ������� �ʱ�ȭ
void drawline( FILE *fp );	// �迭 �׸���
void lineDDA(int y1, int x1, int y2, int x2, int n, int l, double);		// ����Ʈ�� LinDDA (���� ���� ����)
void ranlineDDA(int y1, int x1, int y2, int x2, int n, int l, double d);	// ����Ʈ�� LineDDA
void groundlineDDA(int y1, int x1, int y2, int x2);							// �ܵ� LineDDA

void frac(int x, int y, int l1, int n, int d1);							// ���� ��ǥ ����

int main(void){

	FILE* fp;

	int i , j;
	int n;
	int select;
	int l;
	int height = 140;
	double d;

	d = 90;			// �ʱ� �� 90���� ����
	l = 100;		// �ʱ� ���� 100���� ����

	double x[2]={50,50};
	double y[2]={200,300};

	fp=fopen("tree.ppm","wb"); //fp=fopen("test.ppm","w");
	fprintf(fp,"P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp,"%d %d\n",W,H); //file size 
	fprintf(fp,"%d\n",maxlevel); //max level 

	printf("1.����Ʈ��   2. ����Ʈ��\n������ ���� ���� :");
	scanf("%d", &select);
	
	while(1)
	{
		printf("������ �Է� (7 ~ 12) : ");
		scanf("%d", &n);

		if(n > 6 && n < 13)
			break;
		else
			printf("������ ������ ���� �Է��Ͻÿ�!\n");
	}

	initline();		// ��� �ʱ�ȭ

	fractalx = (double*)malloc(n*sizeof(double));
	fractaly = (double*)malloc(n*sizeof(double));

	// �ܵ� �׸���
	for(i = 0; i < 5; i++)
	{
		groundlineDDA(0, height, 750, height);
		height+=30;
	}

	// �ʱ� ��ǥ ����
	fractalx[n - 1] = x[1] + (n * 25);
	fractaly[n - 1] = y[1];
	fractalx[n - 2] = x[0] + (n * 25);
	fractaly[n - 2] = y[0];

	/* �������� 1.����Ʈ��   2.����Ʈ�� */
	switch(select)
	{
	case 1:
		lineDDA(fractalx[n - 1], fractaly[n - 1], fractalx[n - 2], fractaly[n - 2],  n - 1, l, d);
		break;
	case 2:
		ranlineDDA(fractalx[n - 1], fractaly[n - 1], fractalx[n - 2], fractaly[n - 2],  n - 1, l, d);
		break;
	}

	
	
	// �迭�� �ִ� ������ ���Ϸ� �׸��� �׷���
	drawline(fp);
	fclose(fp);
	free(fractalx);
	free(fractaly);
	return 0;
}

// ������� �ʱ�ȭ
void initline()
{
	int i, j, k;

	for(i = 0 ; i < H ; i++)
		for(j = 0 ; j < W ; j++)
			for(k = 0; k < 3; k++)
				Iarray[i][j][k] = maxlevel;
}

// �Էµ� �迭�� ���� �׸��� �۾��� ���ִ� �Լ�
void drawline( FILE *fp )
{
	int i, j, k;

	for(i = 0 ; i < H ; i++)
		for(j = 0 ; j < W ; j++)
			for(k = 0; k < 3; k++)
				putc(Iarray[i][j][k],fp);
}

// ���� ��ǥ ���� 
void frac(int x, int y, int l1, int n, int d1)
{
	fractalx[n - 1] = x + (int) l1 * cos(RAD(d1));
	fractaly[n - 1] = y + (int) l1 * sin(RAD(d1));
}

// DDA ������ �׷��ִ� �Լ� 
// ����� �������� ���� ���ϰ� �ϱ� ���ؼ�
// x,y�� ������ y���� �ް� ����
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

	// ����� ���忡�� ���� ���ؼ� y���� �ϴܿ��� �����ϵ��� �ٲ���
	for(i = 0; i < n; i++)
	{
		Iarray[H-x1][y1 + i][0] = 150;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][1] = 50;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][2] = 0;// start pixel marking(setpixel function)
	}
	
	for(k = 0; k < step; k++){
		x += xinc;
		y += yinc;
		
		// ������ 0���� Ŭ��� ���� �� ����, ������ 0�ϰ�� �ֺ� �� ������ ����
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

	// ������ 0���� Ŭ��� ����� �Լ� ����
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

	// ����� ���忡�� ���� ���ؼ� y���� �ϴܿ��� �����ϵ��� �ٲ���
	for(i = 0; i < n; i++)
	{
		Iarray[H-x1][y1 + i][0] = 150;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][1] = 50;// start pixel marking(setpixel function)
		Iarray[H-x1][y1 + i][2] = 0;// start pixel marking(setpixel function)
	}
	for(k = 0; k < step; k++){
		x += xinc;
		y += yinc;
		
		// ������ 0���� Ŭ��� ���� �� ����, ������ 0�ϰ�� �ֺ� �� ������ ����
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

	// ������ 0���� Ŭ��� ����� �Լ� ����
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