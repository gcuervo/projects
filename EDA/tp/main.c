#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CANT_AUTHORS    20
#define NAME_LENGTH     40
#define BOOK_QUANTITY   20
#define SALIR           6

#define OK              0
#define NO_INPUT        1
#define TOO_LONG        50

typedef struct author
{
    char nombreAutor[40];
    struct book *books;
    struct author *sigAuth;
} nodo;

typedef struct book
{
    char title[40];
    struct book *sigBook;
} nodosec;

nodo* deleteAuthor(nodo *authors,char *author);
void deleteBook(nodo* multiList,char *author,char *book);
void ingresarinmuebles(nodo *authors,char **books);
void mostrarLibros(nodosec *books);
void mostrarAutores(nodo *autores);
int isLetter(char *c);
char* toUpper(char autor[NAME_LENGTH]);
char** getAutorLibros(FILE* file,char *autor);
char** getAuthors(FILE* file);
void printAuthors(char** authors);
void ingresarAutores(FILE *file,FILE* file2,nodo* autores,char** authorName);
void  insertarLibros(nodo *nodo,char *author,FILE *file);
void insertAuthor(nodo *multiList,char* author);
void freeBooks(nodosec* books);


int main()
{
    nodo* authors = (struct nodo*) malloc(sizeof(nodo));
    FILE* file1 = fopen("autoresLibros.txt","r");
    FILE* file2 = fopen("libros.txt","r");
    if(file1 == NULL )
    {
        printf("Error al abrir el archivo");
    }
    char** authorNames;
    authorNames = getAuthors(file1);
    //printAuthors(authorNames);
    //char **books=getAutorLibros(file2,"patrick rothfuss");
    printf("Lista:\n");
    ingresarAutores(file1,file2,authors,authorNames);
    printf("------------------\n");
    mostrarAutores(authors);
    int choice;
    printf("ingrese la opcion a realizar:\n");
    printf("1- Ingresar Autor:\n");
    printf("2- Ingresar Libro:\n");
    printf("3- Eliminar Autor:\n");
    printf("4- Eliminar Libro:\n");
    printf("5- Mostrar Lista:\n");
    printf("6- Salir\n");
    scanf("%d",&choice);
    char author[NAME_LENGTH];
    char book[NAME_LENGTH];
    while(choice!=SALIR)
    {
        switch(choice)
        {
        case 1:
            printf("ingrese el autor:\n");
            getText(author);
            insertAuthor(authors,author);
            //mostrarAutores(authors);
            break;
        case 2:
            printf("ingrese el autor:\n");
            getText(author);
            printf("ingrese el libro:\n");
            getText(book);
            insertBook(authors,author,book);
            break;
        case 3:
            printf("ingrese el autor:\n");
            getText(author);
            authors = deleteAuthor(authors,author);
            break;
        case 4:
            printf("ingrese el autor:\n");
            getText(author);
            printf("ingrese el libro:\n");
            getText(book);
            deleteBook(authors,author,book);
            break;
        case 5:
            mostrarAutores(authors);
        }
        if(choice!=5)
        {
            printf("Nueva Lista:\n");
            mostrarAutores(authors);
        }
        printf("------------------\n");
        printf("ingrese la opcion a realizar:\n");
        printf("1- Ingresar Autor:\n");
        printf("2- Ingresar Libro:\n");
        printf("3- Eliminar Autor:\n");
        printf("4- Eliminar Libro:\n");
        printf("5- Mostrar Lista:\n");
        printf("6- Salir\n");
        scanf("%d",&choice);
    }

    fclose(file1);
    fclose(file2);
    return 0;
}

void printAuthors(char** authors)
{
    int i;
    for(i=0; strcmp(authors[i],"X")!=0; i++)
    {
        printf("%s\n",authors[i]);
    }
}


char** getAuthors(FILE* file)
{
    char** authorNames;
    int i,j;
    int readError=0;
    authorNames = (char**) malloc(sizeof(char)*CANT_AUTHORS); // creo la cantidad de autores a tener (filas)

    for(i=0; i<CANT_AUTHORS; i++)
    {
        authorNames[i] = (char*) malloc(sizeof(char)*NAME_LENGTH);   // creo la cantidad de letras que puede tener el nombre (columnas)
    }

    i=0;
    char *name,*lastName,*result;
    name = (char*)malloc(sizeof(char)*30);
    lastName = (char*)malloc(sizeof(char)*30);
    result = (char*)malloc(sizeof(char)*60);
    while(fscanf(file,"%s %s",name,lastName)==2)
    {
        strcpy(result,name);
        strcat(result," ");
        strcat(result,lastName);
        strcpy(authorNames[i],result);
        i++;
    }
    authorNames[i]="X"; //terminan los nombres
    free(name);
    free(lastName);
    free(result);
    return authorNames;
}

