#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct n
{
	char adres[30];
	int sayac;
	struct n *next;
	struct n *prev;
};
typedef struct n node;

int say = 0;

node *doEverything(node *r, char b[30], int slen, int blen)
{
	// Hiç yoksa(oluşturulmamışsa) yeni bir node oluşturur.
	if (r == NULL)
	{
		r = (node *)malloc(sizeof(node));
		r->next = NULL;
		r->prev = NULL;
		strcpy(r->adres, b);
		r->sayac++;
		return r;
	}

	// Eğer varsa, sayaç arttırma, eşik kontrolü gibi işlemleri yapar.
	node *iter;
	iter = r;
	while (iter != NULL)
	{
		if (!strcmp(iter->adres, b)) // Gelen adres ile iterasyonun bulunduğu node'daki adresin aynı olup olmamasını kontrol eder.
		{
			iter->sayac++;
			if (iter->sayac > slen)
			{
				if (iter->prev == NULL)
				{
					return r;
				}
				else if (iter->next == NULL)
				{
					iter->next = r;
					iter->prev->next = NULL;
					iter->prev = NULL;
					r->prev = iter;
					return iter;
				}
				else
				{
					iter->prev->next = iter->next;
					iter->next->prev = iter->prev;
					iter->next = r;
					iter->prev = NULL;
					r->prev = iter;
					return iter;
				}
			}

			return r;
		}
		iter = iter->next;
	}
	// Eğer listede adresi uyan eleman yoksa yeni bir tane oluşturur. Duruma göre eşik sayılarını kontrol eder.
	node *temp = (node *)malloc(sizeof(node));
	strcpy(temp->adres, b);
	temp->next = r;
	temp->sayac = 1;
	temp->prev = NULL;
	r->prev = temp;
	say++;
	node *temp2 = temp;

	// Eğer cache buffer kapasitesi aşılırsa, son düğümün silinmesi. Aşılmadıysa oluşturulan node'un direk olarak root(head) olması.
	if (blen <= say)
	{
		while (temp2->next != NULL)
		{

			temp2 = temp2->next;
		}
		temp2->prev->next = NULL;
		free(temp2);
		say--;
		return temp;
	}
	return temp;
}

// Linked list'i sonuna kadar ziyaret edip okuduğu adresleri ve sayaçları ekrana yazar 
void yazdir(node *r)
{
	while (r != NULL)
	{
		printf(" %s(%d)", r->adres, r->sayac);
		r = r->next;
	}
}

// Bu fonksiyon sadece, cache buffer'ın temizlenmesi istendiğinde çalışır.
void delete (node *r)
{
	// Geçici bir node açıp, listenin son elemanına kadar ulaşıp silen(bu geçici node'u geçiş aracı olarak kullanarak) bir while döngüsü
	// En son açıkta kalan node'u da silmek için "r= NULL" kullandım
	node *temp;
	while (r->next != NULL)
	{
		yazdir(r);
		printf("\n");
		temp = r;
		r = r->next;
		temp->prev = NULL;
		temp->next = NULL;
		free(temp);
	}
	yazdir(r);
		printf("\n");
	r = NULL;
}
int main()
{

	node *root;
	root = NULL;
	int i,t,l,secim;
	char a[30];
	char b[1];
	FILE *abc;
	char emacs[500];
	printf("Değerleri konsoldan girmek için '1', klasörden  okumak için '2' tuşlayınız\n");
	scanf("%d", &secim);
	// Girilen değere göre, adreslerin konsoldan veya klavyeden alınacağını kontrol edip ona göre hareket eden bir switch/case.
	switch (secim)
	{
	case 1:
		//Kullanıcıdan eşik değerleri alınıyor(Sayaç ve Buffer için).
		printf("Sayacın eşik değerini giriniz : ");
		scanf("%d", &t);
		printf("Buffer uzunluğu : ");
		scanf("%d", &l);
		while (1)
		{
			// Kullanıcı "exit" yazana kadar bir adres almaya devam eder, aldığı adresi root(head) değeri ile beraber fonksiyona gönderir.
			printf("Adres giriniz(Çıkmak için 'exit' yazınız) : ");
			scanf("%s", a);
			if (!strcmp(a, "exit"))
			{
				break;
			}
			else
			{
				// Fonksiyon structure tipinde bir geri dönüşe sahip. Değişen, eklenen, silinen değerlere göre root(head) değişiyor ++
				// Bu değişim sonucunda yeni oluşan root(head) return ediliyor. Dolayısıyla root = fonksiyon olarak kullandım.
				root = doEverything(root, a, t, l);
				yazdir(root);
				printf("\n");
			}
		}
		break;
	case 2:
		printf("Sayacın eşik değerini giriniz : ");
		scanf("%d", &t);
		printf("Buffer uzunluğu : ");
		scanf("%d", &l);
		// input.txt de yazan değerleri satır satır okuyup, fonksiyona yolladım.
		abc = fopen("input.txt","r");
		while (fscanf(abc, "%s" ,emacs)!=EOF)	
		{
				strcpy(a,emacs);
				root = doEverything(root, a, t, l);
				yazdir(root);
				printf("\n");
		}
		fclose(abc);
		
	default:
		break;
	}
		// Program bitişi...
		// Cache Buffer'ın son halini ekrana yazdırır, kullanıcıya temizlemek isteyip istemediğini sorar ++
		// Eğer isteniyorsa "delete" fonksiyonu çalıştırılır.
		// Temizlenip temizlenmediğini görmek için, her işlemden önce print yaptırılır(delete fonksiyonunda).
		printf("\n-------------------------------------------");
		printf("\n - - - - Cache Buffer - - - -\n\n");
		yazdir(root);
		printf("\n\n - - - - Cache Buffer - - - -");
		printf("\n-------------------------------------------");
		printf("\n Cache buffer temizlensin mi? (Y/N): ");
		scanf("%s", b);
		if (!strcmp(b, "Y"))
		{
			delete (root);
			printf("\n-------------------------------------------\n");
			printf("Cache Buffer temizlenmiştir.");
			printf("\n-------------------------------------------\n");
		}
	return 0;
}
