#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>

#include "yapilar.hpp"

int main()
{
    using namespace pr;
    std::srand(time(0));
    std::cout << "ADAPTIF MATEMATIK OGRENME UYGULAMASINA HOS GELDINIZ!" << std::endl;
    std::cout << "Proje hakkinda detaylar icin proje GitHub sayfasina bakabilirsiniz: ta2ehi/2026-4006" << std::endl;
    auto okuma = maoh_okuyucu();
    if (!okuma.maoh_okuma_basarisi()) // Baslangic ayarlari
    {
        std::cout << "Uygulamayi ilk kez kullandiginiz tespit edildi. Baslangic icin bazi ayarlari yapmalisiniz." << std::endl;
        std::cout << "Havuz sayisi giriniz (Varsayilan 5, minimum 1, maksimum 6 adettir.): ";
        int havuz_sayisi;
        std::cin >> havuz_sayisi;
        okuma.havuz_sayisi_tanimla(havuz_sayisi);
        okuma.dosyaya_yaz();
        std::cout << "Dosya olusturuldu." << std::endl;
    }
    int secim;
    std::cout << "Ayarlari degistirmek icin 0, uygulamaya giris icin 1 giriniz: ";
    std::cin >> secim;
    if (secim == 0)
    {
        std::cout << "Havuz sayisi giriniz (Varsayilan 5, minimum 1, maksimum 6 adettir.): ";
        int havuz_sayisi;
        std::cin >> havuz_sayisi;
        okuma.havuz_sayisi_tanimla(havuz_sayisi);
        okuma.dosyaya_yaz();
    }

    // ASIL PROGRAM
    if (okuma.soru_sayisini_goster() < 5)
    {
        std::cerr << "HATA 1: Soru sayisi cok dusuk. Lutfen havuzlar.maoh dasyasinda en az 5 sorunun bulundugundan emin olunuz." << std::endl;
        return 1;
    }

    std::cout << "Rastgele soru uretimi basladi. Sorular olusturuluyor . . ." << std::endl;
    auto sorular = rastgele_soru(5, okuma);
    std::cout << "Agirlikli rastgelelik sonucu belirlenen konu: " << sorular.first.konu_ismini_goster() << std::endl;
    int i = 0;
    secenekler sec;
    std::map<int, secenekler> verilen_cevaplar;
    int toplam_dogru_sayisi = 0;
    for (auto [soru_sira_no, secilen_soru] : sorular.second)
    {
        std::cout << ++i << ". SORU KONUMU: " << secilen_soru.soru_konumunu_goster() << std::endl;
        char c;
        std::cout << "Cevap giriniz (Secenekler icin A-E harflerini, bos birakmak icin X harfini kullaniniz.): ";
        std::cin >> c;
        sec = static_cast<secenekler>(c);
        bool dogru_mu = secilen_soru.cevap_dogru_mu(sec);
        if (dogru_mu)
        {
            std::cout << "Tebrikler! Soruya dogru cevap verdiniz." << std::endl;
            ++toplam_dogru_sayisi;
            continue;
        }
        else if (sec != X)
        {
            std::cout << "Yanlis cevap. Dogru cevap " << static_cast<secenekler> (secilen_soru.dogru_cevabi_goster()) << " olmaliydi." << std::endl;
            std::cout << "Soruyu yanlis yapmanizin muhtemel nedeni: " << static_cast<celdiriciler> (secilen_soru.celdirici(sec)) << std::endl;
            continue;
        }
        else
        {
            std::cout << "Soruyu bos biraktiniz." << std::endl;
            continue;
        }
    }
    std::cout << "Size yoneltilen 5 sorudan " << toplam_dogru_sayisi << " soruyu dogru cevapladiniz. ";
    if (toplam_dogru_sayisi <= 3)
    {
        std::cout << "Bu sonuc %60'in altinda bir basariya tekabul ettiginden basarisiz sayildiniz. Konu puaniniz dusuruldu, konu varsa bir alt havuza kaydirildi." << std::endl;
        okuma.konuyu_kaydir(sorular.first, false);
        if (!okuma.dosyaya_yaz()) 
        {
            std::cerr << "HATA 1: Yanlis soru sonucu dosya yazdirma basarisiz oldu." << std::endl;
            return 1;
        }
        
    }

}
