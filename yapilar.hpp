#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>

namespace pr
{
    class soru;

    auto havuz_orani_belirle(const int& havuz_sayisi) -> std::map<int, double>;
    auto oranlari_dosyaya_yaz(const std::map<int, double>& oranlar, std::ofstream& dosya) -> bool;

    enum celdiriciler 
    { 
        DOGRU = 0, 
        ISLEM_HATASI = 1, 
        BILGI_HATALI = 2, 
        BILGI_EKSIK = 3, 
        DIKKAT_HATASI = 4 
    };

    enum secenekler 
    { 
        X = 0, 
        A = 1, 
        B = 2, 
        C = 3, 
        D = 4, 
        E = 5 
    };

    class soru
    {
        std::string soru_konumu;
        pr::secenekler dogru_cevap;
        std::map<pr::secenekler, pr::celdiriciler> celdiriciler_map;
        int zorluk;

        public:
        soru(const std::string& soru_konumu_, const pr::secenekler& dogru_cevap_, const std::map<pr::secenekler, pr::celdiriciler>& celdiriciler_map_, const int& zorluk_)
            : soru_konumu(soru_konumu_), dogru_cevap(dogru_cevap_), celdiriciler_map(celdiriciler_map_), zorluk(zorluk_) {}

        bool operator<(const soru& diger) const
        {
            return soru_konumu < diger.soru_konumu;
        }

        auto cevap_dogru_mu(const pr::secenekler& cvp) -> bool
        {
            return cvp == dogru_cevap;
        }

        auto celdirici(const pr::secenekler& cvp) -> pr::celdiriciler
        {
            return celdiriciler_map.at(cvp);
        }

        void dosyaya_yazdir(std::ofstream& dosya)
        {
            dosya << "SORU: " << soru_konumu << " " << dogru_cevap << " ";
            for (auto [secenek, celd] : celdiriciler_map)
            {
                dosya << celd << " ";
            }
            dosya << zorluk << std::endl;
        }
    };

    class konu
    {
        std::string konu_ismi;
        std::set<pr::soru> sorular;

        public:
        konu(const std::string& konu_ismi_) : konu_ismi(konu_ismi_) {}
        konu(const std::string& konu_ismi_, const std::set<pr::soru>& sorular_) : konu_ismi(konu_ismi_), sorular(sorular_) {}

        void soru_ekle(const pr::soru& s)
        {
            sorular.insert(s);
        }

        void soru_cikar(const pr::soru& s)
        {
            if (!sorular.contains(s)) return;
            sorular.erase(s);
        }

        void dosyaya_yazdir(std::ofstream& dosya) const
        {
            dosya << "KONU: " << konu_ismi << std::endl;
            for (auto soru : sorular)
            {
                soru.dosyaya_yazdir(dosya);
            }
        }

        bool operator< (const pr::konu& diger) const
        {
            return this->konu_ismi < diger.konu_ismi;
        }

        bool operator== (const pr::konu& diger) const
        {
            return this -> konu_ismi == diger.konu_ismi;
        }
    };

    using havuz_tipi = std::set<pr::konu>;

    class havuzlar
    {
        std::map<int, havuz_tipi> havuz_map;
        int havuz_sayisi;
        std::map<int, double> oranlar;
        bool okuma = true;

        public:
        havuzlar(const int& havuz_sayisi_) : havuz_sayisi(havuz_sayisi_)
        {
            for (int i = 0; i < havuz_sayisi; ++i)
            {
                havuz_map.insert({i + 1, {}});
            }
        }

        havuzlar(const bool& okuma_) : okuma(okuma_) {}

        void havuz_sayisi_tanimla(const int& havuz_say)
        {
            havuz_sayisi = havuz_say;
            oranlar = havuz_orani_belirle(havuz_say);
        }

        void konu_ekle(const pr::konu& k, const int& h)
        {
            if (havuz_map.at(h).contains(k)) return;
            havuz_map.at(h).insert(k);
        }

        void konu_cikar(const pr::konu& k)
        {
            for (auto& [havuz_no, konular] : havuz_map)
            {
                if (konular.contains(k))
                {
                    konular.erase(k);
                    return;
                }
            }
        }

        auto konu_hangi_havuzda(const pr::konu& k) -> int
        {
            for (auto& [havuz_no, konular] : havuz_map)
            {
                if (konular.contains(k)) return havuz_no;
            }
            return 0;
        }

        void konuyu_kaydir(const pr::konu& k, const bool& yukari_mi)
        {
            int bulunan_havuz = -1;

            for (auto& [havuz_no, konular] : havuz_map)
            {
                if (konular.contains(k))
                {
                    konular.erase(k);
                    bulunan_havuz = havuz_no;
                    break;
                }
            }

            if ((yukari_mi && bulunan_havuz == havuz_sayisi) || (!yukari_mi && bulunan_havuz == 1) || bulunan_havuz == -1) return;

            havuz_map.at(yukari_mi ? bulunan_havuz + 1 : bulunan_havuz - 1).insert(k);
        }

