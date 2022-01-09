#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#define MAX_CHAR 256
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct sonekAgacDugumu
{
    struct sonekAgacDugumu *altEleman[MAX_CHAR];
    struct sonekAgacDugumu *sonekBagi;

    int ilk;
    int *son;
    int sonekIndex;
};

typedef struct sonekAgacDugumu Node;

char text[100]; //string girisi
char ara[50];
Node *root = NULL; // kok dugum atadik
int x = 30, y = 20, X1 = 120, Y1 = 20, temp = 20;
Node *sonDugum = NULL;   //eklenen son dugum
Node *aktifDugum = NULL; //aktif dugum
int sayac = 0;           //sayac
int index;
int aktifKenar = -1; // aktif kenar
int uzunluk = 0;     // o kenarin uzunlugu
int kalanSonek = 0;  // kalan sonek sayisi
int yaprakUcu = -1;  // yaprak ucu

int *rootEnd = NULL;
int *bolmeSonu = NULL;
int size = -1;         // girilen stringin uzunlugu
int size1 = -1;
Node *newNode(int ilk, int *son) //yeni dugum
{
    sayac++;
    Node *node = (Node *)malloc(sizeof(Node));

    for (int i = 0; i < MAX_CHAR; i++)
    {
        node->altEleman[i] = NULL;
    }


    node->sonekBagi = root; //node un sonek baglantısını da kok yaptik
    node->ilk = ilk;
    node->son = son;

    node->sonekIndex = -1;
    return node;
}

int kenarUzunlugu(Node *n) //kenar uzunlugu
{
    return (*(n->son) - (n->ilk) + 1);
}

int altKokeGit(Node *gecerliDugum) //aşagı gitmek için kullanılan true false fonksiyonu
{
    if (uzunluk >= kenarUzunlugu(gecerliDugum)) //aktif uzunluk kenar uzunlugundan buyuk mu
    {
        aktifKenar = (int)text[aktifKenar + kenarUzunlugu(gecerliDugum)] - (int)' ';
        uzunluk -= kenarUzunlugu(gecerliDugum);
        aktifDugum = gecerliDugum;
        return 1;
    }
    return 0;
}

void sonekEkle(int yer)
{
    yaprakUcu = yer; //yaprak ucu

    kalanSonek++; //kalan sonek sayısı

    sonDugum = NULL;
    while (kalanSonek > 0) //kalan sonek sayısı 0 dan buyuk oldugu surece
    {

        if (uzunluk == 0)
        {
            aktifKenar = (int)text[yer] - (int)' ';
        }

        if (aktifDugum->altEleman[aktifKenar] == NULL)
        {
            //Yeni bir yaprak kenarı oluşturulur
            aktifDugum->altEleman[aktifKenar] = newNode(yer, &yaprakUcu);

            if (sonDugum != NULL)
            {
                sonDugum->sonekBagi = aktifDugum;
                sonDugum = NULL;
            }
        }
        else
        {
            Node *next = aktifDugum->altEleman[aktifKenar];
            if (altKokeGit(next)) // altKokeGit - asagı in
            {
                //Sonraki düğümden başla
                continue;
            }

            if (text[next->ilk + uzunluk] == text[yer])
            {
                if (sonDugum != NULL && aktifDugum != root)
                {
                    sonDugum->sonekBagi = aktifDugum;
                    sonDugum = NULL;
                }
                uzunluk++;
                break;
            }

            bolmeSonu = (int *)malloc(sizeof(int));
            *bolmeSonu = next->ilk + uzunluk - 1;

            Node *ayir = newNode(next->ilk, bolmeSonu);
            aktifDugum->altEleman[aktifKenar] = ayir;

            ayir->altEleman[(int)text[yer] - (int)' '] = newNode(yer, &yaprakUcu);
            next->ilk += uzunluk;
            ayir->altEleman[aktifKenar] = next;

            if (sonDugum != NULL)
            {
                sonDugum->sonekBagi = ayir;
            }
            sonDugum = ayir;
        }

        kalanSonek--;
        if (aktifDugum == root && uzunluk > 0) //APCFER2C1
        {
            uzunluk--;
            aktifKenar = (int)text[yer - kalanSonek + 1] - (int)' ';
        }

        else if (aktifDugum != root)
        {
            aktifDugum = aktifDugum->sonekBagi;
        }
    }
}

void yazdir(int i, int j) // grafik cizdirirken cizdirme komutları burada olabilir
{

    int k;
    char h[20];
    for (k = i; k <= j; k++)
    {
        x = x + 10;
        printf("%c", text[k]);
        sprintf(h, "%c", text[k]);
        outtextxy(20, temp, "+");
        outtextxy(x, y, h);
        temp += 10;
    }
    x = 30;
    y = y + 30;
}

