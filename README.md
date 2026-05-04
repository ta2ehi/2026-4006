# 2026-4006
2026 Tübitak 4006 Bilim Fuarı Projesi: Matematik Öğreniminde Adaptif Öğrenme Yöntemlerinin Uygulanması

### **yapilar.hpp Dosyası**

Bu header dosyası, ana dosyada kullanılan yapıları oluşturmak için yazılmıştır. Tüm `enum`, `class` ve fonksiyon yapıları; `namespace pr` içine yerleştirilmiştir. Bu dosya yazılırken `C++20` standartları ile C++'a eklenen modern yöntemler kullanılmıştır. Bu sebeple dosyayı `C++20` veya daha yeni C++ standartları kullanarak derlediğinizden emin olunuz.

Bu dosyada `"inline auto maoh_okuyucu() -> pr::havuzlar"` olarak tanımlanan dosya okuma fonksiyonunun, genel kabul ve prensiplere uygun olmasa da namespace pr içinde tanımlanan `class` yapılarıyla senkronize ve bağlı çalıştığından header dosyası içinde inline olarak tanımlanması ve ana dosyaya alınmaması uygun görülmüştür.

### **.maoh Dosya Formatı**

`.maoh`, bu projede tüm havuz, konu ve soru verilerinin depolandığı dosya tipidir. Dosyanın başında başlatma etiketi ve dosya sürümü yer alır: 
```
MAOH.START-OF-FILE:3.0
```
Daha sonra ayarlanmış havuz sayısı gelir:
```
HAVUZ-SAYISI:5
```
Daha sonra havuzlar tek tek eklenir. Her havuzun başında havuz numarası etiketi `HAVUZ:3` (3 numarası örnektir) bulunur ve alt satıra geçirir. Havuzların altındaki konular için konunun başında konu ismi etiketi `KONU:ASAL_AA_SAYILAR` (ASAL_AA_SAYILAR konu ismi örnektir) etiketi kullanılır ve alt satıra geçilir. Konuların altında ise sorular yer alır. Her soru bir satırı kaplar ve `SORU:` etiketiyle başlar. Etiketten sonra bir boşluk bırakılır ve aralarında bir boşluk olacak şekilde ve sırasıyla şu elemanlar yazılır: Soru konumu, doğru yanıt, A - E şıkları için sırayla ve aralarında bir boşluk bırakılarak çeldirici türleri (enum rakamları ile), soru zorluk seviyesi. Son olarak dosya sonu etiketi gelir:
```
MAOH.END-OF-FILE:
```

Örnek dosya:
```
MAOH.START-OF-FILE:3.0
HAVUZ-SAYISI:5
HAVUZ:1
KONU:ASAL_AA_SAYILAR
SORU: OGM_TYT_MAT_2025_TEST2_SORU1 D 2 4 1 0 8 2
SORU: OGM_TYT_MAT_2025_TEST2_SORU2 A 0 1 4 5 3 4
SORU: (...)
KONU:BOLUNEBILME
SORU: OGM_TYT_MAT_2025_TEST4_SORU1 B 2 0 1 4 1 1
(...)
HAVUZ:2
KONU:(...)

(...)

MAOH.END-OF-FILE:
```
gibi.

## MANTIK
Projede amaç, kullanıcının matematik öğrenme deneyiminin en verimli hâle getirilmesidir. Bunun için iki farklı yöntem aynı anda uygulanır.

1. **Flashcards Yöntemi:** Genellikle ezber yapmakta kullanılan bu yöntemin uyarlanmış bir versiyonunu projemizde kullanıyoruz. Orijinal yöntemde bir kartın bir yüzüne soru, diğer yüzüne de doğru cevap yazılır ve kartlar üst üste dizilir. Daha sonra sırayla kartlardaki sorular okunarak doğru cevap hatırlanmaya çalışılır. Doğru cevap verilirse kart bir üst desteye alınır, yanlış cevap verilirse ise kart bir alt desteye alınır. Bu şekilde 5 kart destesi oluşturulur ve bu desteler sırasıyla 1, 2, 4, 7 ve 14 günde bir tekrar edilir. Bu şekilde, çok yanlış yapılan kartların sık tekrar edilerek öğrenilmesi amaçlanır.

  Bu projede matematik konuları, birer kart gibi düşünülmüş olup kullanıcının tercihi kadar havuza ayrılır. Kullanıcıya soru sorulacağı zaman havuzlar için kullanıcının belirlediği yüzdelikler üzerinden ağırlıklandırılmış olarak rastgele bir konu seçilir. Örneğin 5 havuzun sırasıyla %50, %25, %12, %8 ve %5 oranlarında ağırlıklandırıldığı varsayılan senaryoda kullanıcıya sorulacak soru için belirlenen rastgelelik; %50 ihtimalle 1. havuz, %25 ihtimalle 2. havuz, ... şeklinde olur. Konu hakkında sorulan sorular belli oranda doğru yanıtlanamazsa bu konu bir alt havuza aktarılır, soruların tamamı doğru yanıtlanırsa da konu bir üst havuza aktarılır. Herhangi bir havuzda hiç konu kalmaması durumunda yüzdlikler yeni havuz sayısı için yeniden dağıtılır.

2. **Adaptif Öğrenme:** 
