#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include "Ingredient.h"
//#include "Retetar.h"
//#include "Produs.h"
//#include "Meniu.h"
//#include "Comanda.h"
#include <vector>
#include <map>
#include <set>
#include <utility>
using namespace std;

class ISerializabil {
public:
    virtual void scrieInBinar(ofstream& out) = 0;
    virtual void citesteDinBinar(ifstream& in) = 0;
    virtual ~ISerializabil() {}
};

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

class Ingredient : public ISerializabil {
    static int idCurent;
    const int id;
    char* denumire = nullptr;
    int cantitate = 0;

public:
    void scrieInBinar(ofstream& out) override {
        int lungimeDenumire = strlen(denumire) + 1;
        out.write(reinterpret_cast<const char*>(&lungimeDenumire), sizeof(lungimeDenumire));
        out.write(denumire, lungimeDenumire);
        out.write(reinterpret_cast<const char*>(&cantitate), sizeof(cantitate));
    }

    void citesteDinBinar(ifstream& in) override {
        int lungimeDenumire = 0;
        in.read(reinterpret_cast<char*>(&lungimeDenumire), sizeof(lungimeDenumire));
        delete[] denumire;
        denumire = new char[lungimeDenumire];
        in.read(denumire, lungimeDenumire);
        in.read(reinterpret_cast<char*>(&cantitate), sizeof(cantitate));
    }

    Ingredient(const int id) :id(++Ingredient::idCurent) {}

    Ingredient() :id(++Ingredient::idCurent), denumire(nullptr), cantitate(0) {}

