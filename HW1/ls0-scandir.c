/** ls0.c
 * ls -R 구현
 * 하위 디렉토리까지 전부 출력
 */
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

void do_ls(char []);

int main(int argc,char* argv[]){
    //옵션이 -R이 아니면 종료
    if(argc!=2||strcmp(argv[1],"-R")!=0){
        printf("옵션을 -R로 입력해주세요\n");
        return 0;
    }
    do_ls(".");
    return 0;
}

void do_ls(char dirname[]){
    struct dirent **dirList;
    int count;

    //scandir로 디렉토리 내 파일들을 오름차순으로 읽어옴 -1이면 오류
    if((count=scandir(dirname,&dirList,NULL,alphasort))==-1){
        printf("%s 디렉토리를 읽어오는데 실패했습니다.\n",dirname);
        return;
    }

    for(int i = 0; i < count; i++) {
        if(strcmp(dirList[i]->d_name,".")==0||strcmp(dirList[i]->d_name,"..")==0)
                continue;
        printf("%s\t", dirList[i]->d_name);
    }
    printf("\n");

    //디렉토리 내 파일들을 순회하며 디렉토리면 재귀호출
    for(int i = 0; i < count; i++) {
        if(dirList[i]->d_type==DT_DIR){
            if(strcmp(dirList[i]->d_name,".")==0||strcmp(dirList[i]->d_name,"..")==0){
                continue;
            }
            char* newDirName=(char*)malloc(sizeof(char)*100);
            strcpy(newDirName,dirname);
            strcat(newDirName,"/");
            strcat(newDirName,dirList[i]->d_name);
            printf("%s:\n",newDirName);
            do_ls(newDirName);
        }
    }

    // 건별 데이터 메모리 해제
    for(int i = 0; i < count; i++) {
        free(dirList[i]);
    }

    // namelist에 대한 메모리 해제
    free(dirList);

    return;
}