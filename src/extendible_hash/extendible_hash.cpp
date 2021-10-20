#include "extendible_hash.hpp"
#include <stdio.h>

using namespace std;

template <class T, class bucket>
extendible_hash<T, bucket> :: extendible_hash(int D, string index_filename, string data_filename){
    this->D = D;
    
    index_file.open(index_filename, ios::in | ios::out | ios::binary);
	data_file.open(data_filename, ios::in | ios::out | ios::binary);

    if (index_empty()) {
        cout << "Iniciando index \n";
        /* starts index with keys 0 and 1, initialize its buckets with no elements */
        start_index(0b0, 0);
        start_index(0b1, 0);
    }

}

template <class T, class bucket>
extendible_hash<T, bucket> :: ~extendible_hash(){
    index_file.close();
    data_file.close();
}

template <class T, class bucket>
int extendible_hash<T, bucket> :: index_size(){
    index_file.seekg (0, index_file.end);
    return index_file.tellg();
}

template <class T, class bucket>
int extendible_hash<T, bucket> :: data_size(){
    data_file.seekg (0, index_file.end);
    return data_file.tellg();
}

template <class T, class bucket>
bool extendible_hash<T, bucket> :: index_empty() {
    return index_size() == 0;
}

template <class T, class bucket>
index_hash extendible_hash<T, bucket> :: start_index(int key, int d) {
    index_hash ih = index_hash();
    int bucket_position = data_size()/sizeof(bucket);
    ih.set(key, bucket_position, d);
    index_file.seekp(index_size(), ios::beg);
    index_file.write((char*)& ih, sizeof(index_hash));

    bucket ihb = bucket();
    ihb.set(0, -1);
    write_bucket(ihb, bucket_position);

    return ih;
}

template <class T, class bucket>
index_hash extendible_hash<T, bucket> :: read_index(int position) {
    int seekpos = position*(sizeof(index_hash));
    index_file.seekg(seekpos, ios::beg);
    index_hash ih;
    index_file.read((char*) &ih, sizeof(index_hash));
    return ih;
}

template <class T, class bucket>
index_hash extendible_hash<T, bucket> :: search_index(int key, int d) {
    index_hash ih;
    int position = 0;
    int cant_index_hash = index_size()/sizeof(index_hash);
    while (position < cant_index_hash) {
        ih = read_index(position);
        if (ih.key == key && ih.d == d) return ih; 
        position++;
    }
    ih.key = -1;
    return ih;
}

template <class T, class bucket>
index_hash extendible_hash<T, bucket> :: iterate_key(int key) {
    index_hash ih;
    int search_key = 0;
    for(int i = 0; i <= D; i++) {
        if (key & (1<<i))
            search_key += 1<<i;
        ih = search_index(search_key, i);
        if  (ih.key == search_key) break;
    }
    return ih;
}

template <class T, class bucket>
bucket extendible_hash<T, bucket> :: read_bucket(int position) {
    int seekpos = position*(sizeof(bucket));
    data_file.seekg(seekpos, ios::beg);
    bucket ihb = bucket();
    data_file.read((char*) &ihb, sizeof(bucket));
    return ihb;
}

template <class T, class bucket>
void extendible_hash<T, bucket> :: write_bucket(bucket& ihb, int bucket_position) {
    data_file.seekp(bucket_position*(sizeof(bucket)), ios::beg);
    data_file.write((char*)& ihb, sizeof(bucket));
}

template <class T, class bucket>
void extendible_hash<T, bucket> :: increment_depth(int key) {
    index_hash ih;
    int position = 0;
    int cant_index_hash = index_size()/sizeof(index_hash);
    while (position < cant_index_hash) {
        ih = read_index(position);
        if (ih.key == key) {
            ih.d++;
            index_file.seekp(position*(sizeof(index_hash)), ios::beg);
            index_file.write((char*)& ih, sizeof(index_hash));
            return;
        }
        position++;
    }
    cerr << "Increment_depth: Error during file manipulation.\n";
    ih.key = -1;
}

template <class T, class bucket>
void extendible_hash<T, bucket> :: print_all() {
    for (int i = 0; i < index_size()/sizeof(index_hash); i++) {
        index_hash ih = read_index(i);
        cout << ih.key << ", " << ih.d << ": ";
        bucket ihb = read_bucket(ih.bucket_position);
        for (int j = 0; j < ihb.cant; j++) 
            cout << ihb.registros[j].rank << " ";
        while(ihb.next != -1) {
            cout << "| ";
            ihb = read_bucket(ihb.next);
            for (int j = 0; j < ihb.cant; j++) 
                cout << ihb.registros[j].rank << " ";
        }   
        cout << '\n' << '\n';
    }
    cout << "\n";
}

template <class T, class bucket>
vector <T> extendible_hash<T, bucket> :: select_all() {
    vector <T> registros;
    for (int i = 0; i < index_size()/sizeof(index_hash); i++) {
        index_hash ih = read_index(i);
        bucket ihb = read_bucket(ih.bucket_position);
        for (int j = 0; j < ihb.cant; j++) 
            registros.push_back(ihb.registros[j]);
        while(ihb.next != -1) {

            ihb = read_bucket(ihb.next);
            for (int j = 0; j < ihb.cant; j++) 
                registros.push_back(ihb.registros[j]);
        }   
    }
    return registros;
}



template <class T, class bucket>
int extendible_hash<T, bucket> :: f(int registro_key) {
    return registro_key % (1<<(D+1));
    /*
    if (codigo[0] == 'm')
        return 0b01000;
    if (codigo[0] == 'n')
        return 0b00000;
    if (codigo[0] == 'k' || codigo[0] == 'l')
        return 0b01010;
    if (codigo[0] == 'f')
        return 0b01110;
    if (codigo[0] == 'w')
        return 0b00110;
    if (codigo[0] == 'o' || codigo[0] == 'p')
        return 0b01001;
    return 0b11111;
    */
}