    Ingredient(const char* denumire, int cantitate) :id(++Ingredient::idCurent) {
        this->denumire = new char[strlen(denumire) + 1];
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

    void setCantitate(int nouaCantitate) {
        if (nouaCantitate >= 0) {
            cantitate = nouaCantitate;
        }
        else {
            cantitate = 0;
        }
    }

    char* getDenumire() const {
        return denumire;
    }

    friend ostream& operator<<(ostream& os, const Ingredient& i) {
        os << "=======================" << endl;
        os << "ID: " << i.id << endl;
        os << "Denumire: ";
        if (i.denumire != nullptr) {
            os << i.denumire;
        }
        else {
            os << "-";
        }
        os << endl;
        os << "Cantitate: " << i.cantitate;
        os << endl;
        return os;

    }

    friend istream& operator>>(istream& is, Ingredient& i) {
        cout << "Introduceti denumirea ingredientului: ";
        string buffer;
        is >> buffer;
        i.denumire = new char[buffer.size() + 1];
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

class Retetar : ISerializabil {
    map<string, int> ingredienteNecesare;
    map<string, vector<pair<string, int>>> retete;
    Ingredient** ingrediente;
    int* cantitateNecesara = nullptr;
    int nrIngrediente = 0;

public:

    void scrieInBinar(ofstream& out) override {
        int numIngredients = ingredienteNecesare.size();
        out.write(reinterpret_cast<const char*>(&numIngredients), sizeof(numIngredients));
        for (const auto& pair : ingredienteNecesare) {
            int len = pair.first.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(pair.first.c_str(), len);
            out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }
    }

    void citesteDinBinar(ifstream& in) override {
        int numIngredients;
        in.read(reinterpret_cast<char*>(&numIngredients), sizeof(numIngredients));
        ingredienteNecesare.clear();
        for (int i = 0; i < numIngredients; ++i) {
            int len;
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            string name(len, '\0');
            in.read(&name[0], len);
            int cantitate;
            in.read(reinterpret_cast<char*>(&cantitate), sizeof(cantitate));
            ingredienteNecesare[name] = cantitate;
        }
    }

    Retetar(const map<string, vector<pair<string, int>>>& reteteMap) {
        for (const auto& pereche : reteteMap) {
            retete[pereche.first] = pereche.second;
        }
    }

    Retetar(const Retetar& r) {
        for (const auto& ingredient : r.ingredienteNecesare) {
            string numeIngredient = ingredient.first;
            int cantitate = ingredient.second;
            this->ingredienteNecesare[numeIngredient] = cantitate;
        }
    }

    Retetar(const vector<pair<string, int>>& ingredienteNecesare) {
        for (const auto& ingredient : ingredienteNecesare) {
            string numeIngredient = ingredient.first;
            int cantitate = ingredient.second;
            this->ingredienteNecesare[numeIngredient] = cantitate;
        }
    }

    Retetar(Ingredient** ingredienteParametru, int* cantitateNecesara, int nrIngrediente) {
        this->ingrediente = new Ingredient * [Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            this->ingrediente[i] = ingredienteParametru[i];
        }
        this->cantitateNecesara = new int[Ingredient::getIdFinal()];
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            this->cantitateNecesara[i] = cantitateNecesara[i];
        }
        this->nrIngrediente = nrIngrediente;
    }

    /*  Retetar(const Retetar& r) {
              this->ingrediente = new Ingredient * [Ingredient::getIdFinal()];
              for (int i = 0; i < Ingredient::getIdFinal(); i++)
                  this->ingrediente[i] = r.ingrediente[i];
              this->cantitateNecesara = new int[Ingredient::getIdFinal()];
              for (int i = 0; i < Ingredient::getIdFinal(); i++)
                  this->cantitateNecesara[i] = r.cantitateNecesara[i];
              this->nrIngrediente = r.nrIngrediente;
      }*/

    Retetar operator=(const Retetar& r) {
        if (this != &r) {
            for (int i = 0; i < Ingredient::getIdFinal(); i++) {
                delete this->ingrediente[i];
            }
            delete[] this->ingrediente;
            this->ingrediente = nullptr;

            this->ingrediente = new Ingredient * [Ingredient::getIdFinal()];
            this->cantitateNecesara = new int[Ingredient::getIdFinal()];
            for (int i = 0; i < Ingredient::getIdFinal(); i++)
                this->ingrediente[i] = r.ingrediente[i];
            for (int i = 0; i < Ingredient::getIdFinal(); i++)
                this->cantitateNecesara[i] = r.cantitateNecesara[i];
            this->nrIngrediente = r.nrIngrediente;
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

    const map<string, int>& getIngredienteNecesare() const {
        return ingredienteNecesare;
    }

    vector<pair<string, int>> getReteta(const string& numeProdus) const {
        auto it = retete.find(numeProdus);
        if (it != retete.end()) {
            // Dacă produsul există în retetar, returnează reteta asociată
            return it->second;
        }
        else {
            // Dacă produsul nu există, returnează un vector gol sau aruncă o excepție
            return vector<pair<string, int>>();
        }
    }

    Ingredient** getIngrediente() {
        return ingrediente;
    }

    ~Retetar() {
        delete[] this->ingrediente;
        this->ingrediente = nullptr;
        delete[] this->cantitateNecesara;
        this->cantitateNecesara = nullptr;
    }

    int* getCantitateNecesara() {
        return cantitateNecesara;
    }

    int getNrIngrediente() {
        return nrIngrediente;
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
            os << r.ingrediente[i]->getDenumire() << " ";
        os << endl;
        os << "Cantitate necesara: ";
        for (int i = 0; i < Ingredient::getIdFinal(); i++)
            os << r.cantitateNecesara[i] << " ";
        os << endl;
        return os;
    }

    friend istream& operator>>(istream& is, Retetar& r) {
        cout << "Introduceti cantitatea necesara: " << endl;
        for (int i = 0; i < Ingredient::getIdFinal(); i++) {
            int cantitate;
            cout << r.ingrediente[i]->getDenumire() << " ";
            is >> cantitate;
            r.cantitateNecesara[i] = cantitate;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Retetar& r);
    friend istream& operator>>(istream& is, Retetar& r);
};

class Produs : public ISerializabil {
    string denumire = "N/A";
    Retetar* reteta = nullptr;
    bool esteDisponibil = 1;

public:
    void scrieInBinar(ofstream& out)  override {
        int lungimeDenumire = denumire.size();
        out.write(reinterpret_cast<const char*>(&lungimeDenumire), sizeof(lungimeDenumire));
        out.write(denumire.c_str(), lungimeDenumire);
        out.write(reinterpret_cast<const char*>(&esteDisponibil), sizeof(esteDisponibil));
        if (reteta != nullptr) {
            reteta->scrieInBinar(out);
        }
    }

    void citesteDinBinar(ifstream& in) override {
        int lungimeDenumire;
        in.read(reinterpret_cast<char*>(&lungimeDenumire), sizeof(lungimeDenumire));
        denumire.resize(lungimeDenumire);
        in.read(&denumire[0], lungimeDenumire);
        in.read(reinterpret_cast<char*>(&esteDisponibil), sizeof(esteDisponibil));
        if (reteta == nullptr) {
            reteta = new Retetar(0, 0, 0);
        }
        reteta->citesteDinBinar(in);
    }

    Produs() {}

    Produs(string denumire, Retetar* reteta, bool esteDisponibil) {
        this->denumire = denumire;
        this->reteta = reteta;
        this->esteDisponibil = esteDisponibil;
    }

    Produs(string denumire, const vector<pair<string, int>>& retetaVector, bool esteDisponibil)
        : denumire(denumire), esteDisponibil(esteDisponibil) {
        this->reteta = new Retetar(retetaVector);
    }

    Produs(const Produs& p) {
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

    string getDenumire() const {
        return denumire;
    }

    Retetar getReteta() const {
        return *reteta;
    }


    friend ostream& operator<<(ostream& os, const Produs& p) {
        os << "Denumire produs: " << p.denumire << endl;
        os << "Reteta produs: ";
        os << *(p.reteta);
        os << "Disponibilitate produs: " << p.esteDisponibil << endl;
        return os;
    }

    friend istream& operator>>(istream& is, Produs& p) {
        cout << "Denumire produs: ";
        is >> p.denumire;
        cout << endl;
        cout << "Disponibilitate produs: ";
        is >> p.esteDisponibil;
        cout << endl;
        return is;
    }

    bool operator<(const Produs& altProdus) const {
        return this->denumire < altProdus.denumire;
    }

    friend ostream& operator<<(ostream& os, const Produs& p);
    friend istream& operator>>(istream& is, Produs& p);
};

class Pizza : public Produs {

};

class Paste : public Produs {

};

class Meniu :public ISerializabil {
    std::set<pair<Produs, int>> produseCuPreturi;

    Produs** produse;
    int nrProduse = 0;
    int* preturi = nullptr;
public:
    void scrieInBinar(ofstream& out) override {
        int numProduse = produseCuPreturi.size();
        out.write(reinterpret_cast<const char*>(&numProduse), sizeof(numProduse));
        for (auto pair : produseCuPreturi) {
            pair.first.scrieInBinar(out);
            out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }
    }

    void citesteDinBinar(ifstream& in) override {
        int numProduse;
        in.read(reinterpret_cast<char*>(&numProduse), sizeof(numProduse));
        produseCuPreturi.clear();
        for (int i = 0; i < numProduse; ++i) {
            Produs p;
            p.citesteDinBinar(in);
            int pret;
            in.read(reinterpret_cast<char*>(&pret), sizeof(pret));
            produseCuPreturi.insert(make_pair(p, pret));
        }
    }

    Meniu(const set<pair<string, int>>& meniuSet, const Retetar& retetar) {
        for (const auto& pereche : meniuSet) {
            string numeProdus = pereche.first;
            int pret = pereche.second;
            auto reteta = retetar.getReteta(numeProdus);

            Produs produs(numeProdus, reteta, 1);
            produseCuPreturi.insert(make_pair(produs, pret));
        }
    }

    Meniu(Produs** produse, int* preturi, int nrProduse) {
        this->nrProduse = nrProduse;
        this->produse = new Produs * [nrProduse];
        this->preturi = new int[nrProduse];
        for (int i = 0; i < this->nrProduse; i++) {
            this->produse[i] = produse[i];
            this->preturi[i] = preturi[i];
        }
    }


    int getNrProduse() {
        return this->nrProduse;
    }

    Produs** getProduse() {
        return produse;
    }

    int* getPreturi() {
        return preturi;
    }

    const Produs* cautaProdus(const string& numeProdus) const {
        for (const auto& pereche : produseCuPreturi) {
            if (pereche.first.getDenumire() == numeProdus) {
                return &pereche.first;
            }
        }
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

    Meniu& operator=(const Meniu& m) {
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
    std::vector<std::pair<std::string, int>> produseComandate;
    static int nrComenzi;
    const int idComanda;
    string operatiune = "N/A";
    int nrProduse = 0;
    int* cantitateProdus = nullptr;
    Produs* produse = nullptr;
    Meniu* meniu = nullptr;
public:
    void adaugaProdus(const std::string& numeProdus, int cantitate) {
        produseComandate.push_back(std::make_pair(numeProdus, cantitate));
    }

    Comanda() :idComanda(++nrComenzi) {
    }

    Comanda(Meniu* meniu) : idComanda(++nrComenzi), meniu(meniu) {}

    Comanda(const int idComanda, string operatiune, int nrProduse, int* cantitateProdus, Produs* produse, Meniu* meniu) :idComanda(++nrComenzi) {
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
            os << c.cantitateProdus[i] << c.produse[i].getDenumire() << " ";
        return os;
    }

    friend istream& operator>>(istream& is, Comanda& c) {
        cout << "Ce ati dori?" << endl;

        // Citirea întregii linii pentru comandă
        string linieComanda;
        getline(is >> std::ws, linieComanda);

        istringstream iss(linieComanda);

        iss >> c.operatiune;
        if (c.operatiune != "CMD") {
            throw ExceptieComanda("Operatie invalida");
        }

        iss >> c.nrProduse;
        c.produse = new Produs[c.nrProduse];
        c.cantitateProdus = new int[c.nrProduse];

        for (int i = 0; i < c.nrProduse; i++) {
            std::string denumireProdus;
            iss >> denumireProdus;
            string restLinie;
            getline(iss, restLinie);
            istringstream issRest(restLinie);
            string cuvant;
            while (issRest >> cuvant) {
                if (isdigit(cuvant[0])) {
                    c.cantitateProdus[i] = std::stoi(cuvant);
                    break;
                }
                else {
                    denumireProdus += " " + cuvant;
                }
            }
            if (c.meniu != nullptr) {
                const Produs* produsMeniu = c.meniu->cautaProdus(denumireProdus);
                if (produsMeniu != nullptr) {
                    c.produse[i] = *produsMeniu;
                }
                else {
                    cout << "Produsul " << denumireProdus << " nu este disponibil in meniu." << endl;
                }
            }
            else {
                cout << "Meniul nu este disponibil." << endl;
            }
        }
        return is;
    }
    void scrieInBinar(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&idComanda), sizeof(idComanda)); 
        out.write(reinterpret_cast<const char*>(&nrProduse), sizeof(nrProduse)); 
        for (int i = 0; i < nrProduse; ++i) {
            int len = produse[i].getDenumire().size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len)); 
            out.write(produse[i].getDenumire().c_str(), len); 
            out.write(reinterpret_cast<const char*>(&cantitateProdus[i]), sizeof(cantitateProdus[i])); 
        }
    }

    void scrieComandaInBinar(const Comanda& comanda) {
        ofstream fisierComenzi("comenzi.bin", ios::binary | ios::app);
        if (!fisierComenzi) {
            cerr << "Eroare la deschiderea fisierului 'comenzi.bin' pentru scriere." << endl;
            return;
        }

        fisierComenzi.write(reinterpret_cast<const char*>(&comanda.idComanda), sizeof(comanda.idComanda));
        fisierComenzi.write(reinterpret_cast<const char*>(&comanda.nrProduse), sizeof(comanda.nrProduse));

        for (int i = 0; i < comanda.nrProduse; ++i) {
            const Produs& produs = comanda.produse[i];
            int lenDenumire = produs.getDenumire().size();
            fisierComenzi.write(reinterpret_cast<const char*>(&lenDenumire), sizeof(lenDenumire));
            fisierComenzi.write(produs.getDenumire().c_str(), lenDenumire);
            fisierComenzi.write(reinterpret_cast<const char*>(&comanda.cantitateProdus[i]), sizeof(comanda.cantitateProdus[i]));
        }

        if (!fisierComenzi) {
            cerr << "Eroare la scrierea in fisierul 'comenzi.bin'." << endl;
        }

        fisierComenzi.close();
    }

    void raportProdusulCelMaiComandat() {
        ifstream fisierComenzi("comenzi.bin", ios::binary);
        if (!fisierComenzi) {
            cerr << "Eroare la deschiderea fisierului 'comenzi.bin' pentru citire." << endl;
            return;
        }

        map<string, int> frecventaProduse;

        while (fisierComenzi) {
            int idComanda, nrProduse;
            fisierComenzi.read(reinterpret_cast<char*>(&idComanda), sizeof(idComanda));
            fisierComenzi.read(reinterpret_cast<char*>(&nrProduse), sizeof(nrProduse));

            for (int i = 0; i < nrProduse; ++i) {
                int lenDenumire;
                fisierComenzi.read(reinterpret_cast<char*>(&lenDenumire), sizeof(lenDenumire));
                string denumire(lenDenumire, '\0');
                fisierComenzi.read(&denumire[0], lenDenumire);
                int cantitate;
                fisierComenzi.read(reinterpret_cast<char*>(&cantitate), sizeof(cantitate));

                frecventaProduse[denumire] += cantitate;
            }

            if (!fisierComenzi) break;
        }

        fisierComenzi.close();

        string produsCelMaiComandat;
        int maxComenzi = 0;
        for (const auto& pereche : frecventaProduse) {
            if (pereche.second > maxComenzi) {
                maxComenzi = pereche.second;
                produsCelMaiComandat = pereche.first;
            }
        }

        cout << "Produsul cel mai comandat: " << produsCelMaiComandat << " (comandat de " << maxComenzi << " ori)" << endl;
    }

    void raportCantitatiPerProdus() {
        ifstream fisierComenzi("comenzi.bin", ios::binary);
        if (!fisierComenzi) {
            cerr << "Eroare la deschiderea fisierului 'comenzi.bin' pentru citire." << endl;
            return;
        }

        map<string, int> cantitatiProduse;

        while (fisierComenzi) {
            int idComanda, nrProduse;
            fisierComenzi.read(reinterpret_cast<char*>(&idComanda), sizeof(idComanda));
            fisierComenzi.read(reinterpret_cast<char*>(&nrProduse), sizeof(nrProduse));

            for (int i = 0; i < nrProduse; ++i) {
                int lenDenumire;
                fisierComenzi.read(reinterpret_cast<char*>(&lenDenumire), sizeof(lenDenumire));
                string denumire(lenDenumire, '\0');
                fisierComenzi.read(&denumire[0], lenDenumire);
                int cantitate;
                fisierComenzi.read(reinterpret_cast<char*>(&cantitate), sizeof(cantitate));

                cantitatiProduse[denumire] += cantitate;
            }

            if (!fisierComenzi) break;
        }

        fisierComenzi.close();

        for (const auto& pereche : cantitatiProduse) {
            if (!pereche.first.empty()) {
                cout << pereche.first << ": " << pereche.second << " unitati" << endl;
            }
        }
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
    int nrComenzi = 0;
    Retetar* retete = nullptr;
    Comanda* comenzi = nullptr;
    Meniu* meniu = nullptr;

public:

    Restaurant() :comenzi(nullptr) {}

    Restaurant(const string numeRestaurant, programRestaurant programSaptamana[7], map<string, std::vector<pair<string, int>>>& retetarMap, set<pair<string, int>>& meniuSet) {
        this->denumire = numeRestaurant;
        // Creăm un obiect Retetar din retetarMap
        this->retete = new Retetar(retetarMap);
        // Creăm un obiect Meniu din meniuSet
        this->meniu = new Meniu(meniuSet, *this->retete);
        for (int i = 0; i < 7; i++) {
            this->programSaptamana[i] = programSaptamana[i];
        }
    }

    Restaurant(const string denumire, programRestaurant porgramSaptamana[7], int nrComenzi, Retetar* retete, Comanda* comenzi, Meniu* meniu) {
        this->denumire = denumire;
        for (int i = 0; i < 7; i++)
            this->programSaptamana[i] = programSaptamana[i];
        this->nrComenzi = nrComenzi;
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
            this->programSaptamana[i] = r.programSaptamana[i];
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

    Meniu* getMeniu() {
        return meniu;
    }

    Retetar* getRetetar() const {
        return retete;
    }

    void adaugaComanda(const Comanda& comandaNoua) {
        Comanda* comenziNoi = new Comanda[nrComenzi + 1];
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

    friend ostream& operator<<(ostream& os, const Restaurant& r) {
        os << r.denumire << endl;
        os << "Program: " << endl;
        for (int i = 0; i < 7; i++) {
            os << r.programSaptamana[i].ziua << " ";
            if (r.programSaptamana[i].oraDeschidere.ora < 10)
                os << "0";
            os << r.programSaptamana[i].oraDeschidere.ora << ":";
            if (r.programSaptamana[i].oraDeschidere.minute < 10)
                os << "0";
            os << r.programSaptamana[i].oraDeschidere.minute << "-";
            if (r.programSaptamana[i].oraInchidere.ora < 10)
                os << "0";
            os << r.programSaptamana[i].oraInchidere.ora << ":";
            if (r.programSaptamana[i].oraInchidere.minute < 10)
                os << "0";
            os << r.programSaptamana[i].oraInchidere.minute << endl;
        }
        return os;
    }

    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (string::npos == first) {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    bool verificaIngrediente(const std::vector<Produs>& produseComandate, const std::vector<Ingredient>& ingrediente, const int* cantitati) {
        for (size_t i = 0; i < produseComandate.size(); ++i) {
            const Produs& produs = produseComandate[i];
            int cantitateComandata = cantitati[i]; // Cantitatea comandată pentru fiecare produs

            Retetar reteta = produs.getReteta();
            for (const auto& ingredientReteta : reteta.getIngredienteNecesare()) {
                int cantitateNecesara = ingredientReteta.second * cantitateComandata; // Cantitatea necesară ajustată conform cantității comandate

                auto it = find_if(ingrediente.begin(), ingrediente.end(), [&](const Ingredient& ingr) {
                    return trim(ingr.getDenumire()) == trim(ingredientReteta.first);
                    });

                if (it == ingrediente.end() || it->getCantitate() < cantitateNecesara) {
                    cout << "Nu avem suficient: " << ingredientReteta.first << endl;
                    return false; // Ingredient insuficient sau lipsă
                }
            }
        }
        return true;
    }



    void actualizeazaStoc(std::vector<Ingredient>& ingrediente, const std::vector<Produs>& produseComandate, const int* cantitateProdus) {
        for (size_t i = 0; i < produseComandate.size(); ++i) {
            const Produs& produs = produseComandate[i];
            int cantitateComandata = cantitateProdus[i]; // Cantitatea comandată pentru fiecare produs

            Retetar reteta = produs.getReteta();
            for (const auto& ingredientReteta : reteta.getIngredienteNecesare()) {
                int cantitateNecesara = ingredientReteta.second * cantitateComandata; // Cantitatea necesară ajustată conform cantității comandate

                auto it = find_if(ingrediente.begin(), ingrediente.end(), [&](Ingredient& ingr) {
                    return trim(ingr.getDenumire()) == trim(ingredientReteta.first);
                    });

                if (it != ingrediente.end()) {
                    it->operator-=(cantitateNecesara);
                }
            }
        }
    }



    friend istream& operator>>(istream& is, Restaurant& r) {
        return is;
    }

    friend ostream& operator<<(ostream& os, const Restaurant& r);
    friend istream& operator>>(istream& is, Restaurant& r);

};

int Comanda::nrComenzi = 0;
int Ingredient::idCurent = 0;

class ManagerRestaurant {
public:
    void procesareStoc(const string& locatieFisier) {
        ifstream file(locatieFisier);
        string linie;
        while (getline(file, linie)) {
            istringstream iss(linie);
        }   // procesam fiecare linie
    }
    void procesareMeniu(const string& locatieFisier) {
        ifstream file(locatieFisier);
        string linie;
        while (getline(file, linie)) {
            istringstream iss(linie);
        }   // procesam fiecare linie
    }
    void afisareStoc(const string& locatieFisier) {
        ifstream file(locatieFisier);
        string linie;
        while (getline(file, linie)) {
            cout << linie << endl;
        }
        cout << endl;
    }
    void afisareMeniu(const string& locatieFisier) {
        ifstream file(locatieFisier);
        string linie;
        while (getline(file, linie)) {
            cout << linie << endl;
        }
        cout << endl;
    }

};

// procesare fisiere
void procesareStoc(const string& locatieFisier, vector<Ingredient>& ingrediente) {
    ifstream file(locatieFisier);
    string linie;
    while (getline(file, linie)) {
        stringstream ss(linie);
        string denumire;
        int cantitate;
        getline(ss, denumire, ',');
        ss >> cantitate;
        ingrediente.push_back(Ingredient(denumire.c_str(), cantitate));

    }
}

void procesareRetetar(const string& filename, map<string, vector<pair<string, int>>>& retetar) {
    ifstream file(filename);
    if (!file) {
        cerr << "Nu s-a putut deschide fisierul: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string retetaName;
        getline(ss, retetaName, ':');

        vector<pair<string, int>> ingrediente;
        string ingredientData;
        while (getline(ss, ingredientData, ';')) {
            stringstream iss(ingredientData);
            string ingredientName;
            int cantitate;
            getline(iss, ingredientName, ',');
            iss >> cantitate;
            ingrediente.push_back(make_pair(ingredientName, cantitate));
        }
        retetar[retetaName] = ingrediente;
    }
}

void procesareMeniu(const string& filename, set<pair<string, int>>& meniu) {
    ifstream file(filename);
    if (!file) {
        cerr << "Nu s-a putut deschide fisierul: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string produsName;
        int pret;
        getline(ss, produsName, ',');
        ss >> pret;
        meniu.insert(make_pair(produsName, pret));
    }
}

// serializare si deserializare
void scrieIngredientInFisierBinar(vector<Ingredient>& ingrediente) {
    ofstream fisierBinar("ingrediente.bin", ios::binary | ios::out);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pt scriere" << endl;
        return;
    }

    for (auto& ingredient : ingrediente) {
        ingredient.scrieInBinar(fisierBinar);
    }

    fisierBinar.close();
}

void citesteIngredientDinFisierBinar(vector<Ingredient>& ingrediente) {
    ifstream fisierBinar("ingrediente.bin", ios::binary | ios::in);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pentru citire" << endl;
        return;
    }

    while (!fisierBinar.eof()) {
        Ingredient ingredient;
        ingredient.citesteDinBinar(fisierBinar);
        if (fisierBinar.eof()) break; // Verifică sfârșitul fișierului după citire
        ingrediente.push_back(ingredient);
    }

    fisierBinar.close();
}

void scrieMeniuInFisierBinar(const string& numeFisier, const set<pair<string, int>>& meniu) {
    ofstream fisierBinar(numeFisier, ios::binary | ios::out);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pentru scriere: " << numeFisier << endl;
        return;
    }

    // Scriere număr produse în meniu
    int numarProduse = meniu.size();
    fisierBinar.write(reinterpret_cast<const char*>(&numarProduse), sizeof(numarProduse));

    for (const auto& pereche : meniu) {
        // Scriere lungime nume produs și nume
        int lungimeNumeProdus = pereche.first.size();
        fisierBinar.write(reinterpret_cast<const char*>(&lungimeNumeProdus), sizeof(lungimeNumeProdus));
        fisierBinar.write(pereche.first.c_str(), lungimeNumeProdus);

        // Scriere preț
        fisierBinar.write(reinterpret_cast<const char*>(&pereche.second), sizeof(pereche.second));
    }

    fisierBinar.close();
}

void citesteMeniuDinFisierBinar(const string& numeFisier, set<pair<string, int>>& meniu) {
    ifstream fisierBinar(numeFisier, ios::binary | ios::in);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pentru citire: " << numeFisier << endl;
        return;
    }

    // Citire număr produse în meniu
    int numarProduse = 0;
    fisierBinar.read(reinterpret_cast<char*>(&numarProduse), sizeof(numarProduse));

    for (int i = 0; i < numarProduse; ++i) {
        // Citire lungime nume produs și nume
        int lungimeNumeProdus;
        fisierBinar.read(reinterpret_cast<char*>(&lungimeNumeProdus), sizeof(lungimeNumeProdus));
        string numeProdus(lungimeNumeProdus, '\0');
        fisierBinar.read(&numeProdus[0], lungimeNumeProdus);

        // Citire preț
        int pret;
        fisierBinar.read(reinterpret_cast<char*>(&pret), sizeof(pret));

        meniu.insert(make_pair(numeProdus, pret));
    }

    fisierBinar.close();
}

void scrieRetetarInFisierBinar(const string& numeFisier, map<string, vector<pair<string, int>>>& retetar) {
    ofstream fisierBinar(numeFisier, ios::binary | ios::out);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pentru scriere: " << numeFisier << endl;
        return;
    }

    // Scrie numărul de rețete
    int numarRetete = retetar.size();
    fisierBinar.write(reinterpret_cast<const char*>(&numarRetete), sizeof(numarRetete));

    for (auto& reteta : retetar) {
        // Scrie lungimea numelui rețetei și numele
        int lungimeNume = reteta.first.size();
        fisierBinar.write(reinterpret_cast<const char*>(&lungimeNume), sizeof(lungimeNume));
        fisierBinar.write(reteta.first.c_str(), lungimeNume);

        // Scrie numărul de ingrediente
        int numarIngrediente = reteta.second.size();
        fisierBinar.write(reinterpret_cast<const char*>(&numarIngrediente), sizeof(numarIngrediente));

        for (auto& ingredient : reteta.second) {
            // Scrie lungimea numelui ingredientului și numele
            int lungimeNumeIngredient = ingredient.first.size();
            fisierBinar.write(reinterpret_cast<const char*>(&lungimeNumeIngredient), sizeof(lungimeNumeIngredient));
            fisierBinar.write(ingredient.first.c_str(), lungimeNumeIngredient);

            // Scrie cantitatea ingredientului
            int cantitate = ingredient.second;
            fisierBinar.write(reinterpret_cast<const char*>(&cantitate), sizeof(cantitate));
        }
    }

    fisierBinar.close();
}

void citesteRetetarDinFisierBinar(const string& numeFisier, map<string, vector<pair<string, int>>>& retetar) {
    ifstream fisierBinar(numeFisier, ios::binary | ios::in);
    if (!fisierBinar) {
        cerr << "Nu s-a putut deschide fisierul pentru citire: " << numeFisier << endl;
        return;
    }

    // Citire număr de rețete
    int numarRetete = 0;
    fisierBinar.read(reinterpret_cast<char*>(&numarRetete), sizeof(numarRetete));

    for (int i = 0; i < numarRetete; ++i) {
        // Citire lungime nume rețetă și nume
        int lungimeNume;
        fisierBinar.read(reinterpret_cast<char*>(&lungimeNume), sizeof(lungimeNume));
        string numeReteta(lungimeNume, '\0');
        fisierBinar.read(&numeReteta[0], lungimeNume);

        // Citire număr ingrediente
        int numarIngrediente;
        fisierBinar.read(reinterpret_cast<char*>(&numarIngrediente), sizeof(numarIngrediente));

        vector<pair<string, int>> ingrediente;
        for (int j = 0; j < numarIngrediente; ++j) {
            // Citire lungime nume ingredient și nume
            int lungimeNumeIngredient;
            fisierBinar.read(reinterpret_cast<char*>(&lungimeNumeIngredient), sizeof(lungimeNumeIngredient));
            string numeIngredient(lungimeNumeIngredient, '\0');
            fisierBinar.read(&numeIngredient[0], lungimeNumeIngredient);

            // Citire cantitate
            int cantitate;
            fisierBinar.read(reinterpret_cast<char*>(&cantitate), sizeof(cantitate));

            ingrediente.push_back(make_pair(numeIngredient, cantitate));
        }

        retetar[numeReteta] = ingrediente;
    }

    fisierBinar.close();
}


void LegendaAdmin() {
    cout << "\nIntrodu 'afisare stoc depozit'\n";
    cout << "Introdu 'afisare retetar'\n";
    cout << "Introdu 'raport1' pentru a afisa cel mai comandat produs din ziua curenta\n";
    cout << "Introdu 'raport2' pentru a afisa cantitatea totala comandata din fiecare produs in ziua curenta\n";
    cout << "Introdu 'meniu principal' pentru a intra in meniul de baza\n";
    cout << "Introdu 'iesire' pentru a iesi din aplicatie\n";
}

int main(int index, char* vector[])
{
    if (index < 3) {
        cerr << "Utilizare " << vector[0] << " stoc.txt retete.txt meniu.txt" << endl;
        return 1;
    }
    string fisierStoc = vector[1];
    string fisierMeniu = vector[2];
    //ManagerRestaurant manager;
    //manager.procesareStoc(fisierStoc);
    //manager.procesareMeniu(fisierMeniu);
    // afisare fisiere text
    //manager.afisareStoc(fisierStoc);

        // scriere ingrediente in binar
    std::vector<Ingredient> ingrediente;
    map<string, std::vector<pair<string, int>>> retetar;
    set<pair<string, int>> meniu;
    procesareStoc(fisierStoc, ingrediente);
    procesareRetetar("retete.txt", retetar);
    procesareMeniu("meniu.txt", meniu);
    scrieIngredientInFisierBinar(ingrediente);;
    scrieMeniuInFisierBinar("meniu.bin", meniu);
    scrieRetetarInFisierBinar("retete.bin", retetar);
    programRestaurant program[7];
    /*  ingrediente.clear();
      meniu.clear();
      retetar.clear();*/
    std::vector<Produs> produseMeniu;
    for (const auto& item : meniu) {
        string numeProdus = item.first;
        auto itReteta = retetar.find(numeProdus);
        if (itReteta != retetar.end()) {
            Retetar reteta(itReteta->second);
            produseMeniu.push_back(Produs(numeProdus, &reteta, true));
        }
    }
    std::vector<int> preturiMeniu;
    for (const auto& item : meniu) {
        preturiMeniu.push_back(item.second);
    }
    Restaurant restaurant1("Restaurant Bucuresti", program, retetar, meniu);
    Comanda c(restaurant1.getMeniu());
    restaurant1.setProgram(0, "Luni", 9, 0, 22, 0);
    restaurant1.setProgram(1, "Marti", 9, 0, 22, 0);
    restaurant1.setProgram(2, "Miercuri", 9, 0, 22, 0);
    restaurant1.setProgram(3, "Joi", 9, 0, 22, 0);
    restaurant1.setProgram(4, "Vineri", 10, 0, 22, 0);
    restaurant1.setProgram(5, "Sambata", 10, 0, 20, 0);
    restaurant1.setProgram(6, "Duminica", 10, 0, 20, 0);
    cout << restaurant1 << endl;
    cout << "Bine ati venit!\n";
    int ok = 1;
    while (ok == 1) {
        cout << "Introduceti 'comanda' pt a face o comanda\n";
        cout << "Introduceti 'meniu' pt a vedea meniul\n";
        cout << "Introduceti 'administrator' pt a intra in modul admin\n";
        cout << "Introdu 'iesire' pentru a iesi din aplicatie\n";
        string input;
        cin >> input;
        if (input == "comanda") {
            cout << "\nComanda se face astfel: CMD nrProduseDiferite produs cantitate\nDe ex: CMD 1 Pizza Casei 1\n";
            cin >> c;
            std::vector<Produs> produseVector(c.getProduse(), c.getProduse() + c.getNrProduse());
            if (restaurant1.verificaIngrediente(produseVector, ingrediente, c.getCantitateProdus())) {
                restaurant1.actualizeazaStoc(ingrediente, produseVector,c.getCantitateProdus());
                restaurant1.adaugaComanda(c);
                c.scrieComandaInBinar(c);
                cout << "Comanda efectuata cu succes!" << endl;
            }
            else {
                cout << "Comanda nu poate fi realizata, nu avem suficiente ingrediente" << endl;
            }
            cout << endl;
            continue;
        }

        if (input == "meniu") {
            cout << "\nMeniu:" << endl;
            for (const auto& item : meniu) {
                cout << "Produs: " << item.first << ", Pret: " << item.second << endl;
            }
            cout << endl;
            continue;
        }
        if (input == "iesire") {
            ok = 0;
            cout << "\nLa revedere!\n";
            break;
        }
        if (input == "administrator") {
            int okAdmin = 1;
            while (okAdmin == 1) {
                LegendaAdmin();
                string inputAdmin;
                getline(cin, inputAdmin);

                if (inputAdmin == "afisare stoc depozit") {
                    cout << endl;
                  /*  citesteIngredientDinFisierBinar(ingrediente);*/
                    for (const auto& ingredient : ingrediente) {
                        cout << "Ingredient: " << ingredient.getDenumire() << ", Cantitate: " << ingredient.getCantitate() << endl;
                    }
                    continue;
                }
                if (inputAdmin == "afisare retetar") {
                    cout << "\nRetetar:" << endl;
                    for (const auto& reteta : retetar) {
                        cout << "Reteta pentru: " << reteta.first << "\nIngrediente:\n";
                        for (const auto& ingrediente : reteta.second) {
                            cout << ingrediente.first << "," << ingrediente.second << ";";
                        }
                        cout << endl;
                    }
                    continue;
                }

                if (inputAdmin == "raport1") {
                    c.raportProdusulCelMaiComandat();
                    continue;
                }
                if (inputAdmin == "raport2") {
                    c.raportCantitatiPerProdus();
                }
                if (inputAdmin == "iesire") {
                    okAdmin = 0; ok = 0;
                    cout << "\nLa revedere!\n";
                    break;
                }
                if (inputAdmin == "meniu principal") {
                    cout << "\n";
                    okAdmin = 0; break;
                }
                else {
                    cout << "\nComanda invalida\n";
                }
            }

        }
        else {
            cout << "\nComanda invalida\n";
        }

    }

    return 0;
}