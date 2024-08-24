// PROJECT: CRM per impresa di assicurazioni

// PATH: ProfessionAI Master AI Engineering Course 2 Programmazione ad alte prestazioni con C++

// AUTHOR: Giacomo Abramo

#include <bits/stdc++.h>
#include <fstream>


using namespace std;

class Cliente {
private:
    std::string nome;
    std::string cognome;

public:
    Cliente(std::string nome, std::string cognome) 
        : nome(nome), cognome(cognome) {}

    std::string getNome() const {
        return nome;
    }

    void setNome(const std::string& nuovoNome) {
        nome = nuovoNome;
    }

    std::string getCognome() const {
        return cognome;
    }

    void setCognome(const std::string& nuovoCognome) {
        cognome = nuovoCognome;
    }
};

class Interazione {
private:
    std::string tipo;  

public:
    Interazione(std::string tipo) 
        : tipo(tipo) {}

    std::string getTipo() const {
        return tipo;
    }

    void setTipo(const std::string& nuovoTipo) {
        tipo = nuovoTipo;
    }
};

class CRM {
private:
    std::vector<Cliente> clienti;
    std::map<int, std::vector<Interazione>> interazioni;

    int generaIdCliente() const;

public:
    void aggiungiCliente(const Cliente& cliente);
    void modificaCliente(const std::string& nome, const std::string& cognome, const std::string& nuovoNome, const std::string& nuovoCognome);
    void eliminaCliente(const std::string& nome, const std::string& cognome);
    void visualizzaClienti() const;
    Cliente* cercaCliente(const std::string& nome, const std::string& cognome);

    void aggiungiInterazione(const Cliente& cliente, const Interazione& interazione);
    void visualizzaInterazioni(const std::string& nome, const std::string& cognome) const;

    void salvaSuFile(const std::string& nomeFile) const;
    void caricaDaFile(const std::string& nomeFile);
};


void CRM::aggiungiCliente(const Cliente& cliente) {
    clienti.push_back(cliente);
}


void CRM::visualizzaClienti() const {
    if (clienti.empty()) {
        std::cout << "Nessun cliente trovato.\n";
        return;
    }
    for (const auto& cliente : clienti) {
        std::cout << "Nome: " << cliente.getNome() << "\n"
                  << "Cognome: " << cliente.getCognome() << "\n"
                  << "----------------------------\n";
    }
}



void CRM::modificaCliente(const std::string& nome, const std::string& cognome, const std::string& nuovoNome, const std::string& nuovoCognome) {
    Cliente* cliente = cercaCliente(nome, cognome);
    if (cliente) {
        if (!nuovoNome.empty()) {
            cliente->setNome(nuovoNome);
        }
        if (!nuovoCognome.empty()) {
            cliente->setCognome(nuovoCognome);
        }
        std::cout << "Cliente modificato con successo!\n";
    } else {
        std::cout << "Cliente non trovato.\n";
    }
}


void CRM::eliminaCliente(const std::string& nome, const std::string& cognome) {
    for (size_t i = 0; i < clienti.size(); ++i) {
        if (clienti[i].getNome() == nome && clienti[i].getCognome() == cognome) {
            clienti.erase(clienti.begin() + i);
            
            auto it = interazioni.find(i);
            if (it != interazioni.end()) {
                interazioni.erase(it);
            }

            std::cout << "Cliente eliminato con successo!\n";
            return;
        }
    }

    std::cout << "Cliente non trovato.\n";
}


Cliente* CRM::cercaCliente(const std::string& nome, const std::string& cognome) {
    for (auto& cliente : clienti) {
        if (cliente.getNome() == nome && cliente.getCognome() == cognome) {
            return &cliente; 
        }
    }
    return nullptr; 
}


void CRM::aggiungiInterazione(const Cliente& cliente, const Interazione& interazione) {
    auto it = std::find_if(clienti.begin(), clienti.end(), [&](const Cliente& c) {
        return c.getNome() == cliente.getNome() && c.getCognome() == cliente.getCognome();
    });

    if (it != clienti.end()) {
        int index = std::distance(clienti.begin(), it);
        interazioni[index].push_back(interazione);
    } else {
        std::cout << "Cliente non trovato.\n";
    }
}


