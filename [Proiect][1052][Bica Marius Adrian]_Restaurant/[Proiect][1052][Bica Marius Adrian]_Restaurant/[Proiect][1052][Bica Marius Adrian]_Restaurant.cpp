#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<string>
using namespace std;


class ExceptieComanda {
    string mesaj;
public:
    ExceptieComanda(string mesaj) :mesaj(mesaj) {}
    string getMesaj() { return this->mesaj; }
};

struct oraRestaurant {
    int ora;
    int minute;
};

struct programRestaurant {
    string ziua;
    oraRestaurant oraDeschidere;
    oraRestaurant oraInchidere;
};

class Ingredient {
    static int idCurent;
    const int id;
    char* denumire=nullptr;
    int cantitate=0;
  
public:
    Ingredient(const int id):id(++Ingredient::idCurent) {} 

    Ingredient():id(++Ingredient::idCurent),denumire(nullptr),cantitate(0){} 

    Ingredient(const char* denumire,int cantitate):id(++Ingredient::idCurent){
        this->denumire = new char[strlen(denumire)+1];
        strcpy(this->denumire, denumire); 
        this->cantitate = cantitate;
    }

    Ingredient(const Ingredient& i) :id(i.id) {
        this->denumire = new char[strlen(i.denumire) + 1];
        strcpy(this->denumire, i.denumire);
        this->cantitate = i.cantitate;

    }

    Ingredient& operator=(const Ingredient& i) {
        if (this != &i) {
            delete[] this->denumire;
            this->denumire = nullptr;
            this->denumire = new char[strlen(i.denumire) + 1];
            strcpy(this->denumire, i.denumire);
            this->cantitate = i.cantitate;
        }
        else {
            delete[] this->denumire;
            this->denumire = nullptr;
        }
        return *this;
    }

    ~Ingredient() {
        delete[] this->denumire;
        this->denumire = nullptr;
    }

    static int getID() {
        return idCurent;
    }

    static int getIdFinal() {
        return Ingredient::idCurent;
    }

    int getCantitate() const {
        return cantitate;
    }

    void setDenumire(const char* denumireNoua) {
        if (denumireNoua != nullptr) {
            if (strcmp(denumire, denumireNoua) != 0) {
                delete[] this->denumire;
                denumire = new char[strlen(denumireNoua) + 1];
                strcpy(denumire, denumireNoua);
            }
        }
    }

        const char* getDenumire() {
            return denumire;
        }

    friend ostream& operator<<(ostream& os, const Ingredient& i) {
        os << "======================="<<endl;
        os << "ID: " << i.id << endl;
        os << "Denumire: ";
        if (i.denumire != nullptr) {
            os << i.denumire;
        }
        else {
            os << "-";
        }
        os << endl;
        os << "Cantitate: "<<i.cantitate;
        os << endl;
        return os;

    }

    friend istream& operator>>(istream& is, Ingredient& i) {
        cout << "Introduceti denumirea ingredientului: ";
        string buffer;
        is >> buffer;
        i.denumire = new char[buffer.size()+1];
        strcpy(i.denumire, buffer.data());
        cout << "Introduceti cantitatea: ";
        is >> i.cantitate;
        cout << endl;
        return is;
    }

    Ingredient& operator+=(int cantitatePlus) {
        this->cantitate += cantitatePlus;
        return *this;
    }

    Ingredient& operator-=(int cantitateMinus) {
        if (this->cantitate - cantitateMinus >= 0)
            this->cantitate -= cantitateMinus;
        else this->cantitate = 0;
        return *this;
    }

    bool operator<(int valoare) {
        if (this->cantitate < valoare)
            return true;
        else return false;
    }

    bool operator>=(int valoare) {
        if (this->cantitate >= valoare)
            return true;
        else return false;
    }

    friend ostream& operator<<(ostream& os, const Ingredient& i);
    friend istream& operator>>(istream& is, Ingredient& i);

};

class Retetar {
    Ingredient** ingrediente;
    int* cantitateNecesara=nullptr;
   /* string* modalitatePreparare=nullptr;*/
public:

    Retetar(Ingredient** ingredienteParametru, int* cantitateNecesara/*, string* modalitatePreparare*/) {
        this->ingrediente = new Ingredient*[Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            this->ingrediente[i] = ingredienteParametru[i];
        }
        this->cantitateNecesara = new int[Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            this->cantitateNecesara[i] = cantitateNecesara[i];
        }
    }