        auto dosyaya_yaz() -> bool
        {
            auto dosya = std::ofstream{"havuzlar.maoh", std::ios::out | std::ios::trunc};
            if (!dosya.is_open()) return false;

            dosya << "MAOH.START-OF-FILE:3.1" << std::endl;
            dosya << "HAVUZ-SAYISI: " << havuz_sayisi << std::endl;
            dosya << "HAVUZ-ORANLARI: ";
            oranlari_dosyaya_yaz(oranlar, dosya);

            for (auto& [havuz_no, konular] : havuz_map)
            {
                dosya << "HAVUZ: " << havuz_no << std::endl;
                for (auto& konu : konular)
                {
                    konu.dosyaya_yazdir(dosya);
                }
            }

            dosya << "MAOH.END-OF-FILE:" << std::endl;
            return true;
        }

        bool operator== (const havuzlar& diger) const
        {
            return this -> havuz_map == diger.havuz_map;
        }

        auto maoh_okuma_basarisi() -> bool
        {
            return okuma;
        }
    };

    inline auto maoh_okuyucu() -> pr::havuzlar
    {
        auto dosya = std::ifstream{"havuzlar.maoh", std::ios::in};
        if (!dosya.is_open()) return pr::havuzlar(false);

        std::string token;

        int havuz_sayisi = 1;
        std::map<int, havuz_tipi> temp_map;

        int aktif_havuz = -1;
        std::string aktif_konu_ismi;

        std::map<int, double> oranlar_map;

        while (dosya >> token)
        {
            if (token == "MAOH.START-OF-FILE:3.0") continue;
            if (token == "MAOH.END-OF-FILE:") break;

            else if (token == "HAVUZ-SAYISI:")
            {
                dosya >> havuz_sayisi;
            }

            else if (token == "HAVUZ-ORANLARI:") 
            {
                for (int i = 0; i < havuz_sayisi; ++i)
                {
                    dosya >> oranlar_map[i];
                }
            }

            else if (token == "HAVUZ:")
            {
                dosya >> aktif_havuz;
                temp_map[aktif_havuz] = {};
            }

            else if (token == "KONU:")
            {
                dosya >> aktif_konu_ismi;
                temp_map[aktif_havuz].insert(pr::konu(aktif_konu_ismi));
            }

            else if (token == "SORU:")
            {
                std::string soru_konumu;
                int dogru, cA, cB, cC, cD, cE, zorluk;

                dosya >> soru_konumu >> dogru >> cA >> cB >> cC >> cD >> cE >> zorluk;

                pr::soru s(
                    soru_konumu,
                    static_cast<pr::secenekler>(dogru),
                    {
                        {pr::A, static_cast<pr::celdiriciler>(cA)},
                        {pr::B, static_cast<pr::celdiriciler>(cB)},
                        {pr::C, static_cast<pr::celdiriciler>(cC)},
                        {pr::D, static_cast<pr::celdiriciler>(cD)},
                        {pr::E, static_cast<pr::celdiriciler>(cE)},
                    },
                    zorluk
                );

                auto it = temp_map[aktif_havuz].find(pr::konu(aktif_konu_ismi));
                if (it != temp_map[aktif_havuz].end())
                {
                    pr::konu kopya = *it;
                    temp_map[aktif_havuz].erase(it);
                    kopya.soru_ekle(s);
                    temp_map[aktif_havuz].insert(kopya);
                }
            }
        }

        pr::havuzlar sonuc(havuz_sayisi);

        for (auto& [no, konular] : temp_map)
        {
            for (auto& k : konular)
            {
                sonuc.konu_ekle(k, no);
            }
        }

        return sonuc;
    }

    auto havuz_orani_belirle(const int& havuz_sayisi) -> std::map<int, double>
    {
        if (havuz_sayisi < 1 || havuz_sayisi > 6) return {};
        switch (havuz_sayisi)
        {
            case 1:
                return {{1, 1}};
            case 2:
                return {{1, 0.75}, {2, 0.25}};
            case 3:
                return {{1, 0.6}, {2, 0.25}, {3, 0.15}};
            case 4:
                return {{1, 0.5}, {2, 0.25}, {3, 0.16}, {4, 0.9}};
            case 5:
                return {{1, 0.5}, {2, 0.25}, {3, 0.12}, {4, 0.08}, {5, 0.05}};
            case 6:
                return {{1, 0.5}, {2, 0.25}, {3, 0.1}, {4, 0.07}, {5, 0.05}, {6, 0.03}};
        };
        return {};
    }

    auto oranlari_dosyaya_yaz(const std::map<int, double>& oranlar, std::ofstream& dosya) -> bool
    {
        if (!dosya.is_open()) return false;
        for (auto [havuz_no, oran] : oranlar)
        {
            dosya << oran << " ";
        }
        dosya << std::endl;
        return true;
    }

}
