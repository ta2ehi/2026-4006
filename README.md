# 2026-4006
### **_2026 Tübitak 4006 Bilim Fuarı Projesi: Matematik Öğreniminde Adaptif Öğrenme Yöntemlerinin Uygulanması_**

**Uygulamayı kullanmak için C++ dosyasını derleyebilir veya releases kısmında yer alan zip dosyasını indirdikten sonra exe içindeki uygulamayı çalıştırabilirsiniz. Uygulamanın çalışabilmesi için tüm dosyayı ayıkladığınızdan emin olunuz.**

**Proje Türü: Tasarım**

**Ana Alanı: Matematik**

**Tematik Konusu: Algoritma Tasarımı ve Uygulamaları**

**Danışman Öğretmen: Sn. Ahmet ALTINOK**

**_Hazırlayanlar:_**

**Kayra YİĞİTER**

**Yalın İNCE**

## **yapilar.hpp Dosyası**

Bu header dosyası, ana dosyada kullanılan yapıları oluşturmak için yazılmıştır. Tüm `enum`, `class` ve fonksiyon yapıları; `namespace pr` içine yerleştirilmiştir. Bu dosya yazılırken `C++20` standartları ile C++'a eklenen modern yöntemler kullanılmıştır. Bu sebeple dosyayı `C++20` veya daha yeni C++ standartları kullanarak derlediğinizden emin olunuz.

## **.maoh Dosya Formatı**

`.maoh`, bu projede tüm havuz, konu ve soru verilerinin depolandığı dosya tipidir. Dosyanın başında başlatma etiketi ve dosya sürümü yer alır: 
```
MAOH.START-OF-FILE:3.3
```
Daha sonra ayarlanmış havuz sayısı gelir:
```
HAVUZ-SAYISI: 5
```
Daha sonra havuzlar için belirlenmiş oranları gelir:
```
HAVUZ-ORANLARI: 0.5 0.25 0.12 0.08 0.05
```
Daha sonra havuzlar tek tek eklenir. Her havuzun başında havuz numarası etiketi `HAVUZ:3` (3 numarası örnektir) bulunur ve alt satıra geçirir. Havuzların altındaki konular için konunun başında konu ismi etiketi `KONU:ASAL_AA_SAYILAR` (ASAL_AA_SAYILAR konu ismi örnektir) etiketi kullanılır, daha sonra konu puanı gelir ve alt satıra geçilir. Konuların altında ise sorular yer alır. Her soru bir satırı kaplar ve `SORU:` etiketiyle başlar. Etiketten sonra bir boşluk bırakılır ve aralarında bir boşluk olacak şekilde ve sırasıyla şu elemanlar yazılır: Soru konumu, doğru yanıt, A - E şıkları için sırayla ve aralarında bir boşluk bırakılarak çeldirici türleri (enum rakamları ile), soru zorluk seviyesi. Son olarak dosya sonu etiketi gelir:
```
MAOH.END-OF-FILE:
```

