#include <iostream>
#include <fstream>
#include <cstdlib>

#include "yapilar.hpp"

int main()
{
    using namespace pr;
    std::srand(time(0));
    std::cout << "ADAPTIF MATEMATIK OGRENME UYGULAMASINA HOS GELDINIZ!" << std::endl;
    std::cout << "Kullanim ipuclari icin readme.md dosyasini inceleyebilirsiniz." << std::endl;
    auto okuma = maoh_okuyucu();
    if (!okuma.maoh_okuma_basarisi())
    {
        std::cout << "Uygulamayi ilk kez kullandiginiz tespit edildi. Baslangic icin bazi ayarlari yapmalisiniz." << std::endl;
        std::cout << "Havuz sayisi giriniz (Varsayilan 5, minimum 1, maksimum 6 adettir.): ";
        int havuz_sayisi;
        std::cin >> havuz_sayisi;
        okuma.havuz_sayisi_tanimla(havuz_sayisi);
        okuma.dosyaya_yaz();
        std::cout << "Dosya olusturuldu." << std::endl;
    }
}
