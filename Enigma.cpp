#include <iostream>

using namespace std;

struct rotor {
	int kod = 0;
	rotor* nastepny = NULL;
	bool czy_obrot = false;
	int ktora_litera = 0;
};

void dodaj_element(rotor*& head, int element) {
	rotor* wsk = new rotor;
	wsk->kod = element;
	if (head) {
		int x = head->ktora_litera + 1;
		wsk->nastepny = head->nastepny;
		head->nastepny = wsk;
		wsk->ktora_litera = x;
	}
	else {
		wsk->nastepny = wsk;
		wsk->ktora_litera = 1;
	}
	head = wsk;
}

rotor* przeszukaj_rotor(rotor* head, int element, bool czy_kod) {
	if (element <= 0)
		return head;
	rotor* p = head;
	if (p) {
		if (czy_kod)
			while (p->kod != element)
				p = p->nastepny;
		else
			while (p->ktora_litera != element)
				p = p->nastepny;
	}
	return p;
}

void pokaz_listy(rotor* head) {
	rotor* p = head;
	do {
		cout << "Lista" << endl;;
		cout << "Numer litery:   " << p->ktora_litera << endl;
		cout << "Kod:            " << p->kod << endl;
		cout << "Nastepny:       " << p->nastepny << endl;
		cout << "Czy sie obraca: " << p->czy_obrot << endl << endl;
		p = p->nastepny;
	} while (p != head);
}

int ktory_element(rotor* head, int element, bool czy_kod) {
	int el = 1;
	if (czy_kod)
		while (head->kod != element) {
			head = head->nastepny;
			el++;
		}
	else
		while (head->ktora_litera != element) {
			head = head->nastepny;
			el++;
		}
	return el;
}

rotor* ustaw_pozycje(rotor* head, int przejscia) {
	while (przejscia > 1) {
		head = head->nastepny;
		przejscia--;
	}
	return head;
}

int szyfrowanie(rotor** head, int szyfr, int liczba_wirnikow, int* reflektor) {
	//Na dol
	for (int i = 0; i < liczba_wirnikow; i++) {
		szyfr = ustaw_pozycje(head[i], szyfr)->kod;
		szyfr = ktory_element(head[i], szyfr, false);
	}
	//Reflektor
	szyfr = reflektor[szyfr - 1];
	//Do gory
	for (int i = liczba_wirnikow - 1; i >= 0; i--) {
		szyfr = ustaw_pozycje(head[i], szyfr)->ktora_litera;
		szyfr = ktory_element(head[i], szyfr, true);
	}
	return szyfr;
}

void obrot_rotorow(rotor**& head, int liczba_wirnikow) {
	head[0] = head[0]->nastepny;
	for (int i = 1; i < liczba_wirnikow; i++)
		if (head[i - 1]->czy_obrot) {
			head[i] = head[i]->nastepny;
			//pokaz_listy(head[i]);
		}

}

void usun_element(rotor*& head) {
	if (head) {
		rotor* p = head->nastepny;
		head->nastepny = p->nastepny;
		if (p->nastepny == p)
			head = NULL;
		delete p;
	}
}


int main()
{
	//----- Definicja maszyny ---------
	int n = 0, m = 0;
	cin >> n >> m;
	rotor** wirnik = new  rotor * [m];
	for (int i = 0; i < m; i++) {
		wirnik[i] = NULL;
		int pomoc = 0;
		for (int j = 0; j < n; j++) {
			//dodawanie elementow do listy
			cin >> pomoc;
			dodaj_element(wirnik[i], pomoc);
		}
		cin >> pomoc;
		while (pomoc) {
			//zmienia czy_obrot na true
			int ktora_obraca;
			cin >> ktora_obraca;
			przeszukaj_rotor(wirnik[i], ktora_obraca, false)->czy_obrot = true;
			pomoc--;
		}
		//pokaz_listy(wirnik[i]);
	}
	int r = 0;
	cin >> r;
	int** reflektor = new int* [r];
	for (int i = 0; i < r; i++) {
		reflektor[i] = new int[n];
		for (int j = 0; j < n; j++)
			cin >> reflektor[i][j];
	}

	//----- Zestaw "p" do wykonania ---------
	int p = 0;
	cin >> p;
	for (int i = 0; i < p; i++) {
		int liczba_wirnikow = 0;
		cin >> liczba_wirnikow;
		rotor** uzywane_wirniki = new rotor * [liczba_wirnikow];
		int pomoc = 0, pozycja = 0;
		while (pomoc < liczba_wirnikow) {
			int ktory_wirnik = 0;
			cin >> ktory_wirnik >> pozycja;
			if (pozycja - pomoc >= 0)
				pozycja = (pozycja - pomoc) % n + 1;
			else
				pozycja = (pomoc - pozycja) % n + 1;
			uzywane_wirniki[pomoc] = wirnik[ktory_wirnik] = przeszukaj_rotor(wirnik[ktory_wirnik], pozycja, false);
			//pokaz_listy(uzywane_wirniki[pomoc],n);
			pomoc++;
		}
		int ktory_reflektor = 0;
		cin >> ktory_reflektor;

		int sekwencja = 0;
		cin >> sekwencja;
		//sekwencja = (sekwencja + pozycja-1) % n;
		do {
			cout << szyfrowanie(uzywane_wirniki, sekwencja, liczba_wirnikow, reflektor[ktory_reflektor]) << " ";
			obrot_rotorow(uzywane_wirniki, liczba_wirnikow);

			cin >> sekwencja;
		} while (sekwencja);
		cout << endl;
		//for (int i = 0; i < m; i++)
		//      while (uzywane_wirniki[i])
		//              usun_element(uzywane_wirniki[i]);
		//delete [] uzywane_wirniki;
	}

	//------ Zwalnianie pamieci ---------
	for (int i = 0; i < m; i++)
		while (wirnik[i])
			usun_element(wirnik[i]);
	delete[] wirnik;
	for (int i = 0; i < r; i++)
		delete[] reflektor[i];
	delete[] reflektor;
	return 0;
}