void dizineAktar(Node *n, int eY)
{
    if (n == NULL)
        return;

    if (n->ilk != -1) //Kok olmayan bir dugum
    {
        yazdir(n->ilk, *(n->son));
    }
    int yaprakSay = 1;

    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (n->altEleman[i] != NULL)
        {
            if (yaprakSay == 1 && n->ilk != -1)
            {
                char h[20];
                index = n->sonekIndex;
                printf(" [%d]\n", index);
                sprintf(h, "[%d].yaprak", index);

                outtextxy(X1, Y1, h);
                Y1 += 30;
            }

            yaprakSay = 0;
            dizineAktar(n->altEleman[i], eY + kenarUzunlugu(n->altEleman[i]));
        }
    }
    if (yaprakSay == 1)
    {
        n->sonekIndex = size - eY;
        index = n->sonekIndex;
        char h[20];
        printf(" [%d]\n", index);
        sprintf(h, "[%d].yaprak", index);

        outtextxy(X1, Y1, h);
        Y1 = Y1 + 30;
    }
}

void temizle(Node *n)
{
    if (n == NULL)
        return;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (n->altEleman[i] != NULL)
        {
            temizle(n->altEleman[i]);
        }
    }
    if (n->sonekIndex == -1)
        free(n->son);
    free(n);
}

void sonekOlustur()
{
    size = strlen(text);
    int i;
    rootEnd = (int *)malloc(sizeof(int));
    *rootEnd = -1;

    root = newNode(-1, rootEnd);

    aktifDugum = root; // ilk aktif dugum kok olacak
    for (i = 0; i < size; i++)
        sonekEkle(i);
    int eY = 0;
    dizineAktar(root, eY);
    temizle(root);
}

//----------------------------------------------------------------------------
void altdizi(char s[], char sub[], int p, int len)
{
    int c = 0;
    while (c < len)
    {
        sub[c] = s[p + c];
        c++;
    }
    sub[c] = '\0';
}

//Function thats gives the longest common prefix among two strings.
int lcp(char s[], char t[], char a[])
{
    int n = MIN(strlen(s), strlen(t));
    for (int i = 0; i < n; i++)
    {
        if (s[i] != t[i])
        {
            altdizi(s, a, 0, i);
            return 0;
        }
    }
   altdizi(s, a, 0, n);
    return 0;
}

//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
    initwindow(600, 400);
    char kelime[MAX_CHAR];
    char arama[MAX_CHAR];
    int sayac = 0;

    do
    {

        printf("\t\tMENU\n1-String girisi;\n2-Suffix tree\n3-Aranacak katar bilgiliri\n4-Tekrar eden en uzun katar\n5-Cikis\n\nLutfen islem seciniz:"); //menü tablosu
        switch (_getch())
        {
        case '1':
        {
            //STRİNG GİRİSİ
            FILE *dosya;
            dosya=fopen("input.txt","w");
            if(dosya == NULL)
            {
                puts("Dosya acilamadi.");
                exit(1);
            }

            cleardevice();
            system("cls");
            printf("\nLutfen stringi giriniz:");
            scanf("%s", kelime);
            fprintf(dosya,"%s\n",kelime);
            strcpy(text, kelime);
            printf("Girilen string:%s\n", kelime);
            fclose(dosya);
            break;
        }
        case '2':
        {
              cleardevice();
              system("cls");
            sonekOlustur();
            printf("Sonek agacindaki dugum sayisi:%d\n\n\n", sayac);



            break;
        }
        case '3':
        {

            system("cls");
            printf("\nLutfen aranacak stringi giriniz:");
            scanf("%s", arama);
            int sayac = 0;
            char *p = kelime;
            char *indeks = strstr(p, arama);
            if (indeks == NULL)
            {
                printf("\n%s Stringi bulunamamistir\n", arama);
            }
            else
            {
                printf("\nIlk gectigi indeks:%d \n", indeks - kelime);
                sayac++;
                p = indeks + 1;
                while (indeks != NULL)
                {
                    indeks = strstr(p, arama);
                    if (indeks > 0)
                    {
                        sayac++;
                    }

                    p = indeks + 1;
                }
            }
            printf("\n%s Stringi %d defa tekrar etmektedir\n", arama, sayac);
            sayac = 0;
            p = NULL;
            indeks = NULL;

            break;
        }
        case '4':
        {

            system("cls"); // TEKRAR EDEN EN UZUN KATAR

            char lrs[30], x[30], res[30], sub[30], sub1[30];
            int i, j, n = strlen(kelime);
            int sayac = 0;
            for (i = 0; i < n; i++)
            {
                for (j = i + 1; j < n; j++)
                {

                    altdizi(kelime, sub, i, n);
                    altdizi(kelime, sub1, j, n);
                    lcp(sub, sub1, x);

                    if (strlen(x) > strlen(lrs))
                    {
                        strcpy(lrs, x);
                    }
                }
            }
            printf("\nEn uzun tekrar eden katar: %s\n", lrs);
            char *q = kelime;
            char *say = strstr(q, lrs);
            if (say == NULL)
            {
                printf("\n%s Stringi bulunamamistir\n", lrs);
            }
            else
            {
                sayac++;
                q = say + 1;
                while (say != NULL)
                {
                    say = strstr(q, lrs);
                    if (say > 0)
                    {
                        sayac++;
                    }

                    q = say + 1;
                }
            }

            printf("%s stringinin tekrar sayisi: %d\n", lrs, sayac);
            sayac = 0;
            q = NULL;
            say = NULL;
            break;
        }

        case '5':
        {
            return 0;
            break; //cikis
        }
        default:
        {
            printf("Hatali giris");
        }
        }
    }
    while (1);
    _getch();

    closegraph();

    return 0;
}
