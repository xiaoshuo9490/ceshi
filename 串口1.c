//波特率9600 SMOD=0 判断上位机发的是不是X x
#include<reg51.h>
unsigned char tableok[]="OK";
unsigned char tableerror[]="error";
unsigned int a,flag,i;
void chushihua()//初始化函数
{
TMOD=0x20;//定时器工作方式  T1 工作方式2  第三种
SM0=0;
SM1=1;
REN=1;//以上三行可改写为SCON   详见P117
PCON=0;//SMOD=0;
TH1=0xFD;//根据计算得到  取Fosc=12MHz  SMOD=0
TL1=0xFD;//同上
TR1=1;//启动定时器
EA=1;//总开关
ES=1;//分开关
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
				while(!TI);//等着吧ＳＢＵＦ清空　有东西TI为０　　没有为１
				TI=0;//手动清零
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
RI=0;//接收中断  SBUF由空变满为1表示以满   需手动清零
a=SBUF;
flag=1;
}