#include "std_lib_facilities.h"

enum struct farbe { kreuz = 12, pik = 11, karo = 9, herz = 10 };
enum struct wert { sieben, acht, neun, bube, dame, koenig, zehn, as };
struct karte { farbe f; wert w; };


void init(karte *skat)  {
  for (int i = 0;  i < 4;  ++i)  
    for (int j = 0;  j < 8;  ++j)  {
      karte *k = skat + i * 8 + j;
      switch (i)  {
        case 0: k->f = farbe::kreuz; break; case 1: k->f = farbe::pik;   break;
        case 2: k->f = farbe::karo;  break; case 3: k->f = farbe::herz;  break;
      }
      switch (j)  {
        case 0: k->w = wert::sieben; break; case 1: k->w = wert::acht;   break;
        case 2: k->w = wert::neun;   break; case 3: k->w = wert::bube;   break;
        case 4: k->w = wert::dame;   break; case 5: k->w = wert::koenig; break;
        case 6: k->w = wert::zehn;   break; case 7: k->w = wert::as;     break;
      }
    }
}

void mix(karte *skat)  {
  random_device rd;  mt19937 gen(rd( ));
  uniform_int_distribution<int> dis(0, 31);
  for (int i = 0;  i < 32;  ++i)  {
     int position = dis(gen);
     karte temp = skat[i];
     skat[i] = skat[position];
     skat[position] = temp;  
  }
}

void printFarbeWert(karte *k)  {
      switch (k->f)  {
        case farbe::kreuz: cout << "Kreuz, "; break; case farbe::pik: cout << "Pik, ";   break;
        case farbe::karo: cout << "Karo, ";   break; case farbe::herz: cout << "Herz, ";  break;
      }
      switch (k->w)  {
        case wert::sieben: cout << "Sieben";  break; case wert::acht: cout << "Acht";     break;
        case wert::neun: cout << "Neun";      break; case wert::bube: cout << "Bube";     break;
        case wert::dame: cout << "Dame";      break; case wert::koenig: cout << "Koenig"; break;
        case wert::zehn: cout << "Zehn";      break; case wert::as: cout << "As";         break;
      }
}

void printSkat(karte *skat)  {
  for (int i = 0;  i < 32;  ++i)  {
    printFarbeWert(skat + i);  cout << endl;
  }
}

int main() {
  random_device rd;  mt19937 gen(rd( ));
  uniform_int_distribution<int> dis(0, 31);
  karte skat[32];
  init(skat);
  // printSkat(skat);
  mix(skat);
  // printSkat(skat);
  printFarbeWert(&skat[0]); cout << endl;
  printFarbeWert(&skat[1]); cout << endl;
  return 0;  
}
