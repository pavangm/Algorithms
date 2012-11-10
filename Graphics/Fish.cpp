//**************************************
// Name: fish
// Description:this code implements moving fish using the predefined functions in graphics.h
// By: pavan g m
//
//This code is copyrighted and has// limited warranties.Please see http://www.Planet-Source-Code.com/vb/scripts/ShowCode.asp?txtCodeId=10432&lngWId=3//for details.//**************************************

/*
name: pavan g m
emailid: pavangm@gmail.com
*/
#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
//WEED IS A KIND OF SEA-WEED SIMULATION FOUND AT BOTTOM OF SEA BED.
weed(int x, int h)
{
	int i,k;
	static int j=0;
	setcolor(2);
	for(i=h,k=j%5;i>0;i=i-10,k++)
	{
	arc(x,getmaxy()-i,270+k*180,90+k*180,5); // TO ADD THICKNESS
	arc(x+1,getmaxy()-i,270+k*180,90+k*180,5);
	arc(x+2,getmaxy()-i,270+k*180,90+k*180,5);
	}
	setcolor(7);
	j++;
}
//FISH FUNCTION PROVIDES BASIC FISH STRUCTURE
fish(int x,int y)
{
	static int k;
	ellipse(x,y, 20,160, 50,30) ;
	ellipse(x,y-20, 200, 342 , 50, 30) ;
	setcolor(8);
	pieslice(x+35,y-14,0, 360,2);
	setcolor(7);
	circle(x+48,y-10-k%35,1);
	circle(x+48,y-10-k%20,1);
	line(x-60,y-20,x-47,y-10);
	line(x-60,y,x-47,y-10);
	line(x-60,y-20,x-60,y);
	line(x+35,y-8,x+47,y-10) ;
	k++;
}
stone(int x,int x1,int y1)
{
setcolor(6);
ellipse(x,getmaxy()-y1,0,360,x1,y1);
setfillstyle(SOLID_FILL,8) ;
fillellipse(x,getmaxy() -y1, x1 ,y1) ;
setcolor(7);
}
int main()
{
int gdriver=DETECT, gmode, i ;
initgraph(&gdriver,&gmode,"c:\\tc\\bgi");
setbkcolor(9);
setcolor(7);
//simulates fish movement
for(i=0; i<500&&!kbhit();i++) //WAITS UNTILL A KEY IS HIT ON KEYBOARD
{
	fish (100+2*i, 300);
	weed(10, 60);
	weed (350, 70) ;
	weed (260, 56);
	weed (179 ,45);
	stone(100,15,10);
	stone(300,20,12);
	stone(250,10,5) ;
	outtextxy(400,450,"press any key to stop");
	delay(90);
	cleardevice();
}
closegraph();
return 0;
}