Örnek dosya:
```
MAOH.START-OF-FILE:3.3
HAVUZ-SAYISI: 5
HAVUZ-ORANLARI: 0.5 0.25 0.12 0.08 0.05
HAVUZ:1
KONU:ASAL_AA_SAYILAR 3.45
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

_.maoh dosya formatının son kararlı sürümü 3.3 sürümüdür._

## Mantık
Projede amaç, kullanıcının matematik öğrenme deneyiminin en verimli hâle getirilmesidir. Bunun için iki farklı yöntem aynı anda uygulanır.

**_1. Flashcards Yöntemi:_** Genellikle ezber yapmakta kullanılan bu yöntemin uyarlanmış bir versiyonunu projemizde kullanıyoruz. Orijinal yöntemde bir kartın bir yüzüne soru, diğer yüzüne de doğru cevap yazılır ve kartlar üst üste dizilir. Daha sonra sırayla kartlardaki sorular okunarak doğru cevap hatırlanmaya çalışılır. Doğru cevap verilirse kart bir üst desteye alınır, yanlış cevap verilirse ise kart bir alt desteye alınır. Bu şekilde 5 kart destesi oluşturulur ve bu desteler sırasıyla 1, 2, 4, 7 ve 14 günde bir tekrar edilir. Bu şekilde, çok yanlış yapılan kartların sık tekrar edilerek öğrenilmesi amaçlanır.

  Bu projede matematik konuları, birer kart gibi düşünülmüş olup kullanıcının tercihi kadar havuza ayrılır. Kullanıcıya soru sorulacağı zaman havuzlar için kullanıcının belirlediği yüzdelikler üzerinden ağırlıklandırılmış olarak rastgele bir konu seçilir. Örneğin 5 havuzun sırasıyla %50, %25, %12, %8 ve %5 oranlarında ağırlıklandırıldığı varsayılan senaryoda kullanıcıya sorulacak soru için belirlenen rastgelelik; %50 ihtimalle 1. havuz, %25 ihtimalle 2. havuz, ... şeklinde olur. Konu hakkında sorulan sorular belli oranda doğru yanıtlanamazsa bu konu bir alt havuza aktarılır, soruların tamamı doğru yanıtlanırsa da konu bir üst havuza aktarılır. Herhangi bir havuzda hiç konu kalmaması durumunda yüzdlikler yeni havuz sayısı için yeniden dağıtılır.

**_2. Adaptif Öğrenme:_** Uygulamada yer alan her soru için 1-5 arasında (1: Çok kolay, 2: Kolay, 3: Orta, 4: Zor, 5: Çok zor) birer zorluk seviyesi belirlenmiştir. Kullanıcı soruları çözdükçe her konu için bir puanı oluşur. Doğru çözülen sorular puanı artırır, yanlış çözülen sorular ise azaltır. Yeni soruların zorluk seviyesi, puana göre belirlenir. Puanı yüksek olan konulardan sorulacak soruların zorluk seviyeleri daha yüksek olacaktır. Böylelikle kullanıcının zayıf olduğu konularda kolay sorular ile konunun öğrenilmesi sağlanacak, kullanıcının güçlü olduğu konularda ise zor sorular ile mükemmelleşme hedeflenecektir.

Adaptif öğrenme uygulanırken, sorulacak 5 sorunun tamı doğru cevaplanırsa o konunun puanına, konunun puanı p olmak üzere f(p) = 5 * sech(p/5) + p donusumu uygulanacaktır. Böylece çok zayıf olan konularda puanın birdenbire artmasının önüne geçilirken ortalama seviyede olunan konularda iyileşildiğinde puanın sıçraması sağlanmıştır. Zaten iyi seviyede olunan konularda ise puanın çok fazla yükselerek konunun kendi kendi devre dışı bırakmasına sebep olmaması için bu tercih yapılmıştır

### Algoritma:
1. En üstte havuzlar sistemi, daha sonra sırasıyla havuz, konu ve sorular gelmek üzere hiyerarşik bir yapı oluştur.
2. Havuzlar sistemindeki *n* havuzu 1'den *n*'e kadar numaralandır.
3. En az 1, en çok 100 olacak şekilde rastgele bir tam sayı seç ve bu sayıyı *r* olarak tanımla.
4. *j* sayısını 0 olarak tanımla.
5. Tekrarla:

  i. *j* sayısını 1 artır.
  
  ii. 1.den *j*. havuza kadar olan tüm havuzların rastgelelik oranlarını topla.
  
  iii. Bu toplam *r*'den küçük veya *r*'ye eşit ise 6. adımdan devam et.
  
7. *j* numaralı havuzu seçilen havuz olarak tanımla.
8. Seçilen havuzdan rastgele bir konu seç ve bu konuyu seçilen konu olarak tanımla.
9. Seçilen konunun mevcut puanı (*p*) ile *z* değişkenini tanımla: *z = 3 + (p / 3)*. *z < 1* ise *z = 1*, *z > 5* ise *z = 5* tanımla.
10. Seçilen konunun içinden, zorluk puanı *z*'ye en yakın olan sorulardan rastgele bir tanesini seç ve kullanıcıya sor.
11. Girilen cevabı kontrol et:
     
      Cevap doğru:
    
     i. Seçilen konunun puanını (*k*) tekrar tanımla: *k = k + 5 * sech(k / 5)*.
    ii. Seçilen soruyu havuzlar sisteminden kaldır.
    iii. Seçilen konuyu havuzlar sisteminde bir üst havuza taşı. Zaten en üst havuzda ise hiçbir şey yapma.
      Cevap yanlış veya boş:
    
    i. Seçilen konunun puanını (*k*) tekrar tanımla: *k = k - 5 * sech(k / 5)*.
    ii. Seçilen konuyu havuzlar sisteminde bir alt havuza taşı. Zaten en alt havuzda ise hiçbir şey yapma.
    iii. Seçeneğe ait muhtemel çeldiriciyi kullanıcıya göster.
    
12. Kullanıcının talebi halinde 3. adıma geri dön.