    Retetar(const Retetar& r) {

        this->ingrediente = new Ingredient * [Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++)
            this->ingrediente[i] = r.ingrediente[i];
        this->cantitateNecesara = new int[Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++)
            this->cantitateNecesara[i] = r.cantitateNecesara[i];
        }

    Retetar operator=(const Retetar& r) {
        if (this != &r) {
            for (int i = 0; i < Ingredient::getIdFinal(); i++) {
                delete this->ingrediente[i];
            }
            delete[] this->ingrediente;
            this->ingrediente = nullptr;

            this->ingrediente = new Ingredient*[Ingredient::getIdFinal()];
            this->cantitateNecesara = new int[Ingredient::getIdFinal()]; 
            for (int i = 0; i < Ingredient::getIdFinal(); i++)
                this->ingrediente[i] = r.ingrediente[i];
            for (int i = 0; i < Ingredient::getIdFinal(); i++)
                this->cantitateNecesara[i] = r.cantitateNecesara[i];
        }
        else {
            for (int i = 0; i < Ingredient::getIdFinal(); i++) {
                delete this->ingrediente[i];
            }
            delete[] this->ingrediente;
            this->ingrediente = nullptr;
        }
        return *this;
    }

     Ingredient** getIngrediente() {
        return ingrediente;
    }

    ~Retetar() { 
        delete[] this->ingrediente;
        this->ingrediente = nullptr;
        delete[] this->cantitateNecesara;
        this->cantitateNecesara=nullptr;
    }

    int* getCantitateNecesara() {
        return cantitateNecesara;
    }

    bool operator==(char* existaIngredient) {
        for (int i = 1; i < Ingredient::getIdFinal(); i++) {
            if (strcmp(existaIngredient, ingrediente[i]->getDenumire()) == 0) {
                return true;
            }
        }
            return false;
        
    }

    bool operator!=(char* existaIngredient) {
        for (int i = 1; i < Ingredient::getIdFinal(); i++) {
            if (strcmp(existaIngredient, ingrediente[i]->getDenumire()) != 0) {
                return true;
            }
        }
        return false;
    }

    friend ostream& operator<<(ostream& os, const Retetar& r) {
        os << "========================" << endl;
        os << "Denumire ingrediente: ";
        for (int i = 0; i < Ingredient::getIdFinal(); i++)
            os << r.ingrediente[i]->getDenumire()<<" ";
        os << endl;
        os << "Cantitate necesara: ";
        for (int i = 0; i < Ingredient::getIdFinal(); i++)
            os << r.cantitateNecesara[i]<<" ";
        os << endl;
        return os;
    }

    friend istream& operator>>(istream& is, Retetar& r) {
        cout << "Introduceti cantitatea necesara: "<<endl;
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            int cantitate;
            cout << r.ingrediente[i]->getDenumire()<<" ";
            is >> cantitate;
            r.cantitateNecesara[i] = cantitate;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Retetar& r);
    friend istream& operator>>(istream& is, Retetar& r);
};

class Produs {
    string denumire = "N/A";
    Retetar* reteta=nullptr;
    bool esteDisponibil = 1;

public:
    Produs() {}

    Produs(string denumire, Retetar* reteta, bool esteDisponibil) { 
        this->denumire = denumire;
        this->reteta = reteta;
        this->esteDisponibil = esteDisponibil;
    }

    Produs (const Produs& p) {
        this->denumire = p.denumire;
        this->reteta = p.reteta;
        this->esteDisponibil = p.esteDisponibil;
    }

    Produs& operator=(const Produs& p) {
        if (this != &p) {
            this->reteta = nullptr;
            this->denumire = p.denumire;
            this->reteta = p.reteta;
            this->esteDisponibil = p.esteDisponibil;
        }
        else {
            this->reteta = nullptr;
        }
        return *this;
    }

    ~Produs() {
        this->reteta = nullptr;
    }

    void setDenumire(const string& denumireNoua) {
        if (denumireNoua != denumire)
            denumire = denumireNoua;
    }

    const string getDenumire() {
        return denumire;
    }

     Retetar getReteta() {
        return *reteta;
    }


    friend ostream& operator<<(ostream& os, const Produs& p) {
        os <<"Denumire produs: "<< p.denumire << endl;
        os << "Reteta produs: ";
        os << *(p.reteta);
        os << "Disponibilitate produs: " << p.esteDisponibil << endl;
        return os;
    }

