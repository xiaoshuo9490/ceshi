//������9600 SMOD=0 �ж���λ�������ǲ���X x
#include<reg51.h>
unsigned char tableok[]="OK";
unsigned char tableerror[]="error";
unsigned int a,flag,i;
void chushihua()//��ʼ������
{
TMOD=0x20;//��ʱ��������ʽ  T1 ������ʽ2  ������
SM0=0;
SM1=1;
REN=1;//�������пɸ�дΪSCON   ���P117
PCON=0;//SMOD=0;
TH1=0xFD;//���ݼ���õ�  ȡFosc=12MHz  SMOD=0
TL1=0xFD;//ͬ��
TR1=1;//������ʱ��
EA=1;//�ܿ���
ES=1;//�ֿ���
}
void main()
{
chushihua();
flag=0;
while(1)
	{
	if(flag==1)
		{
		ES=0;
		flag=0;
		if(a=='X')
			{
			for(i=0;i<2;i++)
				{
				SBUF=tableok[i];
				while(!TI);
				TI=0;
				}
			}
		else if(a=='x')
			{
			for(i=0;i<2;i++)
				{
				SBUF=tableok[i];
				while(!TI);//���Űɣӣ£գ���ա��ж���TIΪ������û��Ϊ��
				TI=0;//�ֶ�����
				}
			}
		else
			{
			for(i=0;i<5;i++)
				{
				SBUF=tableerror[i];
				while(!TI);
				TI=0;
				}
			}
		ES=1;

		}
	}
}
void aaa() interrupt 4
{
RI=0;//�����ж�  SBUF�ɿձ���Ϊ1��ʾ����   ���ֶ�����
a=SBUF;
flag=1;
}