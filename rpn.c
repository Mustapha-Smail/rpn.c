#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/**************** Convertir l'expression en une chaine de caracteres ************************/

char * convert(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    int taille = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *str;
    str = (char*) malloc(sizeof(char)*(taille+1));

    for(int i=0; i<taille; i++) str[i]=fgetc(fp);
    str[taille]='\0';

    return str;
}

//LA PILES DE CARACTERES ET SES FONCTIONS**

struct pile {
    char *tab;
    int nb_elements;
    int taille_max;
};
typedef struct pile pile;

pile * creation_pile(int taille)
{
    pile* p;
    p=(pile*) malloc(sizeof(pile));
    p->tab=(char*) malloc(sizeof(char)*taille);
    p->taille_max=taille;
    p->nb_elements=0;
    return p;
}

int pile_est_vide(pile * p)
{
    if(p->nb_elements==0)
    {
        return 1;
    }
    return 0;
}

int pile_est_pleine(pile * p)
{
    if(p->nb_elements==p->taille_max)
    {
        return 1;
    }
    return 0;
}

int empiler(pile * p, char val)
{
    if(pile_est_pleine(p))
    {
        return INT_MIN;
    }
    else
    {
        p->tab[p->nb_elements]=val;
        p->nb_elements++;
        return val;
    }
}

int depiler(pile * p)
{
    if(pile_est_vide(p))
    {
        return INT_MIN;
    }
    else
    {
        p->nb_elements--;
        return (p->tab[p->nb_elements]);
    }
}


// LA PILE D'ENTIERS ET SES FONCTIONS**

struct pile_entiers {
    int *tab;
    int nb_elements;
    int taille_max;
};
typedef struct pile_entiers pile_entiers;

pile_entiers * creation_pile_entiers(int taille)
{
    pile_entiers* p;
    p=(pile_entiers*) malloc(sizeof(pile_entiers));
    p->tab=(int*) malloc(sizeof(int)*taille);
    p->taille_max=taille;
    p->nb_elements=0;
    return p;
}

int pile_entiers_est_vide(pile_entiers * p)
{
    if(p->nb_elements==0)
    {
        return 1;
    }
    return 0;
}

int pile_entiers_est_pleine(pile_entiers * p)
{
    if(p->nb_elements==p->taille_max)
    {
        return 1;
    }
    return 0;
}

int empile_entiers(pile_entiers * p, int val)
{
    if(pile_entiers_est_pleine(p))
    {
        return INT_MIN;
    }
    else
    {
        p->tab[p->nb_elements]=val;
        p->nb_elements++;
        return val;
    }
}

int depile_entiers(pile_entiers * p)
{
    if(pile_entiers_est_vide(p))
    {
        return INT_MIN;
    }
    else
    {
        p->nb_elements--;
        return (p->tab[p->nb_elements]);
    }
}

/**************** Convertir l'expression vers la notation en polonais inverse ************************/

int is_operator(char op)
{
    if(op == '^' || op == '*' || op == '/' || op == '+' || op =='-' || op =='%')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int priorite(char op)
{
    if (op == '^')
    {
        return (3);
    }
    else if(op == '*' || op == '/' || op == '%')
    {
        return(2);
    }
    else if(op == '+' || op == '-')
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

char *  rpn(char* str)
{
    int i=0, j=0;
    char x;

    pile * p;
    p=creation_pile(1000);

    empiler(p,'(');
    strcat(str,")");

    char * postfix;
    postfix = (char*) malloc(sizeof(char)*(strlen(str)*2));

    while(str[i] != '\0')
    {
        if(isdigit(str[i]))
        {
            while(isdigit(str[i]))
            {
                postfix[j]=str[i];
                j++; i++;
            }
            postfix[j]=' ';
            j++;
        }

        if(is_operator(str[i]) == 1)
        {
            x=depiler(p);
            while(is_operator(x) == 1 && priorite(x)>= priorite(str[i]))
            {
                postfix[j] = x;
                j++;
                x = depiler(p);
            }

            empiler(p,x);
            empiler(p,str[i]);
        }

        else if(str[i] == ')')
        {
            x = depiler(p);
            while(x != '(')
            {
                postfix[j] = x;
                j++;
                x = depiler(p);
            }
        }

        else if (str[i] == '(')
        {
            empiler(p,str[i]);
        }

        else if (str[i] == ' ') // permettre la presence des espaces dans les expressions sans les prendre en compte
        {
            ;
        }

        else
        {
            printf("\nInvalid Expression.\n");
            getchar();
            exit(1);
        }

        i++;
    }

    postfix[j] = '\0';

    return postfix;
}

int evaluer(char *postfix)
{
    pile_entiers *operandes;
    operandes=creation_pile_entiers(1000);

    int i=0, k, num1, num2, val;
    char op, buffer[512];

    while(postfix[i]!='\0')
    {
        if (isdigit(postfix[i]))
        {
            k=0;
            while (isdigit(postfix[i]))
            {
                buffer[k]=postfix[i];
                i++; k++;
            }
            buffer[k]='\0';
            char * e;
            int val=strtol(buffer, &e, 10);
            empile_entiers(operandes, val);
        }

        if (is_operator(postfix[i]))
        {
            op=postfix[i];
            num2=depile_entiers(operandes);
            num1=depile_entiers(operandes);
            switch(op){
                case '+':
                    val=num1+num2;
                break;
                case '-':
                    val=num1-num2;
                break;
                case '*':
                    val=num1*num2;
                break;
                case '^':
                    val=pow(num1,num2);
                break;
                case '/':
                    if(num2==0)
                    {
                        printf("Error: Zero Division\n");
                        return INT_MIN;
                    }
                    val=num1/num2;
                break;
                case '%':
                    if(num2==0)
                    {
                        printf("Error: Zero Division\n");
                        return INT_MIN;
                    }
                    val=num1%num2;
                break;
                default:
                    printf("Error: operateur!\n");
                break;
            }
            empile_entiers(operandes, val);
        }

        i++;
    }

    return depile_entiers(operandes);
}


int main()
{
  FILE *fp;                                                       // declarer le fichier
  fp = fopen("fichier.txt", "r");                                 // ouvrir le fichier
  if (fp == NULL){
      printf("Could not open file");
      return 1;
  }

  char *str = convert(fp);                                        // stocker l'expression dans une chaine de caracteres
  fclose(fp);                                                     // Fermer le fichier

  printf("Votre expression est:\n%s\n\n", str);                   // Afficher l'expression

  char *res = rpn(str);
  printf("La notation en polonais inverse\n%s\n\n", res);         // Convertir l'expression en polonais inverse
  printf("Resultat de l'expression: %d\n", evaluer(res));         // Evaluer l'expression
  return 0;
}
