# **OpenMP dan OpenMPI**

## Semester II Tahun 2021/2022

### Tugas Besar 1 IF3230 Sistem Paralel dan Terdistribusi

_Program Studi Teknik Informatika_ <br />
_Sekolah Teknik Elektro dan Informatika_ <br />
_Institut Teknologi Bandung_ <br />

_Semester II Tahun 2021/2022_

## **Description**

program paralel yang menerima satu matriks kernel dan n buah matriks masukan. Program akan melakukan operasi konvolusi matriks pada matriks kernel dan setiap matriks masukan untuk menghasilkan matriks hasil konvolusi. Setelah itu, program Anda akan membuat n bilangan bulat positif yang merupakan selisih elemen terbesar dan terkecil dari setiap matriks hasil konvolusi, serta melakukan sorting dari n bilangan tersebut. <br />
<br/>
Keluaran program Anda adalah nilai maksimum, nilai minimum, median, dan rata-rata n bilangan tersebut. Nilai median dan rata-rata dihitung menggunakan integer division untuk pembulatan.

## **Author**

1. Daru Bagus Dananjaya (13519080)
2. Aulia Adila (13519100)
3. Shifa Salsabiila (13519106)

## **Requirements**

- [C](https://www.gnu.org/software/gcc/gcc.html)
- [OpenMPI](https://www.open-mpi.org/)
- [OpenMP](https://openmp.org/)

## **Analisis Program**

### Cara Kerja Program

#### OpenMP

OpenMP adalah standar yang kita digunakan untuk melakukan pemrograman paralel multithreading pada arsitektur shared memory. Termasuk pada compiler GCC, sehingga tidak perlu instalasi tambahan. Kompilasi program menggunakan gcc dengan flag `--openmp` dan menggunakan library `"omp.h"`.
<br><br>
Langkah yang dilakukan adalah dengan menggunakan #pragma omp _parallel for_ untuk melakukan paralelisasi pada instruksi for. Hal ini kami lakukan saat membentuk V subset inisialisasi dengan weight 0 dan parent adalah i. Selain itu kami juga melakukan omp _nowait_ karena tidak dibutuhkan sinkronisasi setelah looping. Kami juga menggunakan #pragma omp critical untuk bagian yang tidak bisa dilakukan bersamaan saat proses paralel.
<br><br>
Selain itu kami juga menggunakan #pragma omp task untuk melakukan paralelisasi terhadap 2 buah instruksi rekursif saat melakukan quick sort yang dibungkus dengan #pragma omp single dan dibungkus dengan #pragma omp parallel

#### OpenMPI

OpenMPI (Message Passing Interface) merupakan sebuah antarmuka protokol komunikasi terdistribusi. Open MPI membagi kinerja proses ke dalam beberapa node device yang saling terhubung sehingga program dapat dijalankan secara parallel. Kompilasi program menggunakan `mpicc` dan menggunakan library `"mpi.h"`. Program dijalankan dengan menggunakan `mpirun`.
<br><br>
Pertama-tama, MPI diinisialisasi dengan perintah `MPI_Init`. Selanjutnya, dilakukan inisialisasi terhadap nilai world*size dan world_rank yang merepresentasikan jumlah proses dan indeks proses. User akan memasukkan input pada \_root process*, yaitu proses dengan word_rank == 0.
<br><br>
MPI juga memiliki fungsi untuk melakukan broadcast yang mengirimkan data yang sama untuk seluruh proses, seperti matriks kernel, jumlah matriks input, jumlah baris input, dan jumlah kolom input. Untuk mendistribusikan matriks pada setiap proses, digunakan perintah `MPI_Send`. Proses akan menerima input matriks dengan perintah `MPI_Recv`.

### Analisis Waktu Tercepat Program Parallel dan Serial

Berdasarkan pengujian yang dilakukan, waktu tercepat yang diperoleh adalah pada TC1. Perbandingan waktu eksekusi dengan program serial dapat dilihat pada tabel dibawah ini.

| Test Case | Serial   | Parallel |
| --------- | -------- | -------- |
| TC1       | 0.023833 | 0.066698 |
| TC2       | 3.583684 | 0.931103 |
| TC3       | 3.146370 | 1.124144 |
| TC4       | 51.374452| 11.784437|

TC1 menghasilkan waktu eksekusi tercepat oleh proses serial. Meskipun waktu eksekusi pada TC1-serial paling cepat secara keseluruhan, namun dapat dilihat berdasarkan hasil pengujian yang diperoleh, bahwa waktu eksekusi tercepat TC yang lain diperoleh oleh proses paralel. Hal ini disebabkan oleh kecilnya jumlah data yang diproses pada TC1, sehingga overhead yang dihasilkan pada TC1-paralel lebih besar dibandingkan waktu eksekusi test case-nya sendiri. 

### Analisis Hasil Program Parallel dan Serial

Program parallel dan program serial memiliki nilai akhir (hasil komputasi) yang sama, namun dengan waktu eksekusi berbeda karena perbedaan tipe _processing_.  
- Serial
  - Setiap task dijalankan dalam satu waktu
  - Task dieksekusi dalam processor secara berurutan
- Parallel
  - Sejumlah task diselesaikan dalam satu waktu dengan processor yang berbeda-beda

### Analisis Waktu Eksekusi dari Percobaan

#### Test Case 1
| No  | Node OpenMPI | Thread OpenMP | Waktu Eksekusi |
| --- | ------------ | ------------- | -------------- |
| 1   | 2            | 5             | 0.070582       |
| 2   | 2            | 16            | 0.098614       |
| 3   | 3            | 5             | 0.076323       |
| 4   | 3            | 16            | 0.066698       |
| 5   | 4            | 5             | 0.088775       |
| 6   | 4            | 16            | 0.085719       |

#### Test Case 2
| No  | Node OpenMPI | Thread OpenMP | Waktu Eksekusi |
| --- | ------------ | ------------- | -------------- |
| 1   | 2            | 5             | 1.220536       |
| 2   | 2            | 16            | 1.064894       |
| 3   | 3            | 5             | 1.187758       |
| 4   | 3            | 16            | 1.124650       |
| 5   | 4            | 5             | 0.934219       |
| 6   | 4            | 16            | 0.931103       |

#### Test Case 3
| No  | Node OpenMPI | Thread OpenMP | Waktu Eksekusi |
| --- | ------------ | ------------- | -------------- |
| 1   | 2            | 5             | 1.294398       |
| 2   | 2            | 16            | 1.206018       |
| 3   | 3            | 5             | 1.416028       |
| 4   | 3            | 16            | 1.297336       |
| 5   | 4            | 5             | 1.215546       |
| 6   | 4            | 16            | 1.124144       |

#### Test Case 4
| No  | Node OpenMPI | Thread OpenMP | Waktu Eksekusi |
| --- | ------------ | ------------- | -------------- |
| 1   | 2            | 5             | 20.690235      |
| 2   | 2            | 16            | 13.661053      |
| 3   | 3            | 5             | 15.432311      |
| 4   | 3            | 16            | 13.750079      |
| 5   | 4            | 5             | 12.131542      |
| 6   | 4            | 16            | 11.784437      |

OpenMP dan OpenMPI keduanya memungkinkan suatu program untuk dieksekusi melalui lebih dari satu proses. Secara teori, maka seharusnya suatu program yang dieksekusi dengan menggunakan openMP dan openMPI akan memiliki waktu eksekusi yang lebih cepat dibandingkan program yang sama yang dieksekusi secara serial. Pada implementasinya, sebuah program yang dieksekusi secara parallel akan memiliki waktu eksekusi yang lebih cepat dibandingkan program serialnya ketika berada di atas suatu batas beban tertentu. Hal ini terjadi karena program yang dibuat dengan openMP dan openMPI memiliki overhead untuk proses persiapan serta distribusi datanya. Dari hasil data di atas, dapat dilihat bahwa keberadaan proses paralel dan distribusi task lebih bermanfaat pada task dengan ukuran besar. Pada contoh ini, dapat dilihat bahwa penambahan jumlah node dan thread pada test case 4 (test case dengan ukurang paling besar) mempercepat prorses penyelesaian task secara paling signifikan, sementara pada task berukuran kecil, dalam contoh ini test case 1, jumlah penambahan node ataupun thread tidak terlalu berpengaruh secara signifikan terhadap kecepatan eksekusi task. Hal ini membuktikan bahwa OpenMPI dan OpenMP akan mempercepat sebuah task ketika task tersebut cukup besar dan beban yang diselesaikan secara paralel lebih banyak dibandingkan overhead yang harus dibayar. 