void ingresarAutores(FILE *file,FILE *file2,nodo* autores,char** authorName)
{
    strcpy(autores->nombreAutor,*authorName);

    if(strcmp(autores->nombreAutor,"X")==0)
    {
        autores->sigAuth = NULL;
        return;
    }
    else
    {
        //autores->books = (nodosec*)malloc(sizeof(nodosec));
        insertarLibros(autores,*authorName,file2);
        autores->sigAuth = (nodo*) malloc(sizeof(nodo));
        ingresarAutores(file,file2,autores->sigAuth,authorName+1);
    }
}



char** getAutorLibros(FILE* file,char *autor)
{
    char c;
    char auxAuthor[30],**books=NULL;
    int i;
    books = (char**)malloc(sizeof(char*)*BOOK_QUANTITY); //20 es la cantidad de libros
    for(i=0; i<BOOK_QUANTITY; i++)
    {
        books[i] = (char*) malloc(sizeof(char)*NAME_LENGTH);
    }
    int j;
    int index=0;
    int anotherWord=0;
    int hasBooks=0;
    char *name,*lastName,*result;
    //name = (char*)malloc(sizeof(char)*30);
    int finish=0;
    //lastName = (char*)malloc(sizeof(char)*30);
    i=0;
    j=0;
    while((c=fgetc(file))!= EOF && !finish)
    {
        while(c!=':')
        {
            if(isLetter(c))
                auxAuthor[index++]=c;
            c=fgetc(file);
        }
        auxAuthor[index]='\0';

        if(strcmp(strupr(auxAuthor),toUpper(autor)) == 0)    // si es el autor que estoy buscando agrego sus libros
        {
            while(c != '.')
            {
                if(c == ',')
                {
                    books[i][j]='\0';
                    i++;
                    j=0;
                    anotherWord=1;
                }
                else
                {
                    if(isLetter(c))
                        books[i][j++]=c;
                }
                c=fgetc(file);
            }
            books[i][j]='\0';
            hasBooks = 1;
            //printf("dentro de funcion:%s\n",books[i]);
            books[i+1]="X";
            //printAuthors(books);
            finish=1;
        }
        while(!finish && (c=fgetc(file))!= '.')   // saltea los libros que no me interesan
        {
        }

        index=0;
    }
    rewind(file);
    if(!hasBooks)
        books=NULL;
    return books;
}

void insertarLibros(nodo *nodo,char *author,FILE *file)
{
    char** books = getAutorLibros(file,author);
    int i;
    if(books!=NULL)
    {
        //strcpy(nodo->books->title,books[0]);
        // nodosec *aux;//=nodo->books->sigBook;
        //nodo->books->sigBook = aux;
        nodosec *n;
        int first=1;
        for(i=0; strcmp(books[i],"X")!=0; i++)
        {
            if(first)
            {
                nodo->books = (nodosec*)malloc(sizeof(nodosec));
                strcpy(nodo->books->title,books[i]);
                nodo->books->sigBook=NULL;

                first=0;
            }
            else
            {
                n =(nodosec*) malloc(sizeof(nodosec));

                strcpy(n->title,books[i]);
                //aux->sigBook = (nodosec*) malloc(sizeof(nodosec));
                //aux = aux->sigBook;

                n->sigBook = nodo->books;
                nodo->books = n;

            }

        }
        //aux= NULL;
    }
    else
        nodo->books=NULL;

}
/*
void ingresarinmuebles(nodo *authors,char **books)
{

    nodosec *nuevo;
	// en caso de ingresar un X corto la ejecucion, no tiene sentido preguntar
	// por mas datos
	if (strcmp(*books, "X") == 0)
		{
		return;
		}
	else
		{
		// empiezo a guardar los valores
		// si es el primero que ingreso el siginm deberia ser distinto de NULL
		if (authors->books->sigBook!= NULL)
			{
			strcpy(authors->books->title, *books);
			authors->books->sigBook=NULL;
			}
		else // Caso contrario debo crear uno nuevo que va a quedar entre el
		// anterior ingresado y el primero al que apunta nodo
			{
			nuevo=(nodosec *)malloc(sizeof(nodosec));
			strcpy(nuevo->title, *books);
			nuevo->sigBook=authors->books;
			authors->books=nuevo;
			}
		ingresarinmuebles(authors,books+1);
		}
}

*/
int isLetter(char *c)
{
    int ans=0;
    if(c>='A' && c<='Z' || c==' ')
    {
        ans = 1;
    }
    if(c>='a' && c<='z')
    {
        ans=1;
    }
    return ans;
}

