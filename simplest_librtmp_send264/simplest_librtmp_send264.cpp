/**
 * Simplest Librtmp Send 264
 *
 * �����裬����
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ���������ڽ��ڴ��е�H.264����������RTMP��ý���������
 *
 */

#include <stdio.h>
#include "librtmp_send264.h"



FILE *fp_send1;

//���ļ��Ļص�����
int read_buffer1(unsigned char *buf, int buf_size ){
	if(!feof(fp_send1)){
		int true_size=fread(buf,1,buf_size,fp_send1);
		return true_size;
	}else{
		return -1;
	}
}

int main(int argc, char* argv[])
{
	fp_send1 = fopen("cuc_ieschool.h264", "rb");

	//��ʼ�������ӵ�������
	Connect("rtmp://192.168.199.166/publishlive/livestream");
	//����
	SendH264Data(read_buffer1);

	//�Ͽ����Ӳ��ͷ������Դ
	Close();

	return 0;
}
