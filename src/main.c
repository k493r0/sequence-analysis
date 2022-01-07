#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
#include "ahocorasick.h"

void callback_match_total(void *arg, struct aho_match_t* m)
{
    long long int* match_total = (long long int*)arg;
    (*match_total)++;
}

void callback_match_pos(void *arg, struct aho_match_t* m)
{
    int n;

    printf("Match No. %d  ", n);

    char* text = (char*)arg;

    printf("Pattern:");
    for(unsigned int i = m->pos; i < (m->pos+m->len); i++)
    {
        printf("%c", text[i]);
    }
    // m->id is the id of the pattern
    // m->pos is the position of the match
    // m->len is the length of the pattern

    unsigned int posEnd = m->pos + m->len;

    printf(" [Position: %llu -> %llu]\n", m->pos,posEnd, m->len);
    n++;
}

void findMatch()
{

    FILE *fp;
    long lSize;
    char *buffer;
    char file_path[500];

    printf("Enter full file path");
    scanf("%s",file_path);


    fp = fopen ( file_path , "rb" );
    if( !fp ) perror(file_path),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    if( 1!=fread( buffer , lSize, 1 , fp) ) {
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);
    };

    struct ahocorasick aho;
    int id[10] = {0};
    char* target = buffer;
    aho_init(&aho);

    // Input from user
    int nPattern;
    printf("Input number of patterns: ");
    scanf("%d", &nPattern);

    // Loop for input patterns and input text
    for(int i = 0; i < nPattern; i++)
    {
        char pattern[100];
        printf("Input pattern %d: ", i+1);
        scanf("%s", pattern);
        id[i] = aho_add_match_text(&aho, pattern, strlen(pattern));
    }

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_pos, (void*)target);
//    aho_register_match_callback(&aho, callback_match_total, (void*)target);

    printf("try:  ");
    printf("total match:%u\n", aho_findtext(&aho, target, strlen(target)));

    aho_destroy(&aho);
}

int main(int argc, const char *argv[])
{
    findMatch();
    return 0;
}
