#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <cmath>
#include <string>
#include <cctype>

namespace pr
{
    class soru;

    auto havuz_orani_belirle(const int& havuz_sayisi) -> std::map<int, double>;
    auto oranlari_dosyaya_yaz(const std::map<int, double>& oranlar, std::ofstream& dosya) -> bool;

    enum celdiriciler { DOGRU = 0, ISLEM_HATASI = 1, BILGI_HATALI = 2, BILGI_EKSIK = 3, DIKKAT_HATASI = 4, DIGER = 5, ISARET_HATASI = 6, KAVRAM_YANILGISI = 7, FORMUL_HATASI = 8 };
    enum secenekler { X = 0, A = 1, B = 2, C = 3, D = 4, E = 5 };

    inline pr::secenekler char_to_secenek(char c)
    {
        c = std::toupper(c);
        if (c >= 'A' && c <= 'E') return static_cast<pr::secenekler>(c - 'A' + 1);
        return pr::X;
    }

    inline std::string secenek_to_string(pr::secenekler s)
    {
        if (s >= pr::A && s <= pr::E) {
            std::string res = "";
            res += static_cast<char>('A' + s - 1);
            return res;
        }
        return "X";
    }

    inline std::string celdirici_to_string(pr::celdiriciler c)
    {
        switch (c) {
            case DOGRU: return "Doğru";
            case ISLEM_HATASI: return "İşlem Hatası (Toplama, çıkarma, çarpma vb. basit işlem hatası)";
            case BILGI_HATALI: return "Bilgi Hatası (Yanlış formül veya kural kullanımı)";
            case BILGI_EKSIK: return "Bilgi Eksikliği (Soruyu yarım bırakma veya eksik yorumlama)";
            case DIKKAT_HATASI: return "Dikkat Hatası (Soruyu veya isteneni yanlış okuma)";
            case DIGER: return "Diğer (Güçlü bir çeldirici değil)";
            case ISARET_HATASI: return "İşaret Hatası (+ yerine - bulma vb.)";
            case KAVRAM_YANILGISI: return "Kavram Yanılgısı (Temel matematiksel kavramları karıştırma)";
            case FORMUL_HATASI: return "Formül Hatası (Formülü yanlış hatırlama veya uygulama)";
            default: return "Bilinmeyen Hata";
        }
    }

    class soru
    {
        int yanlis_yapilma_sayisi = 0;
        std::string soru_konumu;
        pr::secenekler dogru_cevap;
        std::map<pr::secenekler, pr::celdiriciler> celdiriciler_map;
        int zorluk;

    public:
        soru() {}
        soru(const std::string& soru_konumu_, const pr::secenekler& dogru_cevap_, const std::map<pr::secenekler, pr::celdiriciler>& celdiriciler_map_, const int& zorluk_, const int yanlis_yapilma_sayisi_)
            : soru_konumu(soru_konumu_), dogru_cevap(dogru_cevap_), celdiriciler_map(celdiriciler_map_), zorluk(zorluk_), yanlis_yapilma_sayisi(yanlis_yapilma_sayisi_) {}

        bool operator==(const soru& diger) const { return soru_konumu == diger.soru_konumu; }
        auto cevap_dogru_mu(const pr::secenekler& cvp) const -> bool { return cvp == dogru_cevap; }
        auto celdirici(const pr::secenekler& cvp) const -> pr::celdiriciler {
            if (celdiriciler_map.contains(cvp)) return celdiriciler_map.at(cvp);
            return pr::DIGER;
        }
        void dosyaya_yazdir(std::ofstream& dosya) const {
            dosya << "SORU: " << soru_konumu << " " << secenek_to_string(dogru_cevap) << " ";
            dosya << celdiriciler_map.at(pr::A) << " " << celdiriciler_map.at(pr::B) << " " << celdiriciler_map.at(pr::C) << " "
                  << celdiriciler_map.at(pr::D) << " " << celdiriciler_map.at(pr::E) << " " << zorluk << " " << yanlis_yapilma_sayisi << "\n";
        }
        void yanlis_yapildi() { yanlis_yapilma_sayisi++; }
        auto soru_konumunu_goster() const -> std::string { return soru_konumu; }
        auto dogru_cevabi_goster() const -> secenekler { return dogru_cevap; }
        auto zorluk_goster() const -> int { return zorluk; }
    };

    class konu
    {
        std::string konu_ismi;
        std::vector<pr::soru> sorular;
        double konu_puani = 0.0;

    public:
        konu() {}
        konu(const std::string& konu_ismi_, const double konu_puani_) : konu_ismi(konu_ismi_), konu_puani(konu_puani_) {}
        bool operator==(const pr::konu& diger) const { return this->konu_ismi == diger.konu_ismi; }