    friend istream& operator>>(istream& is, Produs& p) {
        cout << "Denumire produs: ";
        is >> p.denumire;
        cout<<endl;
        cout << "Disponibilitate produs: ";
        is >> p.esteDisponibil;
        cout<< endl;
        return is;
    }


    friend ostream& operator<<(ostream& os,const Produs& p);
    friend istream& operator>>(istream& is,Produs& p);
};

class Pizza : public Produs {

};

class Paste : public Produs {

};

class Meniu {
    Produs** produse;
    int nrProduse=0;
    int* preturi=nullptr;
public:
    Meniu(Produs** produse,int* preturi,int nrProduse) {  
        this->nrProduse = nrProduse;
        this->produse = new Produs*[nrProduse];
        this->preturi = new int[nrProduse];
        for (int i = 0; i < this->nrProduse; i++) {
            this->produse[i] = produse[i];
            this->preturi[i] = preturi[i];
        }
    }
    int getNrProduse(){
        return this->nrProduse;
    }

    Produs** getProduse() {
        return produse;
    }

    int* getPreturi() {
        return preturi;
    }

    Meniu(const Meniu& m) {
        this->nrProduse = m.nrProduse;
        produse = new Produs * [m.nrProduse];
        preturi = new int[m.nrProduse];
        for (int i = 0; i < m.nrProduse; i++) {
            this->produse[i] = m.produse[i];
            this->preturi[i] = m.preturi[i];
        }
    }

    Meniu operator=(const Meniu& m) {
        if (this != &m) {
            delete[] this->produse;
            this->produse;
            delete[] this->preturi;
            this->preturi = nullptr;

            this->nrProduse = m.nrProduse;
            produse = new Produs * [m.nrProduse];
            preturi = new int[m.nrProduse];
            for (int i = 0; i < m.nrProduse; i++) {
                this->produse[i] = m.produse[i];
                this->preturi[i] = m.preturi[i];
            }
        }
        else {
            delete[] this->produse;
            this->produse;
            delete[] this->preturi;
            this->preturi = nullptr;
        }
        return *this;
    }

    ~Meniu() {
        delete[] this->produse;
        this->produse;
        delete[] this->preturi;
        this->preturi = nullptr;
    }

};

class Comanda {
    static int nrComenzi;
    const int idComanda;
    string operatiune = "N/A";
    int nrProduse = 0;
    int* cantitateProdus=nullptr;
    Produs* produse=nullptr;
    Meniu* meniu = nullptr;
public:
    Comanda() :idComanda(++nrComenzi) {
    }

    Comanda(const int idComanda, string operatiune, int nrProduse,int* cantitateProdus, Produs* produse,Meniu* meniu) :idComanda(++nrComenzi) {
        if (operatiune != "CMD")
            throw exception("Operatie invalida");
        this->operatiune = operatiune;

        this->nrProduse = nrProduse;
        this->produse = new Produs[nrProduse];
        for (int i = 0; i < this->nrProduse; i++)
            this->produse[i] = produse[i];
        this->cantitateProdus = new int[nrProduse];
        for (int i = 0; i < this->nrProduse; i++)
            this->cantitateProdus[i] = cantitateProdus[i];
        this->meniu = meniu;
    }

    Comanda(const Comanda& c) :idComanda(++c.nrComenzi) {
        this->operatiune = c.operatiune;
        this->nrProduse = c.nrProduse;
        produse = new Produs[nrProduse];
        for (int i = 0; i < this->nrProduse; i++)
            this->produse[i] = c.produse[i];
        cantitateProdus = new int[nrProduse];
        for (int i = 0; i < this->nrProduse; i++)
            this->cantitateProdus[i] = c.cantitateProdus[i];
        this->meniu = meniu;
    }

    Comanda operator=(const Comanda& c) {
        if (this != &c) {
            delete[] this->produse;
            this->produse = nullptr;
            delete[] this->cantitateProdus;
            this->cantitateProdus = nullptr;

            this->operatiune = c.operatiune;
            this->nrProduse = c.nrProduse;
            produse = new Produs[this->nrProduse];
            for (int i = 0; i < this->nrProduse; i++)
                this->produse[i] = c.produse[i];
            cantitateProdus = new int[nrProduse];
            for (int i = 0; i < this->nrProduse; i++)
                this->cantitateProdus[i] = c.cantitateProdus[i];
            this->meniu = meniu;
        }
        else {
            delete[] this->produse;
            this->produse = nullptr;
            delete[] this->cantitateProdus;
            this->cantitateProdus = nullptr;
        }
        return *this;
    }


