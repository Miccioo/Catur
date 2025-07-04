# ♟️ ChessGame: Beyond the Board

Selamat datang di **ChessGame**, tempat game catur klasik mendapatkan sentuhan revolusioner! Dikembangkan oleh tim beranggotakan tiga orang yang penuh semangat, proyek ini mendefinisikan ulang game strategi abadi dengan tiga mode yang berbeda dan mendebarkan. Bersiaplah untuk menantang pemikiran taktis Anda dengan cara yang tidak pernah Anda bayangkan!

Proyek ini dibuat khusus untuk memenuhi tugas mata kuliah Struktur Data dan Algoritma. Kami berupaya menerapkan konsep-konsep yang telah dipelajari dalam pengembangan fitur-fitur unik ini.

---

## 🚀 Fitur

### 1. Catur Klasik

Rasakan keindahan fundamental catur dengan rekreasi setia kami dari game tradisional. Sempurnakan pembukaan Anda, kuasai _endgame_ Anda, dan kalahkan lawan Anda dalam game klasik abadi ini.

### 2. Catur Mode Evolve

Bersiaplah untuk game di mana bidak dapat melampaui batasnya! Dalam Catur Mode Evolve, pemain dapat **menggabungkan dua bidak catur (kecuali Raja)** untuk menciptakan entitas yang lebih kuat. Nilai gabungan dari bidak yang digabungkan tidak dapat melebihi 8. Mode ini memperkenalkan lapisan strategi yang dinamis, memaksa Anda untuk berpikir kreatif tentang sinergi bidak dan kontrol papan.

* **Mekanisme Penggabungan:** Gabungkan bidak seperti Kuda dan Gajah untuk menciptakan penyerang yang tangguh, atau Benteng dan Pion untuk terobosan yang tidak terduga.
* **Batas Nilai:** Batas nilai maksimum 8 menambahkan batasan strategis, memastikan evolusi yang seimbang dan mencegah bidak menjadi terlalu kuat.
* PION_BERAT: Sekarang bisa bergerak 1 kotak ke samping secara ortogonal, selain gerakan Pion biasa.
* (KSATRIA_PIONIR, KUDA_BERPELINDUNG, GAJAH_PENJAGA, GAJAH_BIJAKSANA, BENTENG_PENYERBU, BENTENG_PIONIR): Bidak-bidak ini akan bisa melakukan gerakan dari kedua komponen aslinya (misalnya, KSATRIA_PIONIR bisa bergerak seperti Kuda ATAU seperti Pion). Ini diimplementasikan dengan mengecek kedua set kondisi gerakan dalam satu case.
* KSATRIA_GANDA (Kuda + Kuda): Bisa bergerak seperti Kuda biasa DAN bisa bergerak 1 kotak lurus ke segala arah (seperti Raja, tetapi tanpa castling).
* KOMANDAN_MEDAN/PENGUASA_GARIS_DEPAN (Kuda + Gajah): Bisa bergerak seperti Kuda ATAU seperti Gajah.
* GAJAH_AGUNG (Gajah + Gajah): Bisa bergerak seperti Gajah biasa DAN bisa bergerak 1 kotak lurus ke segala arah (seperti Raja, tetapi tanpa castling).

### 3. Catur Mode Zombie

Bersiaplah untuk pertempuran yang tidak terduga dan mendebarkan melawan mayat hidup! Dalam Catur Mode Zombie, **virus dapat menyebar di baris 4 atau 5**, mengubah bidak Anda melawan Anda.

* **Wabah Virus:** Jika sebuah bidak terinfeksi, bidak tersebut menjadi tidak terkendali dan akan bergerak sendiri satu kali.
* **Konversi:** Jika lawan menangkap bidak yang terinfeksi, bidak tersebut akan berubah dan bergabung dengan pasukan mereka, menambahkan lapisan risiko dan hadiah untuk setiap penangkapan.

---

## 🎮 Memulai

Instruksi terperinci tentang cara mengatur dan menjalankan ChessGame akan segera disediakan di sini.

---

## 👨‍💻 Tim Kami

Proyek ini diwujudkan oleh tim beranggotakan tiga pengembang yang berdedikasi:

* [Zaidan Arkan]     [241524030] (KETUA)
* [Nasrul Wahid]     [241524019]
* [Rizki Nurmahmudi] [241524027]

---

## 🤝 Kontribusi

Kami menerima kontribusi! Jika Anda memiliki saran untuk fitur baru, perbaikan _bug_, atau peningkatan, jangan ragu untuk membuka _issue_ atau mengirimkan _pull request_.

---

Selamat bermain dan semoga strategi Anda selalu berkembang!
