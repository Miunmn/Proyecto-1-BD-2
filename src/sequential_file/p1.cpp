#include <iostream>
//#include <bits/stdc++.h>
#include <vector>
#include <fstream>
#include <tuple>
using namespace std;

string file_name;
string aux_name;
string header_name;

struct Registro {
    
    char codigo [6];
    char nombre [20];
    char carrera [20];
    int ciclo;
    int next = 0;

    void setData() {
        cout << "Codigo:\n";
        cin >> codigo;
        cout << "Nombre:\n";
        cin >> nombre;
        cout << "Carrera:\n";
        cin >> carrera;
        cout << "Ciclo:\n";
        cin >> ciclo;
        cout << "Next:\n";
        cin >> next;
    }

    void show() {
        cout << '\n';
        cout << codigo << '\n';
        cout << nombre << '\n';
        cout << carrera << '\n';
        cout << ciclo << '\n';
        cout << next << '\n';
        cout << '\n';
    }

};

void set_header()
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::out | ios::binary);
    int pos = 1;
    header_file.write((char*) &pos, sizeof(int));
    char aux_first_key[6] = {'P', '-', '9', '9', '9'};
    header_file.write((char*) &aux_first_key, 6);
    header_file.close();
}

pair<int, string> read_header()
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::binary);

    int pos_aux_first_key;
    char aux_first_key[6];
    header_file.read((char*) &pos_aux_first_key, sizeof(int));
    header_file.read((char*) &aux_first_key, sizeof(aux_first_key));
    
    header_file.close();

    return {pos_aux_first_key, string(aux_first_key)};
}

void update_header(string aux_first_key, int pos)
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::out | ios::binary);
    char aux_first_key_array[6];
    copy(aux_first_key.begin(), aux_first_key.end(), aux_first_key_array);
    header_file.write((char*) &pos, sizeof(int));
    header_file.write((char*) &aux_first_key_array, 6);
    header_file.close();
}

Registro read_register(int m, string file){
    ifstream data_file;
    data_file.open(file, ios::binary);
    // 1-based index
    int seekpos = (m-1)*(sizeof(Registro));
    data_file.seekg(seekpos, ios::beg);
    Registro registro = Registro();
    data_file.read((char*) &registro, sizeof(Registro));
    data_file.close();
    return registro;
}

int file_size(string file){
    ifstream data_file;
    data_file.open(file, ifstream::ate | ifstream::binary);
    int ret = data_file.tellg()/sizeof(Registro);
    data_file.close();
    return ret;
}

void write_register(Registro registro, string file) {
    auto [pos_aux_first_key, aux_first_key] = read_header();
    ofstream  data_file;
    data_file.open(file, ios::app | ios::binary);
    data_file.write((char*)& registro, sizeof(Registro));
    data_file.close();

    //Update header with new record if necessary
    auto str_code = string(registro.codigo);
    if (str_code < aux_first_key)
    {
        int size = file_size(aux_name);
        update_header(str_code, size);
    }
}

void update_register(Registro registro, int pos, string file) {
    fstream data_file;
    data_file.open(file, ios::in | ios::out | ios::binary);
    int seekpos = (pos)*(sizeof(Registro));
    data_file.seekg(seekpos, ios::beg);
    data_file.write((char*)& registro, sizeof(Registro));
    data_file.close();
}

tuple<Registro, bool, int> sequential_exploration(int m, string key) {
    /*
    Esta funcion regresa el registro con el ultimo codigo
    que es menor o igual que la key buscada dentro de aux. Sigue la 
    secuencia de punteros en cada registro.

    Regreso:
        - candidate::Registro
        - stop_search::bool
        - candidate_logical_position::int
*/

    auto [pos_aux_first_key, aux_first_key] = read_header();

    if (aux_first_key == "P-999")
    {
        Registro not_found = Registro();
        copy(aux_first_key.begin(), aux_first_key.end(), not_found.codigo);
        not_found.next = -2;
        return {not_found, false, -1};
    }

    Registro registro_prev = Registro();
    Registro registro = Registro();
    bool stop_search = false;
    int m_prev = m;

    while (m < 0) {
        registro_prev = registro;
        registro = read_register(-m, aux_name);
        //registro.show();
        if (registro.codigo == key) {
           return {registro, true, m}; //{registro_encontrado, stop, posicion_logica}
        }
        if (registro.codigo > key) {
            return {registro_prev, true, m_prev};
        }
        m_prev = m;
        m = registro.next;
    }

    registro_prev = registro;
    registro = read_register(m, file_name);

    if (registro.codigo > key)
        return {registro_prev, true, m_prev};
    if (registro.codigo == key)
        return {registro, true, m};
    return {registro_prev, false, m_prev};
}