    ~Comanda() {
        delete[] this->produse;
        this->produse = nullptr;
        delete[] this->cantitateProdus;
        this->cantitateProdus = nullptr;
    }
    
    int getNrProduse() const {
        return this->nrProduse;
    }

    Produs* getProduse() const {
        return produse;
    }

    int* getCantitateProdus() const {
        return cantitateProdus;
    }


    friend ostream& operator<<(ostream& os, const Comanda& c) {
        os << "====================" << endl;
        os << "Comanda dvs: ";
         for (int i = 0; i < c.nrProduse; i++)
             os <<c.cantitateProdus[i] <<c.produse[i].getDenumire()<<" ";
        return os;
    }
    friend istream& operator>>(istream& is, Comanda& c) {
        cout << "Ce ati dori?" << endl;
        is >> c.operatiune;
        if (c.operatiune != "CMD")
            throw ExceptieComanda("Operatie invalida");
        is >> c.nrProduse;
        c.produse = new Produs[c.nrProduse];
        c.cantitateProdus = new int[c.nrProduse];
        for (int i = 0; i < c.nrProduse; i++) {
            string denumire;
            is >> denumire;
            is >> c.cantitateProdus[i];

            bool ExistaProdus = false;
            for (int j = 0; j < c.meniu->getNrProduse(); j++) {
                if (denumire == c.meniu->getProduse()[j]->getDenumire()) {
                    c.produse[i] = *c.meniu->getProduse()[j];
                    ExistaProdus = true;
                    break;
                }
            }

            if (!ExistaProdus) {
                cout << "Produsul " << denumire << " nu este in meniu." << endl;
                
            }
        }
        return is;
    }


    Produs& operator[](int index) {
        if (index > 0 && index <= nrProduse)
            return produse[index];
        else {
            throw ExceptieComanda("Comanda cu numarul introdus nu exista");
        }
    }

    Comanda& operator-() { //sterge comanda n

    }

    friend ostream& operator<<(ostream& os, const Comanda& c);
    friend istream& operator>>(istream& is, Comanda& c);
};

class Restaurant {
    string denumire = "N/A";
    programRestaurant programSaptamana[7];
    int nrComenzi=0;
    Retetar* retete=nullptr;
    Comanda* comenzi=nullptr;
    Meniu* meniu=nullptr;

public:

    Restaurant() :comenzi(nullptr) {}

    Restaurant(string denumire,programRestaurant porgramSaptamana[7],int nrComenzi,Retetar* retete,Comanda* comenzi,Meniu* meniu) {
        this->denumire = denumire;
        for (int i = 0; i < 7; i++)
            this->programSaptamana[i]=programSaptamana[i];
        this->nrComenzi=nrComenzi;
        if (comenzi) {
            this->comenzi = new Comanda[nrComenzi];
            for (int i = 0; i < this->nrComenzi; i++)
                this->comenzi[i] = comenzi[i];
        }
        else this->comenzi = nullptr;
        this->retete = retete;
        this->meniu = meniu;

    }

    Restaurant(const Restaurant& r) {
        this->denumire = r.denumire;
        for (int i = 0; i < 7; i++)
            this->programSaptamana[i]=r.programSaptamana[i];
        this->nrComenzi = r.nrComenzi;
        this->retete = r.retete;
        this->meniu = meniu;
        this->comenzi = new Comanda[r.nrComenzi];
        for (int i = 0; i < this->nrComenzi; i++)
            this->comenzi[i] = r.comenzi[i];

    }

    Restaurant operator=(const Restaurant& r) {
        if (this != &r) {
            delete[] this->comenzi;
            this->comenzi = nullptr;
            delete[] this->meniu;
            this->meniu = nullptr;

            this->denumire = r.denumire;
            for (int i = 0; i < 7; i++)
                this->programSaptamana[i] = r.programSaptamana[i];
            this->nrComenzi = r.nrComenzi;
            this->retete = r.retete;
            this->meniu = meniu;
            this->comenzi = new Comanda[r.nrComenzi];
            for (int i = 0; i < this->nrComenzi; i++)
                this->comenzi[i] = r.comenzi[i];
        }
        else {
            delete[] this->comenzi;
            this->comenzi = nullptr;
            delete[] this->meniu;
            this->meniu = nullptr;
        }
        return *this;
    }