char* toUpper(char autor[NAME_LENGTH])
{
    int i;
    char* upper;
    upper = (char*) malloc(sizeof(char)*NAME_LENGTH);
    for(i=0 ; autor[i] != '\0' ; i++)
    {
        if(autor[i]>='a' && autor[i]<='z')
        {
            upper[i] = 'A' + (autor[i] -'a');
        }
        else
            upper[i]=autor[i];
    }
    upper[i]='\0';
    return upper;
}

void mostrarAutores(nodo *autores)
{

    if(autores->sigAuth==NULL)
    {
        return;
    }
    else
    {
        printf("Autor: %s\n",autores->nombreAutor);
        mostrarLibros(autores->books);
        printf("\n");
        mostrarAutores(autores->sigAuth);
    }

}

void myMostrar(char *text)
{

    int i;
    for(i=0; text[i]!='\0'; i++)
    {
        printf("%c",text[i]);
    }
}

void mostrarLibros(nodosec *books)
{
    if(books!=NULL)
        printf("Libro: %s\n",books->title);
    if(books==NULL)
        return;
    if(books->sigBook == NULL)
    {
        return;
    }
    else
    {
        //printf("Libro: %s\n",books->title);
        mostrarLibros(books->sigBook);
    }
}


void insertAuthor(nodo *multiList,char* author)
{
    nodo *aux=multiList;
    nodo *n,*ant;
    while(strcmp(aux->nombreAutor,"X")!=0)
    {
        ant = aux;
        aux = aux->sigAuth;
    }
    printf("el nodo actual es: %s\n",aux->nombreAutor);
    n = (nodo*) malloc(sizeof(nodo));
    strcpy(n->nombreAutor,author);
    n->sigAuth = aux;
    ant->sigAuth = n;
    //aux->sigAuth->books = NULL;
    printf("El autor se agrego correctamente\n");
}

void insertBook(nodo* multiList,char *author,char *book)
{

    nodo *aux=multiList;
    while(strcmp(aux->nombreAutor,"X")!=0 && strcmp(toUpper(aux->nombreAutor),toUpper(author))!=0 )
    {
        aux = aux->sigAuth;
    }

    if(strcmp(toUpper(aux->nombreAutor),toUpper(author))==0)
    {
        nodosec *auxBooks = aux->books;
        if(aux->books!=NULL)
        {
            while(auxBooks->sigBook!=NULL)
            {
                auxBooks = auxBooks->sigBook;
            }
            auxBooks->sigBook = (nodosec*)malloc(sizeof(nodosec));
            strcpy(auxBooks->sigBook->title,book);
            auxBooks->sigBook->sigBook = NULL;
        }
        else
        {
            aux->books = (nodosec*) malloc(sizeof(nodosec));
            aux->books = book;
            aux->books->sigBook = NULL;
        }
        printf("El libro se agrego correctamente!\n");
    }

}

void getText(char *text)
{
    char c;
    int i=0;
    while((c=getchar())!='9')
    {
        if(isLetter(c))
            text[i++]=c;
    }
    text[i]='\0';
}

void deleteBook(nodo* multiList,char *author,char *book)
{

    nodo *aux=multiList;
    while(strcmp(aux->nombreAutor,"X")!=0 && strcmp(toUpper(aux->nombreAutor),toUpper(author))!=0 )
    {
        aux = aux->sigAuth;
    }
    if(strcmp(toUpper(aux->nombreAutor),toUpper(author))==0)
    {
        nodosec *auxBooks = aux->books;
        if(aux->books!=NULL)
        {
            nodosec *antBook;
            while(strcmp(toUpper(auxBooks->title),toUpper(book))!=0)
            {
                antBook = auxBooks;
                auxBooks = auxBooks->sigBook;
            }
            if(strcmp(toUpper(auxBooks->title),toUpper(book))==0)
            {
                antBook->sigBook = auxBooks->sigBook;
                free(auxBooks);
                printf("El libro ah sido destruido!\n");
            }else
                printf("El libro no se encuentra en los registros");
        }
        else
        {
            printf("Este autor no tiene libros para borrar\n");
        }

    }
}

nodo* deleteAuthor(nodo *authors,char *authorName)
{

    nodo *aux=authors;
    nodo *ant=authors;
    int head=1;
    while(strcmp(aux->nombreAutor,"X")!=0 && strcmp(toUpper(aux->nombreAutor),toUpper(authorName))!=0 )
    {
        ant = aux;
        aux = aux->sigAuth;
        head=0;
    }

    if(strcmp(toUpper(aux->nombreAutor),toUpper(authorName))==0)
    {
        if(head){
            ant = authors->sigAuth;
        }else{
            ant->sigAuth= aux->sigAuth;
        }
        freeBooks(aux->books);
        free(aux);
    }
    else
        printf("No existe el autor\n");
    return ant;
}

void freeBooks(nodosec* books){

    if(books->sigBook==NULL){
        return;
    }else{
    freeBooks(books->sigBook);
    }
    free(books);
}