/**
 * Simplest Librtmp Receive
 *
 * 雷霄骅，张晖
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * Modify by Zhao Yi
 *
 * 本程序用于接收RTMP流媒体并在本地保存成FLV格式的文件。
 * This program can receive rtmp live stream and save it as local flv file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

/* example live url: rtmp://live.hkstv.hk.lxdns.com/live/hks */

#define MINURLLEN 16
#define MAXURLLEN 512

#define MAXFILENAMELEN 32

char *default_output = "receive.flv";


int main(int argc, char* argv[])
{
	int nRead;
	/* if it is live stream */
	unsigned int LiveStream = 1;


	/* 10 MB buffer */
	int bufsize = 1024*1024*10;
	char *buf = NULL;
	long countbufsize = 0;

	char *live_url = NULL;
	char *output = NULL;

	if((argc < 2) || (argc > 3)) {
		printf("Usage: %s <live url> [output file]\n", argv[0]);
		exit(1);
	}

	live_url = argv[1];

	if(argc == 3) {
		output = argv[2];
	} else {
		output = default_output;
	}

	buf = (char *)malloc(bufsize);
	if(!buf) {
		printf("Malloc failed\n");
		exit(1);
	}
	memset(buf, 0, bufsize);

	FILE *fp = fopen(output, "wb");
	if (!fp) {
		RTMP_LogPrintf("Open File Error.\n");
		free(buf);
		return -1;
	}

	/* set log level */
	//RTMP_LogLevel loglvl = RTMP_LOGDEBUG;
	//RTMP_LogSetLevel(loglvl);

	RTMP *rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);

	/* set connection timeout, default is 30s */
	rtmp->Link.timeout = 10;

	if(!RTMP_SetupURL(rtmp, live_url)) {
		RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
		RTMP_Free(rtmp);
		free(buf);
		fclose(fp);
		return -1;
	}

	if(LiveStream) {
		rtmp->Link.lFlags |= RTMP_LF_LIVE;
	}

	/* 1 hour */
	RTMP_SetBufferMS(rtmp, 3600*1000);

	if(!RTMP_Connect(rtmp,NULL)) {
		RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
		RTMP_Free(rtmp);
		free(buf);
		fclose(fp);
		return -1;
	}

	if(!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		free(buf);
		fclose(fp);
		return -1;
	}

	while((nRead = RTMP_Read(rtmp, buf, bufsize))) {
		fwrite(buf, 1, nRead, fp);
		countbufsize += nRead;
		RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n", nRead, countbufsize*1.0/1024);
	}

	if(fp) {
		fclose(fp);
	}

	if(buf) {
		free(buf);
	}

	if(rtmp) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		rtmp = NULL;
	}

	return 0;
}
