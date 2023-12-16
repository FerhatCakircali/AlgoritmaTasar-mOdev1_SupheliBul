#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

const int kisiFirmaSayisi = 51; // Kiþiler ve Firmalarýn sayýsýnýn "bir" fazlasý.
const int maxAdBoyutu = 50;     // Kiþiler ve Firmalarýn isimlerinin max uzunluðu

int siralanacakDizi[kisiFirmaSayisi][2]; //toplamTransfer miktarý bu dizide tutulacak ve bu dizi üzerinde sýralama yapýlacak.

struct KisilerFirmalar 
{ 
    int ID;
    char ad[maxAdBoyutu];
    double maxKapasite;
    double vergiBeyani;
};

 //kisilervefirmalar.txt ve transfer.txt dosyasý okunur, okunan veriler diziye atanýr
void dosyaOku(KisilerFirmalar kisiler[], int transferMatris[][kisiFirmaSayisi]) {
	
    ifstream kisilerDosya("kisilervefirmalar.txt");
    if (!kisilerDosya.is_open()) 
	{
        cout << "kisilervefirmalar.txt dosyasi acilamadi." << endl;
        return;
    }

	string satir;
	int index = 0;
	
	while (getline(kisilerDosya, satir) && index < kisiFirmaSayisi)  // "kisilervefirmalar.txt" dosyasýndan satýr satýr okuma iþlemi
	{	  
	    if (index != 0) // Ýlk satýr baþlýk olduðu için onu atlýyoruz
		{  
	        
	        istringstream iss(satir);// "iss" adýnda bir istringstream nesnesi oluþturarak satýrý parçalýyoruz
	
	        iss >> kisiler[index].ID >> ws;  // Kiþilerin ID'sini okuyoruz ve boþluk karakterlerini atlýyoruz
	
	        // Kiþilerin adýný belirtilen maksimum boyutta okuyoruz ve '\t' karakterine kadar olan kýsmý alýyoruz
	        iss.getline(kisiler[index].ad, maxAdBoyutu, '\t');
	
	        // Kiþilerin maksimum kapasitesini ve vergi beyanýný okuyoruz
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
    while (getline(transferDosya, satir) && satirIndex < kisiFirmaSayisi)  // "transfer.txt" dosyasýndan satýr satýr okuma iþlemi
	{ 
	    
	    if (satirIndex != 0) // Ýlk satýrý yine atlýyoruz
		{ 
	        istringstream iss(satir); // satýrý parçalýyoruz
	
	        // Her bir transfer matrisi giriþini sýrayla okuyarak ilgili hücrelere atama yapýyoruz
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

    ofstream kosulsuzDosya("SupheliOlmayanVeKosullariSaglamayan.txt"); // Þüpheli deðil veya Hiçbir koþula uymayanlarýn olduðu listenin dosyasýsýný olusturur
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
            kosulsuzDosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 1 (Supheli Degil)  " << endl; // Þüpheli deðil veya Hiçbir koþula uymayanlarýn olduðu dosyaya yazdýrýr
        } 
		else if ((kisiler[i].vergiBeyani * 1000) > (2 * kisiler[i].maxKapasite * 1000)) 
		{
            dosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 2 (Supheli)" << endl; //SupheliListesi dosyasýna yazdýrýr
        }
		else if ((kisiler[i].vergiBeyani * 1000) != toplamTransfer) 
		{
            dosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: 3 (Supheli)" << endl;  //SupheliListesi dosyasýna yazdýrýr
        } 
		else 
        {
            kosulsuzDosya << kisiler[i].ID << " " << kisiler[i].ad << "\t| Durum: (Belirsiz) " << endl; // Þüpheli deðil veya Hiçbir koþula uymayanlarýn olduðu dosyaya yazdýrýr
        }
    }
		cout << "SupheliListesi.txt dosyasi olusturuldu. \n" << endl;
    dosya.close();
}

void toplamTransfer(KisilerFirmalar kisiler[], int transferMatris[][kisiFirmaSayisi]) { //  Bir kiþiye veya firmaya transfer edilen toplam para miktarý hesaplanýr
    
	for (int i = 1; i < kisiFirmaSayisi; i++) 
	{
	    int toplam = 0;
	
	    // Her bir firmadan gelen transfer miktarýný toplar
	    for (int j = 1; j < kisiFirmaSayisi; j++) 
		{
	        toplam += transferMatris[j][i];
	    }
	
	    // Hesaplanan toplam transfer miktarýný, siralanacakDizi'sine atar
	    siralanacakDizi[i][0] = kisiler[i].ID;   // Kiþi/Firma ID'si
	    siralanacakDizi[i][1] = toplam;           //ID'ye göre Hesaplanan toplam transfer miktarý
	}

}

void transferListele() { //Yapýlan toplam transferlerin bir listesi (en yüksekten en düþüðe) listeler
	
    // Selection sort ile sýralama
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