pair<Registro, int> search_util(string key) {
    /*
        Regresa un par con dos elementos:
            - El primero es el registro encontrado
            - El segundo es la posición lógica de dicho registro
    */
    
    Registro registro = Registro();
    Registro prev = Registro();

    int l = 1; 
    int r = file_size(file_name);
    int m = (l + r) / 2, m_prev = m;

    auto [pos_aux_first_key, aux_first_key] = read_header();
    auto first_data_key = string(read_register(1, file_name).codigo);

    if (key < first_data_key)
    {
        auto [seq_reg, is_greater_equal, index] = sequential_exploration(-pos_aux_first_key, key); 
        //seq_reg.show();
        //cout << index << endl;

        auto str_code = string(seq_reg.codigo);
        if (str_code <= key)
        {
            return make_pair(seq_reg, index);
        }
        if (str_code == "P-999")
        {
            auto registro_dummy = Registro();
            registro_dummy.next = 1;

            return make_pair(registro_dummy, 1);
        }
        return make_pair(Registro(), 0);
    }
    
    while (l <= r) {
        prev = registro;
        m_prev = m;
        m = (l + r) / 2;
        registro = read_register(m, file_name);
        //registro.show();
        if (registro.next < 0) {
            auto [seq_reg, is_greater_equal, index] = sequential_exploration(registro.next, key);
            if (is_greater_equal) 
                return {seq_reg, index};
        }
        if (registro.codigo < key)
            l = ++m;
        else if (registro.codigo > key) 
            r = --m;
        else if (registro.codigo == key){
            return {registro, m};
        }
    }

    if (registro.codigo > key) return {prev, m_prev};
    return {registro, m};
}

void add(Registro registro) {
    // search buscar el índice del archivo anterior
    auto [result_register, prev_m] = search_util(registro.codigo);

    if (string(result_register.codigo) == string(registro.codigo))
        return;

    /* search_util devolvió el elemento previo al registro a insertar.

        El next del resultado de search_util debe pasar a ser el next
        del registro a insertar  */

    registro.next = result_register.next;
    write_register(registro, aux_name);
    
    //sacar el tamaño del file aux
    result_register.next = -file_size(aux_name);

    //escribir el prev nuevamente
    //cout << "\n prev_m: " << prev_m <<" \n";

    if (prev_m >= 0) 
        //escribirlo en la posicion prev_m en el data
        update_register(result_register, prev_m-2, file_name);
    
    else 
        //escribirlo en el aux
        update_register(result_register, -prev_m-1, aux_name);
}

void insertAll(vector<Registro> registros) {
    for (auto r : registros) 
        add(r);
}

// testear con los indexados en aux
Registro search (string key) {

    auto [pos_aux_first_key, aux_first_key] = read_header();

    auto registro = search_util(key).first;

    if (registro.codigo == key) return registro;

    return Registro();
}

vector<Registro> rangeSearch(string begin, string end) {
    vector<Registro> registros;
    //TODO
    return registros;
}

void scan_all() {
    ifstream file;
    file.open(file_name, ios::in | ios::binary);
    Registro *registro;
    if (file.is_open()) {
        while (!file.eof()) {
            file.read((char*) &registro, sizeof(Registro));
            registro->show();
        }
        file.close();
    }
}

void delete_register(string key)
{
    //Primero lo busco
    auto [result, logical_position] = search_util(key);
    if (result.next == 0)
        return;

    //Obtengo la posicion logica del ultimo key eliminado gracias al header
    if (logical_position > 0)
    {
        
    }

    //Actualizo la posicion logica del ultimo key eliminado en el header

    //Escribo en esta posicion la posicion logica del registro anteriormente eliminado
}

void clear_file(string file_name)
{
    ofstream ofs;
    ofs.open(file_name, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void restructure()
{
    /*
        Recorrer secuencialmente todos los 
        registros, empezando desde la menor key
        ya sea en aux o en data.
    */
    auto [pos_aux_first_key, aux_first_key] = read_header();
    string data_first_key = string(read_register(1, file_name).codigo);

    int cur_logical_position;
    if (aux_first_key < data_first_key)
        cur_logical_position = pos_aux_first_key;
    else 
        cur_logical_position = 1;

    //Archivo temporal donde se escribiran los registros
    fstream temp_file;
    Registro cur_register;
    do 
    {
        string file = (cur_logical_position < 0 ? aux_name : file_name);
        cur_register = read_register(cur_logical_position, file);
        write_register(cur_register, "temp.dat");
    }while(cur_register.next != 0);
    temp_file.close();

    //Limpiamos el aux_original
    clear_file(aux_name);
    //Seteamos de nuevo el header
    set_header();

    //Borramos el data file original
    char file_name_array[10];
    copy(file_name.begin(), file_name.end(), file_name_array);
    if(remove(file_name_array))
        cout << "Removed previous data file successfully"; 
    else
        cout << "Error removing data file";

    //Renombramos el archivo temporal
    if(rename("temp.dat", "data.dat"))
        cout << "Renamed temp file successfully";
    else
        cout << "Error renaming temp file";
}


int main() {

    file_name = "data.dat";
    aux_name = "aux.dat";
    header_name = "header.dat";

    //search_util("P-232").first.show();
    //return 0;

    //First add: P-024 Macarena CS 5 
    //Then add: P-050 Miun CS 10
    //Finally add: P-030 Jose CS 8

    while (true)
    {
        Registro add_r = Registro();
        add_r.setData();
        add(add_r);


        cout << "Showing data for aux file\n";
        for (int i = 1; i <= file_size(aux_name); i++) {
            Registro r2 = read_register(i, aux_name);
            r2.show();
        }


        cout << "Showing data for data file\n";
        for (int i = 1; i <= file_size(file_name); i++) {
            Registro r2 = read_register(i, file_name);
            r2.show();
        }
    }
   return 0;
   

    clear_file(file_name);
    clear_file(aux_name);
    Registro record;
    for (int i = 1; i <= 7; i++) {
        record.setData();
        write_register(record, file_name);
    } 
    set_header();
    cout << "Showing data for data file\n";
    for (int i = 1; i <= 7; i++) {
        Registro r2 = read_register(i, file_name);
        r2.show();
    }

}
