/*
** ��BL0�׶Σ�iROM�ڹ̻��Ĵ����ȡnandflash��SD��ǰ�����16K�����ݣ���BL1����iRAM��
** ���ȶ�ǰ16�ֽ��е�У����Ƿ���ȷ����ȷ���������������������һ���豸��
** BL1��ͷ��Ϣ�涨����
** 0x0��BL1�Ĵ�С�����16K������BL1ͷ��Ϣ�Ĵ�С��
** 0x4: 0���涨��
** 0x8��У���
** 0xC��0���涨��
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IMG_SIZE    (16 * 1024)
#define HEADER_SIZE 16
#define BLKSIZE     512

int main(int argc, char *argv[]) {
  FILE *fp;
  unsigned char *Buf;
  int BufLen;
  int nbytes, fileLen;
  unsigned int checksum, count;
  int i;

  if (argc != 3) {
    printf("Usage: %s <source file> <destination file>\n", argv[0]);
    return -1;
  }

  /* ����16K��buffer */
  BufLen = IMG_SIZE;
  Buf = malloc(BufLen);
  if (!Buf) {
    perror("Alloc buffer failed!");
    return -1;
  }
  memset(Buf, 0x00, BufLen);

  /* ��Դbin��buffer */
  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    perror("source file open error");
    free(Buf);
    return -1;
  }
  /* ��ȡԴbin���� */
  fseek(fp, 0L, SEEK_END);
  fileLen = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  /* Դbin���Ȳ��ó���16K-16byte */
  fileLen =
      (fileLen < (IMG_SIZE - HEADER_SIZE)) ? fileLen : (IMG_SIZE - HEADER_SIZE);

  /* ��Դbin��buffer[16] */
  nbytes = fread(Buf + HEADER_SIZE, 1, fileLen, fp);
  if (nbytes != fileLen) {
    perror("source file read error\n");
    free(Buf);
    fclose(fp);
    return -1;
  }
  fclose(fp);

  /* ����У��� */
  for (i = 0, checksum = 0; i < fileLen; i++)
    checksum += Buf[HEADER_SIZE + i];

  /* ����BL1�Ĵ�С:
  ** BL1�Ĵ�С����BL1��ͷ��Ϣ
  ** ����iROM��SD�������ǰ��鿽���ģ����������Ҫ������СΪ512�ֽڵ�������
  */
  fileLen += HEADER_SIZE;
  count = fileLen / BLKSIZE * BLKSIZE;
  if (count < fileLen)
    count += BLKSIZE;
  memcpy(Buf, &count, 4); // ����BL1�Ĵ�С��Buf[0-3]

  // ��У��ͱ�����buffer[8~15]
  memcpy(Buf + 8, &checksum, 4);

  fp = fopen(argv[2], "wb");
  if (fp == NULL) {
    perror("destination file open error");
    free(Buf);
    return -1;
  }
  // ��count + HEADER_SIZE�ֽڵ�buffer������Ŀ��bin��
  nbytes = fwrite(Buf, 1, count, fp);
  if (nbytes != count) {
    perror("destination file write error");
    free(Buf);
    fclose(fp);
    return -1;
  }

  free(Buf);
  fclose(fp);

  return 0;
}
