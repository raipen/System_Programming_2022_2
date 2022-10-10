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

typedef struct node{
    struct dirent *direntp;
    struct node *next;
}direntNode;

void do_ls(char []);
direntNode* scanAndSortDir(char*);
void insertNode(direntNode**, struct dirent*);

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
    //디렉토리 내 파일들을 오름차순으로 읽어옴
    direntNode *head=scanAndSortDir(dirname);

    for(direntNode* cur = head; cur; cur = cur->next)
        printf("%s\t", cur->direntp->d_name);
    printf("\n");

    //디렉토리 내 파일들을 순회하며 디렉토리면 재귀호출
    for(direntNode* cur = head; cur; cur = cur->next){
        if(cur->direntp->d_type==DT_DIR){
            if(strcmp(cur->direntp->d_name,".")==0||strcmp(cur->direntp->d_name,"..")==0){
                continue;
            }
            char* newDirName=(char*)malloc(sizeof(char)*100);
            strcpy(newDirName,dirname);
            strcat(newDirName,"/");
            strcat(newDirName,cur->direntp->d_name);
            printf("%s:\n",newDirName);
            do_ls(newDirName);
        }
    }

    // 연결리스트 메모리 해제
    direntNode* cur = head;
    while(cur){
        direntNode* next = cur->next;
        free(cur);
        cur = next;
    }
}

direntNode* scanAndSortDir(char* dirname){
    DIR *dir_ptr;
    struct dirent *direntp;
    direntNode *head=NULL;

    if ((dir_ptr = opendir(dirname)) == NULL){
        fprintf(stderr, "cannot open %s\n", dirname);
        return NULL;
    }
    while ((direntp = readdir(dir_ptr)) != NULL){
        if(strcmp(direntp->d_name,".")==0||strcmp(direntp->d_name,"..")==0)
            continue;
        insertNode(&head,direntp);
    }
    closedir(dir_ptr);

    return head;
}

void insertNode(direntNode** head, struct dirent* newDirentp){
    direntNode* newNode=(direntNode*)malloc(sizeof(direntNode));
    newNode->direntp=newDirentp;
    newNode->next=NULL;

    if(*head==NULL){
        *head=newNode;
        return;
    }

    direntNode* cur=*head;
    direntNode* prev=NULL;
    while(cur){
        if(strcmp(cur->direntp->d_name,newDirentp->d_name)>0){
            if(prev==NULL){
                newNode->next=cur;
                *head=newNode;
            }
            else{
                newNode->next=cur;
                prev->next=newNode;
            }
            return;
        }
        prev=cur;
        cur=cur->next;
    }
    prev->next=newNode;
}