template <class T, class bucket>
void extendible_hash<T, bucket> :: add_to_bucket(index_hash& ih, bucket& ihb, T registro) {
    ihb.registros[ihb.cant] = registro;
    ihb.cant++;
    write_bucket(ihb, ih.bucket_position);
}

template <class T, class bucket>
void extendible_hash<T, bucket> :: insert_register(T registro){

    int key = f(registro.rank);
    index_hash ih = iterate_key(key);

    bucket ihb = read_bucket(ih.bucket_position);

    if (ihb.cant < fb) 
        add_to_bucket(ih, ihb, registro);

    else overflow_handle(ih, ihb, registro);

}

template <class T, class bucket>
void extendible_hash<T, bucket> :: resize(index_hash& ih, bucket& ihb, index_hash& ih_new, bucket& ihb_new, int d) {
    int ihb_cant = 0, ihb_new_cant = 0;
    /* los registros se distribuyen en los buckets */
    for (int i = 0; i < ihb.cant; i++) {
        T registro_cur = ihb.registros[i];
        int key_cur = f(registro_cur.rank);
        /*  si bit d está prendido (porque los bits con dx < d serán todos iguales) */
        if (key_cur & (1<<d)) {
            ihb_new.registros[ihb_new_cant] = registro_cur;
            ihb_new_cant++;
        }
        else {
            ihb.registros[ihb_cant] = registro_cur;
            ihb_cant++;
        }
    }
    ihb.cant = ihb_cant;
    ihb_new.cant = ihb_new_cant;
    /* consistencia en memoria, escribir buckets */
    write_bucket(ihb, ih.bucket_position);
    write_bucket(ihb_new, ih_new.bucket_position);
}
 
template <class T, class bucket>
void extendible_hash<T, bucket> :: overflow_handle(index_hash& ih, bucket& ihb, T& registro) {  
    /* queda espacio para aumentar el d */
    if (ih.d < D) {
        /* creando nuevo índice y bucket asociado (appennd de 1) */
        int d = ih.d+1;
        int ih_new_key = ih.key + (1<<d);
        index_hash ih_new = start_index(ih_new_key, d);
        bucket ihb_new = read_bucket(ih_new.bucket_position);
        /* actualizando profundidad local (append de 0) */
        ih.d = d;
        increment_depth(ih.key);
        /* reinsertar registros */
        resize(ih, ihb, ih_new, ihb_new, d);
        /* intentar insertar registro nuevo */
        int key_cur = f(registro.rank);
        if (key_cur & (1<<d)) {
            /* espacio disponible */
            if (ihb_new.cant < fb) 
                add_to_bucket(ih_new, ihb_new, registro);
            /* no hay espacio disponible */
            else overflow_handle(ih_new, ihb_new, registro);
        }
        else {
            /* espacio disponible */
            if (ihb.cant < fb) 
                add_to_bucket(ih, ihb, registro);
            /* no hay espacio disponible */
            else overflow_handle(ih, ihb, registro);
        }
    } 

    /* se procede a overflow bucket */
    else {
        int bucket_position = ih.bucket_position;
        while (ihb.next != -1) {
            bucket_position = ihb.next;
            ihb = read_bucket(bucket_position);
        }
        if (ihb.cant == fb) {
            ihb.next = data_size()/sizeof(bucket);
            /* adding a pointer to the next bucket */
            write_bucket(ihb, bucket_position);
            bucket_position = ihb.next;
            /* creating a new bucket */
            ihb = bucket();
            ihb.set(0, -1);
        }
        ihb.registros[ihb.cant] = registro;
        ihb.cant++;
        write_bucket(ihb, bucket_position);
    }
}

template <class T, class bucket>
bool extendible_hash<T, bucket> :: search_register(int registro_key, T& registro) {
    //TODO
    int key = f(registro_key);  
    index_hash ih = iterate_key(key);
    bucket ihb = read_bucket(ih.bucket_position);
    for (int i = 0; i < ihb.cant; i++) {
        registro =  ihb.registros[i];
        if (registro.rank == registro_key)
            return true;
    }
    while (ihb.next != -1) {
        ihb = read_bucket(ihb.next);
        for (int i = 0; i < ihb.cant; i++) {
            registro =  ihb.registros[i];
            //registro_key_search = 
            if (registro.key == registro_key)
                return true;
        }
    }
    cerr << "No se encontró el registro. \n";
    return false;
}

template <class T, class bucket>
bool extendible_hash<T, bucket> :: delete_register(int registro_key) {
    T registro;
    int key = f(registro_key);  
    index_hash ih = iterate_key(key);
    int bucket_position = ih.bucket_position;
    bucket ihb = read_bucket(bucket_position);
    for (int i = 0; i < ihb.cant; i++) {
        registro =  ihb.registros[i];
        if (registro.rank == registro_key) {
            for (int j = i+1; j < ihb.cant; j++) 
                ihb.registros[j-1] = ihb.registros[j];
            ihb.cant--;
            write_bucket(ihb, bucket_position);
            return true;
        }
    }
    while (ihb.next != -1) {
        bucket_position = ihb.next;
        ihb = read_bucket(bucket_position);
        for (int i = 0; i < ihb.cant; i++) {
            registro =  ihb.registros[i];
            if (registro.rank == registro_key) {
                for (int j = i+1; j < ihb.cant; j++) 
                    ihb.registros[ihb.cant-1] = registro;
                ihb.cant--;
                write_bucket(ihb, bucket_position);
                return true;
            }
        }
    }
    cerr << "No se encontró el registro. \n";
    return false;
}