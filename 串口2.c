//������9600 SMOD=0 ��λ���������� ��Ƭ����ʾ	�Ұ��治ͬ����ˮ��
#include<reg51.h>
#include<intrins.h>
sbit dula=P2^6;
sbit wela=P2^7;
unsigned int table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned int a,flag;
void chushihua()
{
TMOD=0x20;//��ʱ��T1 ������ʽ2  8λ �Զ�װ��
TR1=1;//T1����
SM0=0;
SM1=1;
REN=1;//��������  �������пɸ�ΪSCON= 
PCON=0;//�ȼ���SMOD=0 
TH1=0xfd;
TL1=0xfd; 
EA=1;
ES=1;
}
void diaplay(unsigned int i)
{
dula=0;
wela=0;
dula=1;
P0=table[i];
dula=0;
P0=0xff;
wela=1;
P0=0xf0;
wela=0;
P0=0xff;
}
/*void delay(unsigned int xms)
{
unsigned int i,j;
for(i=xms;i>0;i--)
	for(j=110;i>0;j--);
} */

  
void main()
{
chushihua();
while(1)
	{
	if(flag==1)
		{
		flag=0;
		a=SBUF;
		/*if(a!=0)
			P1=0xf0;*/
		diaplay(a);
		/*dula=0;
		wela=0;
		dula=1;
		P0=table[a];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xf0;
		wela=0;
		P0=0xff;*/
		}
	}
}
void aaa() interrupt 4
{
RI=0;//�����жϱ�־ 1��ʾ�ж���   ���ֶ�����
flag=1;
}	


