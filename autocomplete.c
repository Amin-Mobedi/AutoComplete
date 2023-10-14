#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(AUTOCOMPLETE_H)
#define AUTOCOMPLETE_H

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;


void read_in_terms(struct term **terms, int *pnterms, char *filename);
int lowest_match(struct term *terms, int nterms, char *substr);
int highest_match(struct term *terms, int nterms, char *substr);
void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr);

#endif

double getweight(char* array,double weight){
    int j = 0;
    char num[200];
    while(array[j] != '\t'){
        num[j] = array[j];
        j++;
    }
    num[j+1] = '\0';
    weight = atof(num);
    return weight;
}

char* getcity(char* array,char city[]){
    int j = 0;
    while(array[j] != '\t'){
        j++;
    }
    j++;
    int z = 0;
    while(array[z] != '\n'){
        z++;
    }
    
    strcpy(city,(array+j));
    city[z-j] = '\0';
    return city;


}
int cmp_terms(const void *a, const void *b) {
        const term *term_a = (const term *)a;
        const term *term_b = (const term *)b;

        return strcmp(term_a->term, term_b->term);
}


int get_count(char array[]){
    int i = 0;
    while(array[i]!='\n'){
        i++;
    }
    i++;
    array[i] = '\0';
    double count = atof(array);
    return (int) count;
}

void read_in_terms(term **terms, int *pnterms, char *filename){

   
    FILE* fptr = fopen(filename, "r");

    char line[200];
   fgets(line, sizeof(line), fptr);
    
    *pnterms = get_count(line);
    *terms = malloc(*pnterms * sizeof(term));

    for(int i = 0; i < *pnterms; i++){
        fgets(line,sizeof(line),fptr);
        double weight;
        weight = getweight(line,weight);
        char city[200];
        strcpy((*terms+i)->term,getcity(line,city));
            
        (*terms+i)->weight = weight;
        
    

    }
    qsort(*terms, *pnterms, sizeof(term), cmp_terms);

    

    fclose(fptr);
    

}

int lowest_match(term *terms, int nterms, char *substr){

    int left = 0;
    int right = nterms;
    int length = strlen(substr);
    while(left<right){
        int mid = (left + right) / 2;
        char mid_el[200];
        for(int i=0; i<length;i++){
            mid_el[i] = (terms+mid)->term[i];
        }
        mid_el[length]='\0';
        
        if(strcmp(mid_el,substr)<0){
            left = mid + 1;
        }else{
            right = mid;
        }

    }

    return left;
            

}

int highest_match(term *terms, int nterms, char *substr){

    int left = 0;
    int right = nterms;
    int length = strlen(substr);
    while(left<right){
        int mid = (left + right) / 2;
        char mid_el[200];
        for(int i=0; i<length;i++){
            mid_el[i] = (terms+mid)->term[i];
        }
        mid_el[length]='\0';
        
        if(strcmp(mid_el,substr)>0){
            right = mid;
            
        }else{
            left = mid + 1;
            
        }

    }

    return right-1;
            

}

int cmpr_weight(const void* p_a,const void* p_b){

    const term* weight_a = (const term*) p_a;
    const term* weight_b = (const term*) p_b;

    return -weight_a->weight+weight_b->weight;

}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){

    int left_index  = lowest_match(terms,nterms,substr);
    int right_index = highest_match(terms,nterms,substr);
    *n_answer = right_index-left_index+1;
    *answer = malloc(nterms * sizeof(term));
    for(int i = left_index; i < right_index+1; i++){
        (*answer+i-left_index)->weight = (terms+i)->weight;
        strcpy((*answer+i-left_index)->term,(terms+i)->term);
    }

    qsort(*answer, *n_answer , sizeof(term), cmpr_weight);

}

/*
int main(){
    

    term* terms;
    int pnterms;
    term *answer;
    int n_answer;
    read_in_terms(&terms, &pnterms, "cities.txt");
    printf("\nHello, World!");
    printf("%d\n",lowest_match(terms, pnterms,"Tor"));
    printf("%d\n",highest_match(terms, pnterms,"Tor"));
    autocomplete(&answer, &n_answer, terms, pnterms, "Tor");  
    free(terms);
    free(answer);
    return 0;
}

*/