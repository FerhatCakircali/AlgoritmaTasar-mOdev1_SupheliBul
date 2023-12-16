#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int kisiFirmaSayisi = 51; // Ki�iler ve Firmalar�n say�s�n�n "bir" fazlas�.
const int maxAdBoyutu = 50;     // Ki�iler ve Firmalar�n isimlerinin max uzunlu�u

int siralanacakDizi[kisiFirmaSayisi][2]; //toplamTransfer miktar� bu dizide tutulacak ve bu dizi �zerinde s�ralama yap�lacak.

struct KisilerFirmalar 
{ 
    int ID;
    char ad[maxAdBoyutu];
    double maxKapasite;
    double vergiBeyani;
};

 //kisilervefirmalar.txt ve transfer.txt dosyas� okunur, okunan veriler diziye atan�r
void dosyaOku(KisilerFirmalar kisiler[], int transferMatris[][kisiFirmaSayisi]) {
	
    ifstream kisilerDosya("kisilervefirmalar.txt");
    if (!kisilerDosya.is_open()) 
	{
        cout << "kisilervefirmalar.txt dosyasi acilamadi." << endl;
        return;
    }

	string satir;
	int index = 0;
	
	while (getline(kisilerDosya, satir) && index < kisiFirmaSayisi)  // "kisilervefirmalar.txt" dosyas�ndan sat�r sat�r okuma i�lemi
	{	  
	    if (index != 0) // �lk sat�r ba�l�k oldu�u i�in onu atl�yoruz
		{  
	        
	        istringstream iss(satir);// "iss" ad�nda bir istringstream nesnesi olu�turarak sat�r� par�al�yoruz
	
	        iss >> kisiler[index].ID >> ws;  // Ki�ilerin ID'sini okuyoruz ve bo�luk karakterlerini atl�yoruz
	
	        // Ki�ilerin ad�n� belirtilen maksimum boyutta okuyoruz ve '\t' karakterine kadar olan k�sm� al�yoruz
	        iss.getline(kisiler[index].ad, maxAdBoyutu, '\t');
	
	        // Ki�ilerin maksimum kapasitesini ve vergi beyan�n� okuyoruz
	        iss >> kisiler[index].maxKapasite >> kisiler[index].vergiBeyani;
	    }
	    index++;
	}
    kisilerDosya.close();

    ifstream transferDosya("transfer.txt");
    if (!transferDosya.is_open()) 
	{
        cout << "transfer.txt dosyasi acilamadi." << endl;
        return;
    }

    int satirIndex = 0;
    while (getline(transferDosya, satir) && satirIndex < kisiFirmaSayisi)  // "transfer.txt" dosyas�ndan sat�r sat�r okuma i�lemi
	{ 
	    
	    if (satirIndex != 0) // �lk sat�r� yine atl�yoruz
		{ 
	        istringstream iss(satir); // sat�r� par�al�yoruz
	
	        // Her bir transfer matrisi giri�ini s�rayla okuyarak ilgili h�crelere atama yap�yoruz
	        for (int i = 0; i < kisiFirmaSayisi; i++) 
			{
	            iss >> transferMatris[satirIndex][i];
	        }
	    }
	    satirIndex++;
	}
    transferDosya.close();
}