void CRM::visualizzaInterazioni(const std::string& nome, const std::string& cognome) const {
    Cliente* clienteTrovato = nullptr;
    int clienteIndex = -1;

    for (size_t i = 0; i < clienti.size(); ++i) {
        if (clienti[i].getNome() == nome && clienti[i].getCognome() == cognome) {
            clienteTrovato = const_cast<Cliente*>(&clienti[i]);
            clienteIndex = i;
            break;
        }
    }

    if (clienteTrovato) {
        auto it = interazioni.find(clienteIndex);
        if (it != interazioni.end()) {
            const std::vector<Interazione>& interList = it->second;
            if (interList.empty()) {
                std::cout << "Nessuna interazione trovata per questo cliente.\n";
                return;
            }
            std::cout << "Interazioni per il cliente " << nome << " " << cognome << ":\n";
            for (const auto& interazione : interList) {
                std::cout << "- Tipo: " << interazione.getTipo() << "\n";
            }
        } else {
            std::cout << "Nessuna interazione trovata per questo cliente.\n";
        }
    } else {
        std::cout << "Cliente non trovato.\n";
    }
}


void CRM::salvaSuFile(const std::string& nomeFile) const {
    std::ofstream file(nomeFile);

    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file per il salvataggio.\n";
        return;
    }

    file << "Nome,Cognome,Tipo Interazione\n";

    for (size_t i = 0; i < clienti.size(); ++i) {
        const Cliente& cliente = clienti[i];
        file << cliente.getNome() << ","
             << cliente.getCognome() << ",";

        auto it = interazioni.find(i);
        if (it != interazioni.end()) {
            const std::vector<Interazione>& interList = it->second;
            for (size_t j = 0; j < interList.size(); ++j) {
                file << interList[j].getTipo();
                if (j < interList.size() - 1) {
                    file << ";"; 
                }
            }
        }

        file << "\n"; 
    }

    file.close();
    std::cout << "Dati salvati con successo su " << nomeFile << ".\n";
}


void CRM::caricaDaFile(const std::string& nomeFile) {
    std::ifstream file(nomeFile);

    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file per il caricamento.\n";
        return;
    }

    std::string linea;
    std::getline(file, linea);

    while (std::getline(file, linea)) {
        std::istringstream iss(linea);
        std::string nome, cognome, tipoInterazione;

        std::getline(iss, nome, ',');
        std::getline(iss, cognome, ',');
        std::getline(iss, tipoInterazione, ',');

        if (nome.empty() || cognome.empty()) {
            std::cerr << "Nome o cognome vuoti trovati nella riga: " << linea << std::endl;
            continue; 
        }

        clienti.emplace_back(nome, cognome);

        if (!tipoInterazione.empty()) {
            std::stringstream ss(tipoInterazione);
            std::string singolaInterazione;
            int clienteIndex = clienti.size() - 1;

            while (std::getline(ss, singolaInterazione, ';')) {
                interazioni[clienteIndex].emplace_back(singolaInterazione);
            }
        }
    }

    file.close();
    std::cout << "Dati caricati con successo da " << nomeFile << ".\n";
}


void mostraMenu() {
    std::cout << "---------------------\n";
    std::cout << "1. Aggiungi Cliente\n";
    std::cout << "2. Visualizza Clienti\n";
    std::cout << "3. Modifica Cliente\n";
    std::cout << "4. Elimina Cliente\n";
    std::cout << "5. Cerca Cliente\n";
    std::cout << "6. Aggiungi Interazione\n";
    std::cout << "7. Visualizza Interazioni\n";
    std::cout << "8. Salva Dati\n";
    std::cout << "9. Carica Dati\n";
    std::cout << "0. Esci\n";
    std::cout << "---------------------\n";
    std::cout << "Seleziona un'opzione: ";
}