        void soru_ekle(const pr::soru& s) {
            if (std::find(sorular.begin(), sorular.end(), s) == sorular.end()) sorular.push_back(s);
        }
        
        void soru_sil(const std::string& silinecek_konum) {
            std::erase_if(sorular, [&](const soru& s) { return s.soru_konumunu_goster() == silinecek_konum; });
        }

        void soru_yanlis_isaretle(const std::string& konum) {
            for (auto& s : sorular) {
                if (s.soru_konumunu_goster() == konum) { s.yanlis_yapildi(); break; }
            }
        }

        void dosyaya_yazdir(std::ofstream& dosya) const {
            if (sorular.empty()) return; 
            dosya << "KONU: " << konu_ismi << " " << konu_puani << "\n";
            for (const auto& soru : sorular) soru.dosyaya_yazdir(dosya);
        }

        void puan_guncelle(bool dogru_mu) {
            double degisim = 5.0 / std::cosh(konu_puani / 5.0); // sech
            if (dogru_mu) konu_puani += degisim;
            else konu_puani -= degisim;
        }

        auto uygun_sorular_sec(int adet) const -> std::vector<soru> {
            if (sorular.empty()) return {};
            double hedef_zorluk = std::clamp(3.0 + (konu_puani / 3.0), 1.0, 5.0); 
            std::vector<soru> kopya = sorular;
            static std::mt19937 rng(std::random_device{}());
            std::shuffle(kopya.begin(), kopya.end(), rng);
            std::stable_sort(kopya.begin(), kopya.end(), [hedef_zorluk](const soru& a, const soru& b) {
                return std::abs(a.zorluk_goster() - hedef_zorluk) < std::abs(b.zorluk_goster() - hedef_zorluk);
            });
            if (kopya.size() > static_cast<size_t>(adet)) kopya.resize(adet);
            return kopya;
        }

        auto puani_goster() const -> double { return konu_puani; }

        auto konu_ismini_goster() const -> std::string { return konu_ismi; }
        auto soru_sayisi() const -> size_t { return sorular.size(); }
    };

    using havuz_tipi = std::vector<pr::konu>;

    class havuzlar
    {
        std::map<int, havuz_tipi> havuz_map;
        int havuz_sayisi = 1;
        std::map<int, double> oranlar;
        bool okuma = true;

    public:
        havuzlar(const int& havuz_sayisi_) : havuz_sayisi(havuz_sayisi_) {
            for (int i = 0; i < havuz_sayisi; ++i) havuz_map.insert({i + 1, {}});
        }
        havuzlar(const bool& okuma_) : okuma(okuma_) {}

        auto havuz_sayisi_goster() const -> int { return havuz_sayisi; }

        // KRİTİK DÜZELTME: Olmayan havuzları da bellekte yaratır
        void havuz_sayisi_tanimla(const int& havuz_say) {
            havuz_sayisi = havuz_say;
            oranlar = havuz_orani_belirle(havuz_say);
            for (int i = 1; i <= havuz_sayisi; ++i) {
                if (!havuz_map.contains(i)) {
                    havuz_map[i] = {};
                }
            }
        }

        void konu_ekle(const pr::konu& k, const int& h) {
            havuz_map[h].push_back(k);
        }

        // --- YENİ MANTIK: Geriye yeni yerleştirilen havuzun numarasını döner ---
        int konuyu_veritabaninda_guncelle(const pr::konu& guncel_konu, bool dogru_mu)
        {
            int mevcut_havuz = -1;
            for (auto& [havuz_no, konular] : havuz_map) {
                auto it = std::find(konular.begin(), konular.end(), guncel_konu);
                if (it != konular.end()) {
                    konular.erase(it);
                    mevcut_havuz = havuz_no;
                    break;
                }
            }
            
            if (mevcut_havuz == -1) return -1;
            if (guncel_konu.soru_sayisi() == 0) return 0; // Sorusu biten konu tamamen silinir.

            int yeni_havuz = mevcut_havuz;
            if (dogru_mu && mevcut_havuz < havuz_sayisi) yeni_havuz++;
            else if (!dogru_mu && mevcut_havuz > 1) yeni_havuz--;

            havuz_map[yeni_havuz].push_back(guncel_konu);
            return yeni_havuz;
        }

        auto dosyaya_yaz() -> bool {
            auto dosya = std::ofstream{"havuzlar.maoh", std::ios::out | std::ios::trunc};
            if (!dosya.is_open()) return false;

            dosya << "MAOH.START-OF-FILE:3.3\n";
            dosya << "HAVUZ-SAYISI: " << havuz_sayisi << "\n";
            dosya << "HAVUZ-ORANLARI: ";
            oranlari_dosyaya_yaz(oranlar, dosya);

            for (const auto& [havuz_no, konular] : havuz_map) {
                if(konular.empty()) continue; // Boş olan havuzları dosyaya yazarak kalabalık yapmaz
                dosya << "HAVUZ: " << havuz_no << "\n";
                for (const auto& konu : konular) konu.dosyaya_yazdir(dosya);
            }
            dosya << "MAOH.END-OF-FILE:\n";
            return true;
        }

