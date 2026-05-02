#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <set>

namespace pr
{
    class soru;
    
    enum secenekler 
    { 
        X = 0, 
        A = 1, 
        B = 2, 
        C = 3, 
        D = 4, 
        E = 5 
    };
    
    enum konular 
    { 
        DIGER = 0, 
        SAYI_KUMELERI = 1, 
        TEMEL_ISLEMLER = 2, 
        TEK_CIFT_SAYILAR = 3, 
        POZ_NEG_SAYILAR = 4, 
        ARDISIK_SAYILAR = 5, 
        SAYI_BASAMAKLARI = 6, 
        ASAL_AA_SAYILAR = 7, 
        RASYONEL_SAYILAR = 8, 
        ONDALIK_SAYILAR = 9, 
        KALANLI_BOLME = 10,
        BOLUNEBILME = 11, 
        ASAL_CARP = 12 
    };

    enum celdiriciler 
    { 
        DOGRU = 0, 
        ISLEM_HATASI = 1, 
        BILGI_HATALI = 2, 
        BILGI_EKSIK = 3, 
        DIKKAT_HATASI = 4 
    };

    inline std::set<pr::soru> sorular;

    class konu_havuzlari
    {
        std::map<int, std::vector<pr::konular>> havuzlar;
        int havuz_sayisi;

        public:
        konu_havuzlari(int havuz_sayisi_ = 5) : havuz_sayisi(havuz_sayisi_)
        {
            for (int i = 1; i <= havuz_sayisi; ++i) 
            {
                havuzlar[i] = {};
            }
        }

        auto konu_ekle(const pr::konular konu, const int havuz_derecesi) -> bool
        {
            if (havuz_derecesi > havuz_sayisi || havuz_derecesi < 1) return false;
            if (konu_bul(konu).first != 0) return false;

            havuzlar[havuz_derecesi].push_back(konu);
            return true;
        }

        auto konu_cikar(const pr::konular konu) -> bool
        {
            auto yer = konu_bul(konu);
            if (yer.first == 0) return false;

            auto& vec = havuzlar[yer.first];
            vec.erase(std::remove(vec.begin(), vec.end(), konu), vec.end());
            return true;
        }

        auto konu_bul(const pr::konular konu) -> std::pair<int, int>
        {
            for (auto& [no, liste] : havuzlar) 
            {
                auto it = std::find(liste.begin(), liste.end(), konu);
                if (it != liste.end()) 
                {
                    return {no, (int)std::distance(liste.begin(), it)};
                }
            }
            return {0, -1};
        }

        auto konu_kaydir(const pr::konular konu, const bool yukari) -> bool
        {
            auto yer = konu_bul(konu);
            if (yer.first == 0) return false;

            int mevcut_havuz = yer.first;
            int hedef_havuz = yukari ? mevcut_havuz + 1 : mevcut_havuz - 1;

            if (hedef_havuz < 1 || hedef_havuz > havuz_sayisi) return false;

            konu_cikar(konu);
            havuzlar[hedef_havuz].push_back(konu);
            return true;
        }

        auto rastgele_havuz() -> std::vector<pr::konular>
        {
            int rastgele = rand() % 101;
            if (rastgele < 51) return havuzlar[1];
            if (rastgele < 76) return havuzlar[2];
            if (rastgele < 88) return havuzlar[3];
            if (rastgele < 96) return havuzlar[4];
            return havuzlar[5];
        }
    };

    class soru
    {
        pr::secenekler dogru_cevap;
        std::string soru_konumu;
        std::map<pr::secenekler, pr::celdiriciler> celdiriciler_map;
        pr::konular konu;

        public:
        soru(std::string soru_konumu_, pr::secenekler dogru_cevap_, pr::celdiriciler celdA, pr::celdiriciler celdB, pr::celdiriciler celdC, pr::celdiriciler celdD, pr::celdiriciler celdE, pr::konular konu_, konu_havuzlari& havuz_sistemi) 
            : soru_konumu(soru_konumu_), dogru_cevap(dogru_cevap_), konu(konu_) 
            {
                celdiriciler_map = {{pr::A, celdA}, {pr::B, celdB}, {pr::C, celdC}, {pr::D, celdD}, {pr::E, celdE}};
                havuz_sistemi.konu_ekle(konu_, 1);
            }