int main() {
    CRM crm;
    int scelta;

    do {
        mostraMenu();
        std::cin >> scelta;
        std::cin.ignore();

        if (scelta == 1) {
            std::string nome, cognome;

            std::cout << "Inserisci il nome del cliente: ";
            std::getline(std::cin, nome);

            std::cout << "Inserisci il cognome del cliente: ";
            std::getline(std::cin, cognome);

            Cliente nuovoCliente(nome, cognome);
            crm.aggiungiCliente(nuovoCliente);

            std::cout << "Cliente aggiunto con successo!\n";
        }
        else if (scelta == 2) {
            crm.visualizzaClienti();
        }
        else if (scelta == 3) {
            std::string nomeRicerca, cognomeRicerca;

            std::cout << "Inserisci il nome del cliente da modificare: ";
            std::getline(std::cin, nomeRicerca);

            std::cout << "Inserisci il cognome del cliente da modificare: ";
            std::getline(std::cin, cognomeRicerca);

            std::string nuovoNome, nuovoCognome;
            std::cout << "Inserisci il nuovo nome (lascia vuoto per non cambiare): ";
            std::getline(std::cin, nuovoNome);
            std::cout << "Inserisci il nuovo cognome (lascia vuoto per non cambiare): ";
            std::getline(std::cin, nuovoCognome);

            crm.modificaCliente(nomeRicerca, cognomeRicerca, nuovoNome, nuovoCognome);
        }
        else if (scelta == 4) {
            std::string nome, cognome;

            std::cout << "Inserisci il nome del cliente da eliminare: ";
            std::getline(std::cin, nome);

            std::cout << "Inserisci il cognome del cliente da eliminare: ";
            std::getline(std::cin, cognome);

            crm.eliminaCliente(nome, cognome);
        }
        
        else if (scelta == 5) {
            std::string nomeRicerca, cognomeRicerca;

            std::cout << "Inserisci il nome del cliente da cercare: ";
            std::getline(std::cin, nomeRicerca);

            std::cout << "Inserisci il cognome del cliente da cercare: ";
            std::getline(std::cin, cognomeRicerca);

            Cliente* clienteTrovato = crm.cercaCliente(nomeRicerca, cognomeRicerca);
            if (clienteTrovato) {
                std::cout << "Cliente trovato:\n";
                std::cout << "Nome: " << clienteTrovato->getNome() << "\n";
                std::cout << "Cognome: " << clienteTrovato->getCognome() << "\n";
            } else {
                std::cout << "Nessun cliente trovato con i criteri di ricerca forniti.\n";
            }
        }
        else if (scelta == 6) {
            std::string nomeRicerca, cognomeRicerca;

            std::cout << "Inserisci il nome del cliente: ";
            std::getline(std::cin, nomeRicerca);

            std::cout << "Inserisci il cognome del cliente: ";
            std::getline(std::cin, cognomeRicerca);

            Cliente* clienteTrovato = crm.cercaCliente(nomeRicerca, cognomeRicerca);
            
            if (clienteTrovato) {
                std::string tipoInterazione;

                std::cout << "Inserisci il tipo di interazione: ";
                std::getline(std::cin, tipoInterazione);

                Interazione nuovaInterazione(tipoInterazione);

                crm.aggiungiInterazione(*clienteTrovato, nuovaInterazione);

                std::cout << "Interazione aggiunta con successo!\n";
            } else {
                std::cout << "Cliente non trovato.\n";
            }
        }
        else if (scelta == 7) {
            std::string nomeRicerca, cognomeRicerca;

            std::cout << "Inserisci il nome del cliente: ";
            std::getline(std::cin, nomeRicerca);

            std::cout << "Inserisci il cognome del cliente: ";
            std::getline(std::cin, cognomeRicerca);

            crm.visualizzaInterazioni(nomeRicerca, cognomeRicerca);
        }

        else if (scelta == 8) {
            crm.salvaSuFile("CRM InsuraPro.txt");
            std::cout << "Dati salvati su file.\n";
        }
        else if (scelta == 9) {
            crm.caricaDaFile("CRM InsuraPro.txt");
            std::cout << "Dati caricati da file.\n";
          
        }
        else if (scelta == 0) {
            std::cout << "Uscita...\n";
        }
        else {
            std::cout << "Opzione non valida. Riprova.\n";
        }
    } while (scelta != 0);

    return 0;
}