        auto rastgele_konu() const -> pr::konu {
            if (havuz_map.empty()) return pr::konu();
            static std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(1, 100);
            int rand_100 = dist(rng);
            int toplam = 0, secilen_havuz = 1;
            
            for (auto [havuz_no, oran] : oranlar) {
                toplam += static_cast<int>(oran * 100);
                if (rand_100 <= toplam) { secilen_havuz = havuz_no; break; }
            }
            
            if (havuz_map.at(secilen_havuz).empty()) {
                for(const auto& [h_no, konular] : havuz_map) {
                    if(!konular.empty()) { secilen_havuz = h_no; break; }
                }
            }
            const auto& konular = havuz_map.at(secilen_havuz);
            if (konular.empty()) return pr::konu();

            std::uniform_int_distribution<int> dist_konu(0, konular.size() - 1);
            return konular[dist_konu(rng)];
        }

        auto gecerli_mi() const -> bool { return okuma; }
        auto havuzlari_goster() const -> const std::map<int, havuz_tipi>& { return havuz_map; }
    };

    inline auto havuz_orani_belirle(const int& havuz_sayisi) -> std::map<int, double> {
        if (havuz_sayisi < 1 || havuz_sayisi > 6) return {};
        switch (havuz_sayisi) {
            case 1: return {{1, 1.0}};
            case 2: return {{1, 0.75}, {2, 0.25}};
            case 3: return {{1, 0.60}, {2, 0.25}, {3, 0.15}};
            case 4: return {{1, 0.50}, {2, 0.25}, {3, 0.16}, {4, 0.09}};
            case 5: return {{1, 0.50}, {2, 0.25}, {3, 0.12}, {4, 0.08}, {5, 0.05}};
            case 6: return {{1, 0.50}, {2, 0.25}, {3, 0.10}, {4, 0.07}, {5, 0.05}, {6, 0.03}};
        };
        return {};
    }

    inline auto oranlari_dosyaya_yaz(const std::map<int, double>& oranlar, std::ofstream& dosya) -> bool {
        for (auto [havuz_no, oran] : oranlar) dosya << oran << " ";
        dosya << "\n";
        return true;
    }

    inline auto maoh_okuyucu() -> pr::havuzlar {
        auto dosya = std::ifstream{"havuzlar.maoh", std::ios::in};
        if (!dosya.is_open()) return pr::havuzlar(false);

        dosya.imbue(std::locale("C"));
        std::string token;
        int havuz_sayisi = 1, aktif_havuz = 1;
        std::map<int, havuz_tipi> temp_map;
        std::string aktif_konu_ismi;

        while (dosya >> token) {
            if (token == "MAOH.START-OF-FILE:3.3") continue;
            if (token == "MAOH.END-OF-FILE:") break;

            if (token == "HAVUZ-SAYISI:") {
                dosya >> havuz_sayisi;
            } else if (token == "HAVUZ-ORANLARI:") {
                double cop; for (int i = 0; i < havuz_sayisi; ++i) dosya >> cop;
            } else if (token == "HAVUZ:") {
                dosya >> aktif_havuz;
                temp_map[aktif_havuz] = {};
            } else if (token == "KONU:") {
                double konu_puani;
                dosya >> aktif_konu_ismi >> konu_puani;
                temp_map[aktif_havuz].push_back(pr::konu(aktif_konu_ismi, konu_puani));
            } else if (token == "SORU:") {
                std::string soru_konumu;
                char dogru_c; 
                int cA, cB, cC, cD, cE, zorluk, yanlis;
                dosya >> soru_konumu >> dogru_c >> cA >> cB >> cC >> cD >> cE >> zorluk >> yanlis;

                pr::soru s(soru_konumu, char_to_secenek(dogru_c),
                    { {pr::A, static_cast<pr::celdiriciler>(cA)}, {pr::B, static_cast<pr::celdiriciler>(cB)}, {pr::C, static_cast<pr::celdiriciler>(cC)}, {pr::D, static_cast<pr::celdiriciler>(cD)}, {pr::E, static_cast<pr::celdiriciler>(cE)} },
                    zorluk, yanlis);
                if (!temp_map[aktif_havuz].empty()) temp_map[aktif_havuz].back().soru_ekle(s);
            }
        }

        pr::havuzlar sonuc(havuz_sayisi);
        sonuc.havuz_sayisi_tanimla(havuz_sayisi);
        for (const auto& [no, konular] : temp_map) {
            for (const auto& k : konular) sonuc.konu_ekle(k, no);
        }
        return sonuc;
    }
}