        auto konuyu_goster() const -> pr::konular 
        { 
            return konu; 
        }
        auto dogru_cevabi_goster() const -> pr::secenekler
        {
            return dogru_cevap;
        }
        auto soru_konumunu_goster() const -> std::string
        {
            return soru_konumu;
        }
        auto celdiricileri_goster() const -> std::map<pr::secenekler, pr::celdiriciler>
        {
            return celdiriciler_map;
        }
        bool operator<(const soru& diger) const
        {
            return this -> soru_konumu < diger.soru_konumu;
        }
    };

    void sorulari_dosyaya_yaz(const std::vector<pr::soru>& sorular_vec)
    {
        auto sorular_dosyasi = std::fstream{"sorular.txt", std::ios::out | std::ios::app};
        for (auto sor : sorular_vec)
        {
            sorular_dosyasi << "SORU: " << sor.soru_konumunu_goster() << " " << sor.dogru_cevabi_goster() << " ";
            auto celdiriciler_kopya = sor.celdiricileri_goster();
            for (auto [sec, celd] : celdiriciler_kopya)
            {
                sorular_dosyasi << celd << " ";
            }
            sorular_dosyasi << sor.konuyu_goster() << std::endl;
        }  
        sorular_dosyasi.close();  
    }

    auto sorulari_dosyadan_oku(pr::konu_havuzlari& havuz_sistemi) -> std::set<pr::soru>
    {
        std::set<pr::soru> okunan_sorular;
        auto sorular_dosyasi = std::fstream{"sorular.txt", std::ios::in}; 
        if (!sorular_dosyasi.is_open()) return okunan_sorular;

        std::string etiket, konum;
        int d_cevap, celdA, celdB, celdC, celdD, celdE, konu;

        while (sorular_dosyasi >> etiket >> konum >> d_cevap >> celdA >> celdB >> celdC >> celdD >> celdE >> konu)
        {
            pr::soru yeni_soru
            (
                konum, 
                static_cast<pr::secenekler>(d_cevap),
                static_cast<pr::celdiriciler>(celdA),
                static_cast<pr::celdiriciler>(celdB),
                static_cast<pr::celdiriciler>(celdC),
                static_cast<pr::celdiriciler>(celdD),
                static_cast<pr::celdiriciler>(celdE),
                static_cast<pr::konular>(konu),
                havuz_sistemi
            );
            okunan_sorular.insert(yeni_soru);
        }
        return okunan_sorular;
    }

    void sorulari_sifirla()
    {
        auto sorular_dosyasi = std::fstream{"sorular.txt", std::ios::out | std::ios::trunc};
        sorular_dosyasi.close();
    }

    void konulari_dosyaya_yaz(const std::map<int, std::vector<pr::konular>>& konular_map)
    {
        auto konular_dosyasi = std::fstream{"konular.txt", std::ios::out | std::ios::trunc};
        for (const auto& [havuz, konular_vec] : konular_map)
        {
            for (auto k : konular_vec)
            {
                konular_dosyasi << "KONU: " << havuz << " " << static_cast<int>(k) << std::endl;
            }
        }
    }

    auto konulari_dosyadan_oku() -> std::map<int, std::vector<pr::konular>>
    {
        std::map<int, std::vector<pr::konular>> okunan_konular = {{1, {}}, {2, {}}, {3, {}}, {4, {}}, {5, {}}};
        auto konular_dosyasi = std::fstream{"konular.txt", std::ios::in};
        if (!konular_dosyasi.is_open()) return okunan_konular;

        int konu_havuzu, konu_no;
        std::string etiket;

        while (konular_dosyasi >> etiket >> konu_havuzu >> konu_no)
        {
            if(okunan_konular.count(konu_havuzu)) 
            {
                okunan_konular[konu_havuzu].push_back(static_cast<pr::konular>(konu_no));
            }
        }
        return okunan_konular;
    }

    void konulari_sifirla()
    {
        auto konular_dosyasi = std::fstream{"konular.txt", std::ios::out | std::ios::trunc};
        konular_dosyasi.close();
    }
}