    Comanda& operator[](int index) {
        if (index >= 0 && index < nrComenzi) {
            return comenzi[index];
        }
        else {
            cerr << "comanda nu exita"; 
        }
    }

    void setProgram(int ziNoua, const string& ziua, int oraDeschidere, int minuteDeschidere, int oraInchidere, int minuteInchidere) {
        if (ziNoua >= 0 && ziNoua < 7) {
            programSaptamana[ziNoua].ziua = ziua;
            programSaptamana[ziNoua].oraDeschidere.ora = oraDeschidere;
            programSaptamana[ziNoua].oraDeschidere.minute = minuteDeschidere;
            programSaptamana[ziNoua].oraInchidere.ora = oraInchidere;
            programSaptamana[ziNoua].oraInchidere.minute = minuteInchidere;
        }
    }

    void adaugaComanda(const Comanda& comandaNoua) {
        Comanda* comenziNoi=new Comanda[nrComenzi+1];
        for (int i = 0; i < nrComenzi; i++) {
            comenziNoi[i] = comenzi[i];
        }
        comenziNoi[nrComenzi] = comandaNoua;
        delete[] comenzi;
        comenzi = comenziNoi;
        nrComenzi++;
    }

    ~Restaurant() {
        delete[] this->comenzi;
        this->comenzi = nullptr;
    }