void supheliBul(KisilerFirmalar kisiler[], int transferMatris[][kisiFirmaSayisi]) {
	
    ofstream dosya("SupheliListesi.txt");
    if (!dosya.is_open()) 
	{
        cout << "SupheliListesi.txt dosyasi acilamadi." << endl;
        return;
    }

    ofstream kosulsuzDosya("SupheliOlmayanVeKosullariSaglamayan.txt"); // ��pheli de�il veya Hi�bir ko�ula uymayanlar�n oldu�u listenin dosyas�s�n� olusturur
    if (!kosulsuzDosya.is_open()) 
	{
        cout << "SupheliOlmayanVeKosullariSaglamayan.txt dosyasi acilamadi." << endl;
        return;
    }

    for (int i = 1; i < kisiFirmaSayisi; i++) 
	{
        int toplamTransfer = 0;
        for (int j = 1; j < kisiFirmaSayisi; j++) 
		{
            toplamTransfer += transferMatris[j][i];
        }

        if (((kisiler[i].vergiBeyani * 1000) <= kisiler[i].maxKapasite * 1000) && ((kisiler[i].vergiBeyani * 1000) == toplamTransfer)) 
		{
            kosulsuzDosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 1 (Supheli Degil)  " << endl; // ��pheli de�il veya Hi�bir ko�ula uymayanlar�n oldu�u dosyaya yazd�r�r
        } 
		else if ((kisiler[i].vergiBeyani * 1000) > (2 * kisiler[i].maxKapasite * 1000)) 
		{
            dosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 2 (Supheli)" << endl; //SupheliListesi dosyas�na yazd�r�r
        }
		else if ((kisiler[i].vergiBeyani * 1000) != toplamTransfer) 
		{
            dosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 3 (Supheli)" << endl;  //SupheliListesi dosyas�na yazd�r�r
        } 
		else 
        {
            kosulsuzDosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: (Belirsiz) " << endl; // ��pheli de�il veya Hi�bir ko�ula uymayanlar�n oldu�u dosyaya yazd�r�r
        }
    }
		cout << "SupheliListesi.txt dosyasi olusturuldu. \n" << endl;
    dosya.close();
}

void toplamTransfer(KisilerFirmalar kisiler[], int transferMatris[][kisiFirmaSayisi]) { //  Bir ki�iye veya firmaya transfer edilen toplam para miktar� hesaplan�r
    
	for (int i = 1; i < kisiFirmaSayisi; i++) 
	{
	    int toplam = 0;
	
	    // Her bir firmadan gelen transfer miktar�n� toplar
	    for (int j = 1; j < kisiFirmaSayisi; j++) 
		{
	        toplam += transferMatris[j][i];
	    }
	
	    // Hesaplanan toplam transfer miktar�n�, siralanacakDizi'sine atar
	    siralanacakDizi[i][0] = kisiler[i].ID;   // Ki�i/Firma ID'si
	    siralanacakDizi[i][1] = toplam;           //ID'ye g�re Hesaplanan toplam transfer miktar�
	}

}

void transferListele() { //Yap�lan toplam transferlerin bir listesi (en y�ksekten en d����e) listeler
	
    // Selection sort ile s�ralama
    for (int i = 1; i < kisiFirmaSayisi - 1; i++) 
	{
        int maxIndex = i;
        
        for (int j = i + 1; j < kisiFirmaSayisi; j++)
		 {
            if (siralanacakDizi[j][1] > siralanacakDizi[maxIndex][1]) 
			{
                int tempID = siralanacakDizi[i][0];
                int tempTransfer = siralanacakDizi[i][1];

                siralanacakDizi[i][0] = siralanacakDizi[j][0];
                siralanacakDizi[i][1] = siralanacakDizi[j][1];

                siralanacakDizi[j][0] = tempID;
                siralanacakDizi[j][1] = tempTransfer;
            }
        }
    }

    cout << "Kisi/Firma ID \t\t" << " Toplam Transfer Miktari"
         << endl;

    for (int j = 1; j < kisiFirmaSayisi; j++) 
	{
        cout << "    "<< siralanacakDizi[j][0] << "\t -------> \t" << siralanacakDizi[j][1] << endl;
    }
}

int main() {
    KisilerFirmalar kisiler[kisiFirmaSayisi];
    int transferMatris[kisiFirmaSayisi][kisiFirmaSayisi] = {0};

    dosyaOku(kisiler, transferMatris);
    toplamTransfer(kisiler, transferMatris);
    supheliBul(kisiler, transferMatris);
    transferListele();

    return 0;
}

