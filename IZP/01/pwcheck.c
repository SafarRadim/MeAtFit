#include <stdio.h>
#include <stdlib.h>

// Precteni argv
int loadArgv(int argc, char *argv[], int *level, int *param)
{
    if(argc < 3){
        return 1;
    } else {
        *level = atoi(argv[1]);
        *param = atoi(argv[2]);
    }

    
    return 0;
}

// Nacteni hesla z stdin
int loadPwd(char pwd[])
{
    char buffer[102];
    int j = 0;

    for(int i = 0; i < 102; i++)
        buffer[i] = 0;

    int chk = 0;

    if(fgets(buffer, 102, stdin) == NULL)
        chk = 1;
    
    if (buffer[100] != '\n' && buffer[100] != 0)
        return -1;

    for(int i = 0; i < 102; i++)
        pwd[i] = buffer[i];
    
    while(pwd[j] != '\0'){
        if(pwd[j] == '\n')
            pwd[j] = '\0';
        j++;
    }

    return chk;
}

// Informace na --stats
void statsStore(char pwd[], int *min, float *avg, int *amn, int set[])
{
    int i = 0;

    while(pwd[i] != '\0' && pwd[i] != '\n'){
        set[(int)pwd[i]] = 1;
        i++;
    }

    if(*min > i)
        *min = i;

    *avg += i;
    *amn += 1;
}

// Dopocet a tisk --stats
void stats(int *min, float *avg, int *amn, int set[])
{   
    int nchars = 0;

    for(int i = 0; i < 256; i++)
        if(set[i] == 1)
            nchars++;

    *avg /= *amn;

    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", nchars);
    printf("Minimalni delka: %d\n", *min);
    printf("Prumerna delka: %.1f\n", *avg);

}

// Pomocna funkce k secondLevel
int secondSpecial(int c)
{

    if(c >= 32 && c <= 47){
        return 1;
    } 
    if(c >= 58 && c <= 64){
        return 1;
    } 
    if(c >= 91 && c <= 96){
        return 1;
    } 
    if(c >= 123 && c <= 126){
        return 1;
    }
    return 0;
}

// zjisteni delky str
int strLen(char str[])
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    
    return len;
}

// vyhledavani substringu v hesle
int strSeach(char pwd[], int offset, int param)
{
    int i = 0;
    int j = 0;
    int pwlen = strLen(pwd);

    int chk = 0;

    while(chk != 2){
        while ((j < param) && (i < pwlen) && (pwd[j + offset] != '\0')){
            if (pwd[i] != pwd[j + offset]){
                i -= j - 1;
                j = -1;
            }
            i++;
            j++;
        }

        if (j == param){
            chk++;
            i -= j - 1;
            j = 0;
        } else {
            break;
        }
    }
    
    if (chk == 2){
        return 1;
    } else {
        return 0;
    }

}

// Kontrola prvni urovne (male a velke pismeno)
int firstLevel(char pwd[])
{
    int i = 0;
    int ckl = 0;
    int cku = 0;

    while(pwd[i] != '\0'){
        if(pwd[i] >= 97 && pwd[i] <= 122)
            cku = 1;
        if(pwd[i] >= 65 && pwd[i] <= 90)
            ckl = 1;
        if(ckl == 1 && cku == 1)
            return 0;
        i++;
    }
    return 1;
}

// Kontrola druhe urovne (x z 4 skupin -> male, velke pismeno, cislo, special)
int secondLevel(char pwd[], int param)
{
    int first = 0;
    int second = 0;
    int third = 0;
    int fourth = 0;
    int i = 0;

    while (pwd[i] != '\0'){
        // male
        if(pwd[i] >= 97 && pwd[i] <= 122)
            first = 1;

        // velke
        if(pwd[i] >= 65 && pwd[i] <= 90)
            second = 1;

        // cislo
        if(pwd[i] >= 48 && pwd[i] <= 57)
            third = 1;

        //special
        if (secondSpecial(pwd[i]) == 1)
            fourth = 1;

        i++;
    }
    if ((first + second + third + fourth) >= param
)
        return 0;

    return 1;    
}

// Kontrola treti urovne (neobsahuje x po sobe jdoucich stejnych znaku)
int thirdLevel(char pwd[], int param)
{
    int i = 0;
    int count = 0;

    while (pwd[i] != '\0'){
        for(int j = 1; j < param; j++){
            if(pwd[i] == pwd[i + j]){
                count++;
            }
        }

        if(count >= param - 1)
            return 1;
        
        i++;
        count = 0;
    }

    return 0;
}

// Kontrola ctvrte urovne (neobsahuje dva stejne podretezce delky x)
int fourthLevel(char pwd[], int param)
{
    int i = 0;

    while(pwd[i] != '\0'){
        if(strSeach(pwd, i, param) == 1)
            return 1;
        i++;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int level = 1;
    int param = 1;
    int chk = 0;

    int MIN = 200;
    float AVG = 0;
    int AMN = 0;

    int set[256];

    for(int i = 0; i < 256; i++)
        set[i] = 0;
    
    char pwd[101];

    if(loadArgv(argc, argv, &level, &param) != 0){
        fprintf(stderr, "Invalid amount of arguments, have %d need at least 2\n", (argc -1));
        return 1;
    }

    while(loadPwd(pwd) != 1){
        statsStore(pwd, &MIN, &AVG, &AMN, set);

        for(int i = 1; i < (level + 1); i++){
            switch(i){
            case 1:
                if(firstLevel(pwd) != 0)
                    chk = 1;
                break;
            
            case 2:
                if(secondLevel(pwd, param) != 0)
                    chk = 1;
                break;

            case 3:
                if(thirdLevel(pwd, param) != 0)
                    chk = 1;
                break;

            case 4:
                if(fourthLevel(pwd, param) != 0)
                    chk = 1;
                break;
            }
        }
        if(chk == 0)
            printf("%s\n", pwd);
        chk = 0;
    }

    stats(&MIN, &AVG, &AMN, set);

    return 0;
}