    bool amSuficienteIngrediente(Produs& produsComandat, int cantitateProdusComandat) {
        Retetar r = produsComandat.getReteta();
        Ingredient** ingredienteReteta = r.getIngrediente();
        int* cantitatiNecesare = r.getCantitateNecesara();

        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            if (ingredienteReteta[i]) {
                int CantNecesaraTotal = cantitatiNecesare[i] * cantitateProdusComandat;
                if (ingredienteReteta[i]->getCantitate() < CantNecesaraTotal)
                    return false;
            }
        }
        return true;
    }


    void Legenda() {
        cout << "Introdu 'comanda' pt a face o comanda" << endl;
        cout << "Introdu 'meniu' pt a vedea meniul" << endl;
        cout << "Introdu 'iesire' pt a iesi din interfata" << endl;
    }

    void LegendaAdministrator() {
        cout << "Introdu 'cantitate minus' pentru a scadea cantitatea unui produs"<<endl;
        cout << "Introdu 'cantitate plus' pentru a mari cantitatea unui produs" << endl;
        cout << "Introdu 'stoc depozit' pentru a vedea toate ingredientele si cantitatile lor" << endl; 
        cout << "Introdu 'gestiune comenzi' pentru a vizualiza comenzile cerute" << endl;
        cout << "Introdu 'afiseaza comenzi' pentru a afisa toate comenzile" << endl;
    }

    void Interfata(Comanda& c) {
        int ok = 1;
        string input;
        cout << "Bine ati venit!" << endl;
        while (ok == 1) {
            Legenda();
            cin >> input;
            if (input == "iesire")
            {
                ok = 0;
                cout << endl<<"La revedere!"<<endl; break;
            }
            if (input == "comanda") {
                cout <<endl<< "Comanda se face astfel: CMD nrProduse produs1 cantitateProdus1 produs2 Cantitateprodus2 ..." << endl;
                cout << "Ex: CMD 1 pizza 1" << endl;
                cin >> c;
                bool comandaPosibila = true;
                for (int i = 0; i < c.getNrProduse(); i++) {
                    Produs produsComandat = c.getProduse()[i];
                    int cantitateProdusComandat = c.getCantitateProdus()[i];
                    if (!amSuficienteIngrediente(produsComandat, cantitateProdusComandat)) {
                        comandaPosibila = false;
                        cout << "Comanda nu poate fi realizata, nu avem suficiente ingrediente pentru a realiza " << cantitateProdusComandat << " " << produsComandat.getDenumire() << endl;
                        break;
                    }
                }
                if (comandaPosibila) {
                    for (int i = 0; i < c.getNrProduse(); i++) {
                        int cantitateProdusComandat = c.getCantitateProdus()[i];
                        Produs produsComandat = c.getProduse()[i];
                        Retetar reteta = produsComandat.getReteta();
                        Ingredient** ingredienteComanda = reteta.getIngrediente();
                        int* cantitatiNecesare = reteta.getCantitateNecesara();
                        for (int j = 0; j < Ingredient::getIdFinal(); j++) {
                            if (ingredienteComanda[j]) {
                                ingredienteComanda[j]->operator-=(cantitatiNecesare[j] * cantitateProdusComandat);
                            }
                        }
                    }
                    adaugaComanda(c);
                    cout << "Comanda efectuata cu succes" << endl;
                }

                cout << endl;
            }

            if (input == "meniu") {
                if (meniu != nullptr) {
                    cout << "Meniul restaurantului:" << endl;
                    for (int i = 0; i < meniu->getNrProduse(); i++) {
                        Produs* produs = meniu->getProduse()[i];
                        cout << produs->getDenumire() << " " << meniu->getPreturi()[i] << " lei" << endl;
                    }
                    cout << endl;
                }
                else cout << "Meniul nu e disponibil" << endl;
            }

            if (input == "administrator") {
                cout << endl;
                LegendaAdministrator();
                string inputAdmin;
                cin.ignore();
                getline(cin, inputAdmin);
                if (inputAdmin == "cantitate minus") {
                    bool gasit = false;
                    string denumireProdus;
                    int cantitateScazuta;
                    cout << "Introduceti numele produsului pe care doriti sa-l modificati: ";
                    cin >> denumireProdus;
                    cout << "Introduceti cantitatea cu care vreti sa scadeti stocul produsului: ";
                    cin >> cantitateScazuta;
                    for (int i = 0; i < Ingredient::getIdFinal(); i++) {
                        if (strcmp(retete->getIngrediente()[i]->getDenumire(), denumireProdus.c_str()) == 0) {
                            gasit = true;
                            *retete->getIngrediente()[i] -= cantitateScazuta;
                            cout << "Cantitatea a fost actualizata" << endl;
                            cout << "Noua cantitate este: " << retete->getIngrediente()[i]->getCantitate() << endl << endl; break;
                        }
                    }
                    if (gasit == false) {
                        cout << "Produsul introdus nu exista in restaurant" << endl << endl;
                    }
                }
                if (inputAdmin == "cantitate plus") {
                    bool gasit = false;
                    string denumireProdus;
                    int cantitateAdaugata;
                    cout << "Introduceti numele produsului pe care doriti sa-l modificati: ";
                    cin >> denumireProdus;
                    cout << "Introduceti cantitatea cu care vreti sa incrementati stocul produsul: ";
                    cin >> cantitateAdaugata;
                    for (int i = 0; i < Ingredient::getIdFinal(); i++) {
                        if (strcmp(retete->getIngrediente()[i]->getDenumire(), denumireProdus.c_str())==0) {
                            gasit = true;
                            *retete->getIngrediente()[i] += cantitateAdaugata;
                            cout << "Cantitatea a fost actualizata"<<endl;
                            cout << "Noua cantitate este: " << retete->getIngrediente()[i]->getCantitate() << endl<<endl; break;
                        }
                    }
                    if (gasit==false) {
                        cout << "Produsul introdus nu exista in restaurant"<<endl<<endl;
                    }
                }
                if (inputAdmin == "stoc depozit") {
                    if (this->retete != nullptr) {
                        cout << "Stocul disponibil: " << endl;
                        for (int i = 0; Ingredient::getIdFinal(); i++) {
                            if (this->retete->getIngrediente()[i] != nullptr) {
                                cout << "Ingredient: " << retete->getIngrediente()[i]->getDenumire() << "  Cantiate: " << retete->getIngrediente()[i]->getCantitate() << endl;
                            }
                        }
                        }
                    else {
                        cout << "Depozitul este gol" << endl;
                    }
                }

                if (inputAdmin == "gestiune comenzi") {
                    cout << "Ce comanda doriti sa vizualizati? ";
                    int n; cin >> n;
                    if (n >= 0 && n < nrComenzi) {
                        Comanda& comandaGestiune = comenzi[n];
                        cout << comandaGestiune<<endl<<endl;
                    }
                    else cout << "nu exista comanda cu numarul cerut";
                }
                if (inputAdmin == "afiseaza comenzi") {
                    for (int i = 0; i < nrComenzi;i++) {
                        Comanda& comandaGestiune = comenzi[i];
                        cout << comandaGestiune << endl;
                    }
                    cout << endl;
                }

            }
            /*else cout << "Comanda invalida" << endl;*/

        }
    }

    friend ostream& operator<<(ostream& os, const Restaurant& r) {
        os << r.denumire<<endl;
        os << "Program: " << endl;
        for (int i = 0; i < 7; i++) {
                os << r.programSaptamana[i].ziua<<" ";
            if (r.programSaptamana[i].oraDeschidere.ora < 10)
                os << "0";
                os << r.programSaptamana[i].oraDeschidere.ora << ":";
            if (r.programSaptamana[i].oraDeschidere.minute < 10)
                os << "0";
                os<< r.programSaptamana[i].oraDeschidere.minute << "-";
            if (r.programSaptamana[i].oraInchidere.ora < 10)
                os << "0";
                os << r.programSaptamana[i].oraInchidere.ora << ":";
            if (r.programSaptamana[i].oraInchidere.minute < 10)
                os << "0";
                os<< r.programSaptamana[i].oraInchidere.minute << endl; 
        }
        return os;
    }

    friend istream& operator>>(istream& is, Restaurant& r) {
        return is;
    }

    friend ostream& operator<<(ostream& os, const Restaurant& r);
    friend istream& operator>>(istream& is, Restaurant& r);

};

