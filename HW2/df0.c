/* df 명령어구현
statfs() 함수를 이용하여 파일시스템의 정보를 얻어온다.
정상작동 합니다!
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct statfs buf;
    int i;
    char *path;
    char unit = 'K';
    long unitSize = 1024;

    if (argc == 1) {
        path = ".";
    } else {
        path = argv[1];
    }

    if(argc == 3)
        unit = argv[2][1];

    switch(unit) {
        case 'b': case 'B':
            unit = 'B';
            unitSize = 1;
            break;
        case 'k': case 'K':
            unit = 'K';
            unitSize = 1024;
            break;
        case 'm': case 'M':
            unit = 'M';
            unitSize = 1024 * 1024;
            break;
        case 'g': case 'G':
            unit = 'G';
            unitSize = 1024 * 1024 * 1024;
            break;
        default:
            printf("Usage: df [path] [unit] (unit: b, k, m, g)\n");
            exit(1);
    }

    if (statfs(path, &buf) < 0) {
        perror("statfs");
        exit(1);
    }
    
    printf("1%c%s\t%10s\t%10s\t%10s%%\n", unit, "-blocks", "Used", "Available", "MyUse");
    if(unit=='M'||unit=='G')
        printf("%10ld\t%9ld%c\t%9ld%c\t%10ld%%\n",
                buf.f_blocks * buf.f_bsize / unitSize, (buf.f_blocks - buf.f_bfree) * buf.f_bsize / unitSize, unit,
                buf.f_bavail * buf.f_bsize / unitSize, unit, 100 - buf.f_bfree * 100 / buf.f_blocks);
    else
        printf("%10ld\t%10ld\t%10ld\t%10ld%%\n",
                buf.f_blocks*buf.f_bsize/unitSize, (buf.f_blocks - buf.f_bfree)*buf.f_bsize/unitSize,
                buf.f_bavail*buf.f_bsize/unitSize, 100 - buf.f_bfree*100/buf.f_blocks);

    return 0;
}