int Comanda::nrComenzi = 0;
int Ingredient::idCurent = 0;

int main()
{
    Ingredient ingredient1("rosii", 30);
    Ingredient ingredient2("salam", 20);
    Ingredient ingredient3("ardei", 30);
    Ingredient ingredient4("faina", 1000); 
    Ingredient ingredient5("mozarella", 100);
    Ingredient ingredient6("bacon", 30);
    Ingredient ingredient7("ciuperci", 50); 
    Ingredient* ingrediente[] = { &ingredient1, &ingredient2, &ingredient3,&ingredient4,&ingredient5,&ingredient6,&ingredient7}; 
    int cantitati1[] = {2,5,3,100,10,0,3};
    int cantitati2[] = {5,0,0,150,20,10,0};
    Retetar retetaPizza(ingrediente, cantitati1); 
    Retetar retetaPaste(ingrediente, cantitati2); 
    Produs Pizza("Pizza", &retetaPizza, true);
    Produs Paste("Paste",&retetaPaste,true);
    //cout << Ingredient::getIdFinal() << endl;
    Retetar retetar(ingrediente, cantitati1);
    Ingredient** retetarIngrediente = retetar.getIngrediente();
    //cout << "Retetar:" << endl;
    //cout << retetar;
    //cin >> retetar;
    //cout << retetar;
    //cout << Pizza;
    //cout << endl;
    //cout << Pizza.getReteta() << endl << endl << endl;
    //cout << Paste.getReteta() << endl << endl << endl;
    Produs* produse[] = {&Pizza,&Paste};
    for (int i = 0; i < 2; i++)
        cout << produse[i]->getReteta();
    cout << endl;
    int preturi[] = { 50,35 };
    Meniu meniu(produse, preturi, 2);
    Produs produseComanda[] = {Pizza};
    int cantitatiComanda[] = { 1 };
    Comanda c(1,"CMD",1,cantitatiComanda,produseComanda,&meniu);
    programRestaurant program[7];
    int nrComenzi = 0;
    Comanda* comenzi = nullptr;
    Retetar* retete = new Retetar(ingrediente, cantitati1);
    Restaurant restaurant1("Restaurant Bucuresti", program, nrComenzi, retete, comenzi, &meniu);
    restaurant1.setProgram(0, "Luni", 9, 0, 22, 0);
    restaurant1.setProgram(1, "Marti", 9, 0, 22, 0);
    restaurant1.setProgram(2, "Miercuri", 9, 0, 22, 0);
    restaurant1.setProgram(3, "Joi", 9, 0, 22, 0);
    restaurant1.setProgram(4, "Vineri", 10, 0, 22, 0);
    restaurant1.setProgram(5, "Sambata", 10, 0, 20, 0);
    restaurant1.setProgram(6, "Duminica", 10, 0, 20, 0);
    cout<<restaurant1<<endl;
    restaurant1.Interfata(c);
    return 0;
}
